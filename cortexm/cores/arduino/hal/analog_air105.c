/*
 * analog_air105.c - ADC, PWM, temporizador periódico y flash del Air105.
 * Basado en el BSP de LuatOS (core_adc.c, core_hwtimer.c) y el SDK de
 * MegaHunt (air105_qspi.c).
 *
 *  ADC   12 bits; canales 1, 2, 4, 5, 6 en PC0, PC1, PC3, PC4, PC5 (función 2).
 *        Con el divisor interno activado mide de 0 a ~3.76 V (PC5: hasta ~5 V,
 *        se escala al mismo rango).
 *  PWM   TIM0..TIM5 en PB0, PB1, PA2, PA3, PC6, PC7 (función 2). Cada timer
 *        cuenta un tiempo en bajo y otro en alto (reloj PCLK).
 *  Timer TIM6 (sin pin) con su IRQ TIM0_6.
 *  Flash QSPI externa: sectores de 4 KB por las funciones de la ROM.
 */
#include "Arduino.h"

#if defined(CM_VENDOR_MH)

#include "air105_qspi.h"
#include "air105_cache.h"

#define NELEM(a) (sizeof(a) / sizeof((a)[0]))
#define ALT_FN2  2u

/* ------------------------------------------------------------- ADC */
static const uint8_t adc_pins[][2] = { {PC0, 1}, {PC1, 2}, {PC3, 4}, {PC4, 5}, {PC5, 6} };
static bool adc_ready;

uint32_t cmAdcRefMv(void) { return 3760u; }
int32_t  cmAdcTemperature(void) { return CM_TEMP_NONE; }

int cmAdcRead(uint8_t pin)
{
  int ch = -1;
  for (unsigned i = 0; i < NELEM(adc_pins); i++) if (adc_pins[i][0] == pin) ch = adc_pins[i][1];
  if (ch < 0) return CM_ADC_NONE;
  if (!adc_ready) {
    SYSCTRL->CG_CTRL1 |= SYSCTRL_APBPeriph_ADC;
    ADC0->ADC_CR1 = 0;
    ADC0->ADC_FIFO_THR = 13;                       /* 14 muestras por lectura */
    ADC0->ADC_FIFO = ADC_FIFO_ENABLE | ADC_FIFO_RESET;
    ADC0->ADC_CR2 &= ~ADC_CR2_BUFF_ENABLE;
    ADC0->ADC_CR2 |= ADC_DIV_RESISTOR_EN_BIT;      /* rango 0-3.76 V */
    adc_ready = true;
  }
  cmPinFunction(pin, ALT_FN2, false, 0);
  ADC0->ADC_FIFO = ADC_FIFO_ENABLE | ADC_FIFO_RESET;
  uint32_t t0 = micros();
  while (ADC0->ADC_FIFO & ADC_FIFO_RESET) if (micros() - t0 > 1000u) return CM_ADC_NONE;
  ADC0->ADC_CR1 = ADC_CR1_SAMP_ENABLE | (uint32_t)ch;
  t0 = micros();
  while (!(ADC0->ADC_SR & ADC_SR_DONE_FLAG)) {
    if (micros() - t0 > 5000u) { ADC0->ADC_CR1 = 0; return CM_ADC_NONE; }
  }
  ADC0->ADC_CR1 = 0;
  uint32_t v = 0;
  for (int i = 0; i < 14; i++) v = ADC0->ADC_DATA & 0xFFFu;   /* las primeras se descartan */
  ADC0->ADC_FIFO = ADC_FIFO_ENABLE | ADC_FIFO_RESET;
  if (ch == 6) {                                    /* PC5: divisor distinto (LuatOS) */
    v = (v * 470u * 1511u / 279u / 2048u) * 4096u / 3760u;
    if (v > 4095u) v = 4095u;
  }
  return (int)v;
}

/* ------------------------------------------------------------- PWM */
static const uint8_t pwm_pins[6] = { PB0, PB1, PA2, PA3, PC6, PC7 };   /* TIM0..TIM5 */

static int pwm_tim(uint8_t pin)
{
  for (int i = 0; i < 6; i++) if (pwm_pins[i] == pin) return i;
  return -1;
}

static uint32_t tim_clk(void) { return cmPeriphClock(TIMM0_BASE); }

