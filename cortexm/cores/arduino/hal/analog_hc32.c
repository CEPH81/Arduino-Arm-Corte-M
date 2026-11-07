/*
 * analog_hc32.c - ADC, PWM, temporizador periódico y flash del HC32F460
 * con los drivers DDL de HDSC.
 *
 *  ADC   ADC1 (16 canales: PA0-PA7, PB0-PB1, PC0-PC5), 12 bits, secuencia A.
 *  PWM   Timer A (unidades 1-6, 8 canales, 16 bits, reloj PCLK1). Las
 *        unidades comparten frecuencia entre sus 8 canales.
 *  Timer Timer A4 (sus pines no se usan para PWM) con la IRQ040.
 *  Flash sectores de 8 KB; escritura por palabras (EFM).
 *  Tabla de pines: framework-arduino-hc32f46x (shadow578), variante genérica.
 */
#include "Arduino.h"

#if defined(HC32F460)

#include "hc32f460_adc.h"
#include "hc32f460_timera.h"

extern void cmHc32IrqRoute(uint32_t irqn, uint32_t src, uint32_t prio);

/* {pin, unidad, canal (1-8), función (4 = Func_Tima0, 5 = Func_Tima1)} */
typedef struct { uint8_t pin, unit, ch, fn; } hc_pwm_t;
static const hc_pwm_t pwm_pins[] = {
  {PA0, 2, 1, 4}, {PA1, 2, 2, 4}, {PA2, 5, 1, 5}, {PA3, 5, 2, 5},
  {PA4, 3, 5, 5}, {PA5, 3, 6, 5}, {PA6, 3, 1, 5}, {PA7, 3, 2, 5},
  {PA8, 1, 1, 4}, {PA9, 1, 2, 4}, {PA10, 1, 3, 4}, {PA11, 1, 4, 4},
  {PA12, 6, 1, 5}, {PA13, 6, 2, 5}, {PA14, 6, 3, 5}, {PA15, 6, 4, 5},
  {PB0, 3, 3, 5}, {PB1, 3, 4, 5}, {PB2, 1, 8, 4}, {PB3, 6, 5, 5},
  {PB4, 6, 6, 5}, {PB5, 6, 7, 5}, {PB6, 6, 8, 5}, {PB10, 5, 8, 5},
  {PB12, 1, 8, 4}, {PB13, 1, 5, 4}, {PB14, 1, 6, 4}, {PB15, 1, 7, 4},
  {PC0, 2, 5, 4}, {PC1, 2, 6, 4}, {PC2, 2, 7, 4}, {PC3, 2, 8, 4},
  {PC4, 3, 7, 5}, {PC5, 3, 8, 5}, {PC6, 5, 8, 5}, {PC7, 5, 7, 5},
  {PC8, 5, 6, 5}, {PC9, 5, 5, 5}, {PC10, 5, 1, 5}, {PC11, 5, 2, 5},
  {PC12, 5, 3, 5},
};
#define NELEM(a) (sizeof(a) / sizeof((a)[0]))

static M4_TMRA_TypeDef *const tmra[] = { 0, M4_TMRA1, M4_TMRA2, M4_TMRA3, M4_TMRA4, M4_TMRA5, M4_TMRA6 };
static const uint32_t tmra_clk[] = { 0, PWC_FCG2_PERIPH_TIMA1, PWC_FCG2_PERIPH_TIMA2, PWC_FCG2_PERIPH_TIMA3,
                                     PWC_FCG2_PERIPH_TIMA4, PWC_FCG2_PERIPH_TIMA5, PWC_FCG2_PERIPH_TIMA6 };

static const hc_pwm_t *pwm_find(uint8_t pin)
{
  for (unsigned i = 0; i < NELEM(pwm_pins); i++) if (pwm_pins[i].pin == pin && cmPinValid(pin)) return &pwm_pins[i];
  return 0;
}

static uint32_t pclk1(void)
{
  stc_clk_freq_t f;
  CLK_GetClockFreq(&f);
  return f.pclk1Freq;
}

