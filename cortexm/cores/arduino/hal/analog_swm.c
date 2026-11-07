/*
 * analog_swm.c - ADC, PWM, temporizador periódico y flash de Synwit.
 *
 * SWM341 (drivers del fabricante SWM341_adc/pwm/flash)
 *   ADC   ADC0 (12 canales) y ADC1 (7), reloj HRC/8.
 *   PWM   PWM0..PWM4, salidas A y B en pines fijos; A y B comparten periodo.
 *   Timer TIMR0 (24 bits + preescala de 8 bits).
 *   Flash sectores de 4 KB por IAP de la ROM; escritura de 4 palabras.
 *
 * SWM181 (registros; no hay drivers fuente de ADC/PWM)
 *   ADC   8 canales en pines fijos, reloj HRC/4.
 *   PWM   PWM0..PWM3 (A y B con periodo propio). La salida va a cualquier
 *         pata con FUNMUX: cada pata nueva toma el siguiente canal libre
 *         (8 como máximo). Reloj del PWM = SystemCoreClock / 8.
 *   Timer TIMR0 (32 bits) por la IRQ5 del multiplexor de interrupciones.
 *   Flash no disponible (la EEPROM emulada no está soportada).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_SWM)

#define NELEM(a) (sizeof(a) / sizeof((a)[0]))

uint32_t cmAdcRefMv(void) { return 3300u; }
int32_t  cmAdcTemperature(void) { return CM_TEMP_NONE; }

#if defined(CHIP_SWM341)
/* =================================================================== */
#include "SWM341_adc.h"
#include "SWM341_pwm.h"
#include "SWM341_flash.h"

extern PORT_TypeDef *const cm_swm_port_tbl[];

/* {pin, ADC (0/1), canal, función del PORT} */
typedef struct { uint8_t pin, adc, ch, fn; } swm_adc_t;
static const swm_adc_t adc_pins[] = {
  {PC6, 0, 0, PORTC_PIN6_ADC0_CH0},   {PC5, 0, 1, PORTC_PIN5_ADC0_CH1},   {PC4, 0, 2, PORTC_PIN4_ADC0_CH2},
  {PC3, 0, 3, PORTC_PIN3_ADC0_CH3},   {PC2, 0, 4, PORTC_PIN2_ADC0_CH4},   {PC1, 0, 5, PORTC_PIN1_ADC0_CH5},
  {PC0, 0, 6, PORTC_PIN0_ADC0_CH6},   {PA15, 0, 7, PORTA_PIN15_ADC0_CH7}, {PA14, 0, 8, PORTA_PIN14_ADC0_CH8},
  {PA13, 0, 9, PORTA_PIN13_ADC0_CH9}, {PA12, 0, 10, PORTA_PIN12_ADC0_CH10}, {PA10, 0, 11, PORTA_PIN10_ADC0_CH11},
  {PD1, 1, 0, PORTD_PIN1_ADC1_CH0},   {PD0, 1, 1, PORTD_PIN0_ADC1_CH1},   {PC13, 1, 2, PORTC_PIN13_ADC1_CH2},
  {PC12, 1, 3, PORTC_PIN12_ADC1_CH3}, {PC11, 1, 4, PORTC_PIN11_ADC1_CH4}, {PC10, 1, 5, PORTC_PIN10_ADC1_CH5},
  {PC9, 1, 6, PORTC_PIN9_ADC1_CH6},
};