static void tim_clock_on(void) { SYSCTRL->CG_CTRL1 |= SYSCTRL_APBPeriph_TIMM0; }

bool cmPwmPin(uint8_t pin) { return pwm_tim(pin) >= 0; }

bool cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits)
{
  int t = pwm_tim(pin);
  if (t < 0 || !freq) return false;
  uint32_t total = tim_clk() / freq;
  if (total < 2u) total = 2u;
  uint32_t high = (uint32_t)(((uint64_t)value * total) >> bits);
  volatile uint32_t *ctrl = &TIMM0->TIM[t].ControlReg;
  if (high == 0 || high >= total) {                 /* 0 % o 100 %: el pin como GPIO */
    *ctrl = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, high ? HIGH : LOW);
    return true;
  }
  tim_clock_on();
  TIMM0->TIM[t].LoadCount = total - high;           /* tiempo en bajo */
  TIMM0->TIM_ReloadCount[t] = high;                 /* tiempo en alto */
  if (!(*ctrl & TIMER_CONTROL_REG_TIMER_ENABLE)) {
    *ctrl = TIMER_CONTROL_REG_TIMER_MODE | TIMER_CONTROL_REG_TIMER_PWM | TIMER_CONTROL_REG_TIMER_INTERRUPT;
    *ctrl |= TIMER_CONTROL_REG_TIMER_ENABLE;
    cmPinFunction(pin, ALT_FN2, false, 0);
  }
  return true;
}

void cmPwmStop(uint8_t pin)
{
  int t = pwm_tim(pin);
  if (t < 0) return;
  TIMM0->TIM[t].ControlReg = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* ---------------------------------------------------- Timer periódico */
#define TMR_N 6
static void (*volatile tmr_cb)(void);

bool cmTimerStart(uint32_t us, void (*cb)(void))
{
  if (!cb || !us) return false;
  uint64_t ticks = ((uint64_t)tim_clk() * us) / 1000000u;
  if (ticks < 2u) ticks = 2u;
  if (ticks > 0xFFFFFFFFu) ticks = 0xFFFFFFFFu;
  tim_clock_on();
  TIMM0->TIM[TMR_N].ControlReg = 0;
  TIMM0->TIM[TMR_N].LoadCount = (uint32_t)ticks - 1u;
  (void)TIMM0->TIM[TMR_N].EOI;
  tmr_cb = cb;
  NVIC_SetPriority(TIM0_6_IRQn, 4);
  NVIC_ClearPendingIRQ(TIM0_6_IRQn);
  NVIC_EnableIRQ(TIM0_6_IRQn);
  TIMM0->TIM[TMR_N].ControlReg = TIMER_CONTROL_REG_TIMER_MODE | TIMER_CONTROL_REG_TIMER_ENABLE;  /* IRQ sin máscara */
  return true;
}

void cmTimerStop(void)
{
  TIMM0->TIM[TMR_N].ControlReg = 0;
  NVIC_DisableIRQ(TIM0_6_IRQn);
  tmr_cb = 0;
}

void TIM0_6_IRQHandler(void)
{
  (void)TIMM0->TIM[TMR_N].EOI;
  void (*cb)(void) = tmr_cb;
  if (cb) cb();
}

/* ------------------------------------------------------------ Flash */
bool cmFlashErase(uint32_t addr)
{
  SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_DMA, ENABLE);
  addr &= ~0xFFFu;
  (void)FLASH_EraseSector(addr);
  CACHE_CleanAll(CACHE);
  for (uint32_t i = 0; i < 0x1000u; i += 4u)          /* la ROM no documenta su código de retorno */
    if (*(const volatile uint32_t *)(addr + i) != 0xFFFFFFFFu) return false;
  return true;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  static uint32_t buf[64];                          /* página de 256 bytes, alineada */
  SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_DMA, ENABLE);
  const uint8_t *p = (const uint8_t *)data;
  uint32_t a = addr, n0 = len;
  while (len) {
    uint32_t n = 256u - (a & 255u);                 /* sin cruzar el final de la página */
    if (n > len) n = len;
    memcpy(buf, p, n);
    (void)FLASH_ProgramPage(a, n, (uint8_t *)buf);
    a += n; p += n; len -= n;
  }
  CACHE_CleanAll(CACHE);
  return memcmp((const void *)addr, data, n0) == 0;
}

#endif