/* Divisor (potencia de 2, 0..10) y periodo para 'ticks' ciclos de PCLK1 */
static uint16_t tmra_setup(uint8_t u, uint64_t ticks)
{
  uint32_t div = 0;
  while (div < 10u && (ticks >> div) > 0xFFFFu) div++;
  uint32_t per = (uint32_t)(ticks >> div);
  if (per < 2u) per = 2u;
  if (per > 0xFFFFu) per = 0xFFFFu;
  PWC_Fcg2PeriphClockCmd(tmra_clk[u], Enable);
  stc_timera_base_init_t b;
  memset(&b, 0, sizeof b);
  b.enClkDiv = (en_timera_clk_div_t)div;
  b.enCntMode = TimeraCountModeSawtoothWave;
  b.enCntDir = TimeraCountDirUp;
  b.enSyncStartupEn = Disable;
  b.u16PeriodVal = (uint16_t)(per - 1u);             /* cuenta 0..per-1 */
  TIMERA_Cmd(tmra[u], Disable);
  TIMERA_BaseInit(tmra[u], &b);
  return (uint16_t)(per - 1u);
}

/* ---------------------------------------------------------------- PWM */
static uint32_t pwm_freq[7];
static uint8_t  pwm_on[7];             /* bit n = canal n+1 activo */

bool cmPwmPin(uint8_t pin) { return pwm_find(pin) != 0; }

bool cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits)
{
  const hc_pwm_t *e = pwm_find(pin);
  if (!e || !freq) return false;
  uint8_t u = e->unit;
  en_timera_channel_t ch = (en_timera_channel_t)(e->ch - 1u);
  if (pwm_freq[u] != freq) {
    tmra_setup(u, pclk1() / freq);
    pwm_freq[u] = freq;
    pwm_on[u] = 0;
    TIMERA_Cmd(tmra[u], Enable);
  }
  uint32_t top = (uint32_t)tmra[u]->PERAR + 1u;
  uint32_t cmp = (uint32_t)(((uint64_t)value * top) >> bits);
  if (!(pwm_on[u] & (1u << ch))) {
    stc_timera_compare_init_t c;
    memset(&c, 0, sizeof c);
    c.u16CompareVal = 0;
    c.enStartCountOutput = TimeraCountStartOutputHigh;
    c.enStopCountOutput = TimeraCountStopOutputLow;
    c.enCompareMatchOutput = TimeraCompareMatchOutputLow;
    c.enPeriodMatchOutput = TimeraPeriodMatchOutputHigh;
    c.enSpecifyOutput = TimeraSpecifyOutputInvalid;
    c.enCacheEn = Disable;
    TIMERA_CompareInit(tmra[u], ch, &c);
    TIMERA_CompareCmd(tmra[u], ch, Enable);
    cmPinFunction(pin, e->fn, false, 0);
    pwm_on[u] |= (uint8_t)(1u << ch);
  }
  if (cmp == 0)        TIMERA_SpecifyOutputSta(tmra[u], ch, TimeraSpecifyOutputLow);
  else if (cmp >= top) TIMERA_SpecifyOutputSta(tmra[u], ch, TimeraSpecifyOutputHigh);
  else {
    TIMERA_SpecifyOutputSta(tmra[u], ch, TimeraSpecifyOutputInvalid);
    TIMERA_SetCompareValue(tmra[u], ch, (uint16_t)cmp);
  }
  return true;
}