/* {pin, PWMn, canal (0 = A, 1 = B), función} (PB12/PB14 son SWD: excluidas) */
typedef struct { uint8_t pin, pwm, ch, fn; } swm_pwm_t;
static const swm_pwm_t pwm_pins[] = {
  {PM1, 0, 0, PORTM_PIN1_PWM0A},  {PM2, 0, 1, PORTM_PIN2_PWM0B},  {PM3, 1, 0, PORTM_PIN3_PWM1A},
  {PD9, 1, 1, PORTD_PIN9_PWM1B},  {PB3, 2, 0, PORTB_PIN3_PWM2A},  {PM12, 2, 0, PORTM_PIN12_PWM2A},
  {PB2, 2, 1, PORTB_PIN2_PWM2B},  {PM11, 2, 1, PORTM_PIN11_PWM2B}, {PC2, 3, 0, PORTC_PIN2_PWM3A},
  {PM10, 3, 0, PORTM_PIN10_PWM3A}, {PN5, 3, 0, PORTN_PIN5_PWM3A}, {PB1, 3, 1, PORTB_PIN1_PWM3B},
  {PB15, 4, 0, PORTB_PIN15_PWM4A}, {PB13, 4, 1, PORTB_PIN13_PWM4B},
};
static PWM_TypeDef *const pwm_regs[] = { PWM0, PWM1, PWM2, PWM3, PWM4 };

static const swm_pwm_t *pwm_find(uint8_t pin)
{
  for (unsigned i = 0; i < NELEM(pwm_pins); i++) if (pwm_pins[i].pin == pin) return &pwm_pins[i];
  return 0;
}

/* ------------------------------------------------------------- ADC */
static uint8_t adc_ready;

int cmAdcRead(uint8_t pin)
{
  const swm_adc_t *e = 0;
  for (unsigned i = 0; i < NELEM(adc_pins); i++) if (adc_pins[i].pin == pin) e = &adc_pins[i];
  if (!e) return CM_ADC_NONE;
  ADC_TypeDef *A = e->adc ? ADC1 : ADC0;
  if (!(adc_ready & (1u << e->adc))) {
    ADC_InitStructure a;
    a.clk_src = ADC_CLKSRC_HRC_DIV8;
    a.samplAvg = ADC_AVG_SAMPLE1;
    a.EOC_IEn = 0;
    a.HalfIEn = 0;
    ADC_Init(A, &a);
    ADC_SEQ_InitStructure q;
    q.channels = 0;
    q.trig_src = ADC_TRIGGER_SW;
    q.conv_cnt = 1;
    q.samp_tim = ADC_SAMPLE_8CLOCK;
    ADC_SEQ_Init(A, ADC_SEQ0, &q);
    ADC_Open(A);
    ADC_Calibrate(A);
    adc_ready |= (uint8_t)(1u << e->adc);
  }
  PORT_Init(cm_swm_port_tbl[cmPinPortIndex(pin)], cmPinBit(pin), e->fn, 0);
  uint32_t m = 1UL << e->ch;
  A->SEQCHN0 = (A->SEQCHN0 & ~ADC_SEQCHN0_SEQ0_Msk) | m;
  while (ADC_DataAvailable(A, ADC_SEQ0)) { uint32_t c; (void)ADC_Read(A, ADC_SEQ0, &c); }
  ADC_Start(A, ADC_SEQ0);
  uint32_t t0 = micros(), v = 0, c = 0xFF;
  bool ok = false;
  while (micros() - t0 < 2000u) {
    if (ADC_DataAvailable(A, ADC_SEQ0)) {
      v = ADC_Read(A, ADC_SEQ0, &c);
      if (c == e->ch) { ok = true; break; }
    }
  }
  return ok ? (int)(v & 0xFFFu) : CM_ADC_NONE;
}

/* ------------------------------------------------------------- PWM */
static uint32_t pwm_freq[5];
static uint8_t  pwm_on[5];

bool cmPwmPin(uint8_t pin) { return pwm_find(pin) != 0; }