void cmPwmStop(uint8_t pin)
{
  const hc_pwm_t *e = pwm_find(pin);
  if (!e) return;
  uint8_t u = e->unit, ch = (uint8_t)(e->ch - 1u);
  TIMERA_CompareCmd(tmra[u], (en_timera_channel_t)ch, Disable);
  pwm_on[u] &= (uint8_t)~(1u << ch);
  if (!pwm_on[u]) { TIMERA_Cmd(tmra[u], Disable); pwm_freq[u] = 0; }
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* ---------------------------------------------------- Timer periódico */
#define TMR_UNIT  4
#define TMR_IRQ   40u
static void (*volatile tmr_cb)(void);

bool cmTimerStart(uint32_t us, void (*cb)(void))
{
  if (!cb || !us) return false;
  tmra_setup(TMR_UNIT, ((uint64_t)pclk1() * us) / 1000000u);
  tmr_cb = cb;
  TIMERA_ClearFlag(tmra[TMR_UNIT], TimeraFlagOverflow);
  TIMERA_IrqCmd(tmra[TMR_UNIT], TimeraIrqOverflow, Enable);
  cmHc32IrqRoute(TMR_IRQ, INT_TMRA4_OVF, 4);
  TIMERA_Cmd(tmra[TMR_UNIT], Enable);
  return true;
}

void cmTimerStop(void)
{
  TIMERA_Cmd(tmra[TMR_UNIT], Disable);
  TIMERA_IrqCmd(tmra[TMR_UNIT], TimeraIrqOverflow, Disable);
  NVIC_DisableIRQ((IRQn_Type)TMR_IRQ);
  tmr_cb = 0;
}

void IRQ040_Handler(void)
{
  TIMERA_ClearFlag(tmra[TMR_UNIT], TimeraFlagOverflow);
  void (*cb)(void) = tmr_cb;
  if (cb) cb();
}

/* ---------------------------------------------------------------- ADC */
static bool adc_ready;

uint32_t cmAdcRefMv(void) { return 3300u; }
int32_t  cmAdcTemperature(void) { return CM_TEMP_NONE; }

static int adc_channel(uint8_t pin)
{
  if (pin <= PA7) return pin - PA0;                 /* ADC1_IN0..7 */
  if (pin == PB0 || pin == PB1) return 8 + (pin - PB0);
  if (pin >= PC0 && pin <= PC5) return 10 + (pin - PC0);
  return -1;
}

int cmAdcRead(uint8_t pin)
{
  int ch = adc_channel(pin);
  if (ch < 0 || !cmPinValid(pin)) return CM_ADC_NONE;
  if (!adc_ready) {
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC1, Enable);
    stc_adc_init_t a;
    memset(&a, 0, sizeof a);
    a.enResolution = AdcResolution_12Bit;
    a.enDataAlign = AdcDataAlign_Right;
    a.enAutoClear = AdcClren_Enable;
    a.enScanMode = AdcMode_SAOnce;
    a.enRschsel = AdcRschsel_Restart;
    ADC_Init(M4_ADC1, &a);
    adc_ready = true;
  }
  cmPinAnalog(pin);
  uint8_t st = 0xFF;                                 /* tiempo de muestreo máximo */
  stc_adc_ch_cfg_t c;
  memset(&c, 0, sizeof c);
  c.u32Channel = 1UL << ch;
  c.u8Sequence = ADC_SEQ_A;
  c.pu8SampTime = &st;
  ADC_AddAdcChannel(M4_ADC1, &c);
  ADC_ClrEocFlag(M4_ADC1, ADC_SEQ_A);
  ADC_StartConvert(M4_ADC1);
  uint32_t t0 = micros();
  while (ADC_GetEocFlag(M4_ADC1, ADC_SEQ_A) != Set) {
    if (micros() - t0 > 1000u) { ADC_DelAdcChannel(M4_ADC1, 1UL << ch); return CM_ADC_NONE; }
  }
  int v = ((volatile uint16_t *)&M4_ADC1->DR0)[ch] & 0xFFF;
  ADC_ClrEocFlag(M4_ADC1, ADC_SEQ_A);
  ADC_DelAdcChannel(M4_ADC1, 1UL << ch);
  return v;
}

/* ---------------------------------------------------------------- Flash */
/* El DDL da por vencido el plazo antes de que termine un borrado: se
 * comprueba el resultado leyendo la flash. */
bool cmFlashErase(uint32_t addr)
{
  addr &= ~(CM_EEPROM_PAGE - 1u);
  uint32_t m = cmIrqSave();
  EFM_Unlock();
  EFM_FlashCmd(Enable);
  (void)EFM_SectorErase(addr);
  EFM_Lock();
  cmIrqRestore(m);
  for (uint32_t i = 0; i < CM_EEPROM_PAGE; i += 4u)
    if (*(const volatile uint32_t *)(addr + i) != 0xFFFFFFFFu) return false;
  return true;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  uint32_t m = cmIrqSave();
  EFM_Unlock();
  EFM_FlashCmd(Enable);
  for (uint32_t i = 0; i + 4u <= len; i += 4u) {
    uint32_t w;
    memcpy(&w, p + i, 4);
    (void)EFM_SingleProgram(addr + i, w);
  }
  EFM_Lock();
  cmIrqRestore(m);
  return memcmp((const void *)addr, data, len & ~3u) == 0;
}

#endif