bool cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits)
{
  const swm_pwm_t *e = pwm_find(pin);
  if (!e || !freq) return false;
  PWM_TypeDef *P = pwm_regs[e->pwm];
  if (pwm_freq[e->pwm] != freq) {
    uint32_t counts = SystemCoreClock / freq;
    uint32_t div = (counts + 0xFFFFu) / 0x10000u;
    if (div < 1u) div = 1u;
    if (div > 1024u) div = 1024u;
    uint32_t per = counts / div;
    if (per < 2u) per = 2u;
    if (per > 0xFFFFu) per = 0xFFFFu;
    uint16_t ha = (uint16_t)P->CMPA, hb = (uint16_t)P->CMPB;
    PWM_InitStructure c;
    memset(&c, 0, sizeof c);
    c.Mode = PWM_EDGE_ALIGNED;
    c.Clkdiv = (uint16_t)div;
    c.Period = (uint16_t)per;
    c.HdutyA = pwm_freq[e->pwm] ? ha : 0;
    c.HdutyB = pwm_freq[e->pwm] ? hb : 0;
    PWM_Stop(1u << e->pwm);
    PWM_Init(P, &c);
    PWM_Start(1u << e->pwm);
    pwm_freq[e->pwm] = freq;
  }
  uint32_t top = P->PERIOD + 1u;
  uint32_t h = (uint32_t)(((uint64_t)value * top) >> bits);
  if (h > top) h = top;
  PWM_SetHDuty(P, e->ch, (uint16_t)h);
  if (!(pwm_on[e->pwm] & (1u << e->ch))) {
    cmPinFunction(pin, e->fn, false, 0);
    pwm_on[e->pwm] |= (uint8_t)(1u << e->ch);
  }
  return true;
}

void cmPwmStop(uint8_t pin)
{
  const swm_pwm_t *e = pwm_find(pin);
  if (!e) return;
  pwm_on[e->pwm] &= (uint8_t)~(1u << e->ch);
  if (!pwm_on[e->pwm]) { PWM_Stop(1u << e->pwm); pwm_freq[e->pwm] = 0; }
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* ------------------------------------------------------- Timer TIMR0 */
static void (*volatile tmr_cb)(void);

bool cmTimerStart(uint32_t us, void (*cb)(void))
{
  if (!cb || !us) return false;
  uint64_t ticks = ((uint64_t)SystemCoreClock * us) / 1000000u;
  uint32_t pre = 1;
  while (pre < 256u && ticks / pre > 0xFFFFFFu) pre++;
  uint32_t load = (uint32_t)(ticks / pre);
  if (load < 2u) load = 2u;
  if (load > 0xFFFFFFu) load = 0xFFFFFFu;
  SYS->CLKEN0 |= (1UL << SYS_CLKEN0_TIMR_Pos);
  TIMRG->EN &= ~(1UL << TIMRG_EN_TIMR0_Pos);
  TIMR0->CR &= ~(TIMR_CR_MODE_Msk | TIMR_CR_CLKSRC_Msk);      /* temporizador, reloj del sistema */
  TIMR0->PREDIV = pre - 1u;
  TIMR0->LOAD = load - 1u;
  TIMR0->IF = 1UL << TIMR_IF_TO_Pos;
  TIMR0->IE |= 1UL << TIMR_IE_TO_Pos;
  tmr_cb = cb;
  NVIC_SetPriority(TIMR0_IRQn, 2);
  NVIC_EnableIRQ(TIMR0_IRQn);
  TIMRG->EN |= 1UL << TIMRG_EN_TIMR0_Pos;
  return true;
}

void cmTimerStop(void)
{
  TIMRG->EN &= ~(1UL << TIMRG_EN_TIMR0_Pos);
  TIMR0->IE &= ~(1UL << TIMR_IE_TO_Pos);
  NVIC_DisableIRQ(TIMR0_IRQn);
  tmr_cb = 0;
}

void TIMR0_Handler(void)
{
  TIMR0->IF = 1UL << TIMR_IF_TO_Pos;
  void (*cb)(void) = tmr_cb;
  if (cb) cb();
}

/* ------------------------------------------------------------ Flash */
bool cmFlashErase(uint32_t addr)
{
  return FLASH_Erase(addr & ~0xFFFu) == FLASH_RES_OK;
}

bool cmFlashWrite(uint32_t addr, const void *data, uint32_t len)
{
  uint32_t buf[4];
  for (uint32_t i = 0; i + 16u <= len; i += 16u) {
    memcpy(buf, (const uint8_t *)data + i, 16);
    if (FLASH_Write(addr + i, buf, 4) != FLASH_RES_OK) return false;
  }
  return memcmp((const void *)addr, data, len & ~15u) == 0;
}

#else /* CHIP_SWM181 */
/* =================================================================== */
extern PORT_TypeDef *const cm_swm_port_tbl[];

/* Canales del ADC: {pin, canal} (función 2 en el PORT) */
static const uint8_t adc_pins[][2] = {
  {PE4, 0}, {PA15, 1}, {PA14, 2}, {PA13, 3}, {PA12, 4}, {PC7, 5}, {PC6, 6},
};
static bool adc_ready;

int cmAdcRead(uint8_t pin)
{
  int ch = -1;
  for (unsigned i = 0; i < NELEM(adc_pins); i++) if (adc_pins[i][0] == pin) ch = adc_pins[i][1];
  if (ch < 0 || !cmPinValid(pin)) return CM_ADC_NONE;
  if (!adc_ready) {
    SYS->CLKEN |= SYS_CLKEN_ADC_Msk;
    SYS->CLKSEL &= ~SYS_CLKSEL_ADC_Msk;          /* HRC / 4 */
    ADC->CTRL = ADC_CTRL_EN_Msk;                  /* conversión única, disparo por software */
    adc_ready = true;
  }
  PORT_Init(cm_swm_port_tbl[cmPinPortIndex(pin)], cmPinBit(pin), 2, 0);
  ADC->CTRL = (ADC->CTRL & ~(0xFFUL | ADC_CTRL_CONT_Msk | ADC_CTRL_TRIG_Msk | ADC_CTRL_RES2FF_Msk))
              | ADC_CTRL_EN_Msk | (1UL << ch);
  ADC->CH[ch].STAT = ADC_STAT_EOC_Msk;
  (void)ADC->CH[ch].DATA;
  ADC->START = ADC_START_GO_Msk;
  uint32_t t0 = micros();
  while (!(ADC->CH[ch].STAT & ADC_STAT_EOC_Msk)) if (micros() - t0 > 2000u) return CM_ADC_NONE;
  int v = (int)(ADC->CH[ch].DATA & 0xFFFu);
  ADC->CH[ch].STAT = ADC_STAT_EOC_Msk;
  return v;
}

/* PWM: 8 salidas (PWM0A, PWM0B, ... PWM3B) asignadas a las patas FUNMUX */
#define PWM_CLKDIV_CODE  3u                 /* /8 */
#define PWM_CLK()        (SystemCoreClock >> PWM_CLKDIV_CODE)
static PWM_TypeDef *const pwm_regs[] = { PWM0, PWM1, PWM2, PWM3 };
static const uint8_t pwm_fn[8] = {
  FUNMUX_PWM0A_OUT, FUNMUX_PWM0B_OUT, FUNMUX_PWM1A_OUT, FUNMUX_PWM1B_OUT,
  FUNMUX_PWM2A_OUT, FUNMUX_PWM2B_OUT, FUNMUX_PWM3A_OUT, FUNMUX_PWM3B_OUT,
};
static uint8_t  pwm_slot_pin[8] = { CM_PIN_NONE, CM_PIN_NONE, CM_PIN_NONE, CM_PIN_NONE,
                                    CM_PIN_NONE, CM_PIN_NONE, CM_PIN_NONE, CM_PIN_NONE };
static uint32_t pwm_freq[8];

static bool funmux_pin(uint8_t pin)
{
  uint8_t port = (uint8_t)cmPinPortIndex(pin), bit = (uint8_t)cmPinBit(pin);
  if (!cmPinValid(pin)) return false;
  if (port <= 1) return true;                       /* PA0-15, PB0-15 */
  if (port == 2) return bit >= 1 && bit <= 7;       /* PC1-7 */
  return bit <= 7;                                  /* PD0-7, PE0-7 */
}

static int pwm_slot(uint8_t pin, bool alloc)
{
  for (int i = 0; i < 8; i++) if (pwm_slot_pin[i] == pin) return i;
  if (!alloc) return -1;
  for (int i = 0; i < 8; i++) if (pwm_slot_pin[i] == CM_PIN_NONE) { pwm_slot_pin[i] = pin; return i; }
  return -1;
}

bool cmPwmPin(uint8_t pin) { return funmux_pin(pin); }

bool cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits)
{
  if (!funmux_pin(pin) || !freq) return false;
  int s = pwm_slot(pin, true);
  if (s < 0) return false;
  PWM_TypeDef *P = pwm_regs[s >> 1];
  bool b = s & 1;
  SYS->CLKEN |= SYS_CLKEN_PWM_Msk;
  PWMG->CLKDIV = PWM_CLKDIV_CODE;
  uint32_t per = PWM_CLK() / freq;
  if (per < 2u) per = 2u;
  if (per > 0xFFFFu) per = 0xFFFFu;
  uint32_t h = (uint32_t)(((uint64_t)value * per) >> bits);
  if (h > per) h = per;
  uint32_t en = 1UL << s;                           /* PWMG_CHEN: PWM0A = bit 0, PWM0B = 1, ... */
  if (!(PWMG->CHEN & en) || pwm_freq[s] != freq) {
    PWMG->CHEN &= ~en;
    P->MODE = 0;                                    /* A y B independientes */
    if (b) { P->PERB = per; P->HIGHB = h; P->DZB = 0; P->INIOUT &= ~PWM_INIOUT_PWMB_Msk; }
    else   { P->PERA = per; P->HIGHA = h; P->DZA = 0; P->INIOUT &= ~PWM_INIOUT_PWMA_Msk; }
    PORT_Init(cm_swm_port_tbl[cmPinPortIndex(pin)], cmPinBit(pin), pwm_fn[s], 0);
    PWMG->CHEN |= en;
    pwm_freq[s] = freq;
  } else {
    if (b) P->HIGHB = h; else P->HIGHA = h;
  }
  return true;
}

void cmPwmStop(uint8_t pin)
{
  int s = pwm_slot(pin, false);
  if (s < 0) return;
  PWMG->CHEN &= ~(1UL << s);
  pwm_slot_pin[s] = CM_PIN_NONE;
  pwm_freq[s] = 0;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* Timer: TIMR0 cuenta hacia abajo desde LDVAL; IRQ5 del multiplexor */
static void (*volatile tmr_cb)(void);

bool cmTimerStart(uint32_t us, void (*cb)(void))
{
  if (!cb || !us) return false;
  uint64_t t = ((uint64_t)SystemCoreClock * us) / 1000000u;
  if (t < 2u) t = 2u;
  if (t > 0xFFFFFFFFu) t = 0xFFFFFFFFu;
  SYS->CLKEN |= SYS_CLKEN_TIMR_Msk;
  TIMR0->CTRL &= ~(TIMR_CTRL_EN_Msk | TIMR_CTRL_CLKSRC_Msk);
  TIMR0->LDVAL = (uint32_t)t - 1u;
  TIMRG->IF = TIMRG_IF_TIMR0_Msk;
  TIMRG->IE |= TIMRG_IE_TIMR0_Msk;
  tmr_cb = cb;
  IRQ_Connect(IRQ0_15_TIMR0, IRQ5_IRQ, 2);
  TIMR0->CTRL |= TIMR_CTRL_EN_Msk;
  return true;
}

void cmTimerStop(void)
{
  TIMR0->CTRL &= ~TIMR_CTRL_EN_Msk;
  TIMRG->IE &= ~TIMRG_IE_TIMR0_Msk;
  NVIC_DisableIRQ(IRQ5_IRQ);
  tmr_cb = 0;
}

void IRQ5_Handler(void)
{
  TIMRG->IF = TIMRG_IF_TIMR0_Msk;
  void (*cb)(void) = tmr_cb;
  if (cb) cb();
}

bool cmFlashErase(uint32_t addr) { (void)addr; return false; }
bool cmFlashWrite(uint32_t addr, const void *d, uint32_t n) { (void)addr; (void)d; (void)n; return false; }

#endif
#endif
