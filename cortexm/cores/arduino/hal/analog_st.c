/*
 * analog_st.c - ADC, PWM y temporizador periódico para las familias con
 * periféricos "estilo ST": STM32 F4/L4/G4/WB/H5, GD32F4xx, AT32F43x y
 * AT32F403A.
 *
 *  Temporizadores: el mapa de registros es idéntico en las cuatro marcas
 *    (CR1..BDTR); solo cambian los nombres del reloj. Se usan por número
 *    (TIM1 = TIMER0 de GD = TMR1 de Artery).
 *  ADC: tres variantes
 *    v1  SR/CR1/CR2/SQR3/DR    STM32F4, GD32F4 (registros propios)
 *    v2  ISR/CR/CFGR/SQR1/DR   STM32 L4/G4/WB/H5 (header CMSIS)
 *    at  driver oficial de Artery (AT32F43x y AT32F403A)
 *  Pines: tablas generadas desde stm32duino (hal/pins_st.h).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_ST) || defined(CM_VENDOR_GD) || defined(CM_VENDOR_AT)

typedef struct { uint8_t pin, adc; } cm_adc_pin_t;            /* adc = (unidad << 5) | canal */
typedef struct { uint8_t pin, tim, ch, af; } cm_pwm_pin_t;

#if defined(STM32F407xx) || defined(GD32F4XX) || defined(AT32F435_437)
  #define CM_PINS_F407
#elif defined(STM32L432xx)
  #define CM_PINS_L432
#elif defined(STM32G431xx)
  #define CM_PINS_G431
#elif defined(STM32WB55xx)
  #define CM_PINS_WB55
#elif defined(STM32H503xx)
  #define CM_PINS_H503
#endif

#if defined(AT32F403A_407)
/* AT32F403A: mapa por defecto de los temporizadores (igual que STM32F103) */
static const cm_adc_pin_t cm_adc_pins[] = {
  {PA0, (1 << 5) | 0}, {PA1, (1 << 5) | 1}, {PA2, (1 << 5) | 2}, {PA3, (1 << 5) | 3},
  {PA4, (1 << 5) | 4}, {PA5, (1 << 5) | 5}, {PA6, (1 << 5) | 6}, {PA7, (1 << 5) | 7},
  {PB0, (1 << 5) | 8}, {PB1, (1 << 5) | 9},
  {PC0, (1 << 5) | 10}, {PC1, (1 << 5) | 11}, {PC2, (1 << 5) | 12}, {PC3, (1 << 5) | 13},
  {PC4, (1 << 5) | 14}, {PC5, (1 << 5) | 15},
};
static const cm_pwm_pin_t cm_pwm_pins[] = {
  {PA0, 2, 1, 0}, {PA1, 2, 2, 0}, {PA2, 2, 3, 0}, {PA3, 2, 4, 0},
  {PA6, 3, 1, 0}, {PA7, 3, 2, 0}, {PA8, 1, 1, 0}, {PA9, 1, 2, 0},
  {PA10, 1, 3, 0}, {PA11, 1, 4, 0}, {PB0, 3, 3, 0}, {PB1, 3, 4, 0},
  {PB6, 4, 1, 0}, {PB7, 4, 2, 0}, {PB8, 4, 3, 0}, {PB9, 4, 4, 0},
  {PC6, 8, 1, 0}, {PC7, 8, 2, 0}, {PC8, 8, 3, 0}, {PC9, 8, 4, 0},
};
#else
#include "pins_st.h"
#endif

#define NELEM(a) (sizeof(a) / sizeof((a)[0]))

static const cm_adc_pin_t *adc_find(uint8_t pin)
{
  for (unsigned i = 0; i < NELEM(cm_adc_pins); i++) if (cm_adc_pins[i].pin == pin) return &cm_adc_pins[i];
  return 0;
}

static const cm_pwm_pin_t *pwm_find(uint8_t pin)
{
  for (unsigned i = 0; i < NELEM(cm_pwm_pins); i++) {
    const cm_pwm_pin_t *e = &cm_pwm_pins[i];
    if (e->pin == pin && cmPinValid(pin)) return e;
  }
  return 0;
}

/* ======================================================================
 * Temporizadores
 * ==================================================================== */
typedef struct {
  volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR[2], CCER, CNT, PSC, ARR, RCR, CCR[4], BDTR;
} cm_tim_t;

#define TIM_CR1_CEN_   (1u << 0)
#define TIM_CR1_ARPE_  (1u << 7)
#define TIM_EGR_UG_    (1u << 0)
#define TIM_DIER_UIE_  (1u << 0)
#define TIM_SR_UIF_    (1u << 0)
#define TIM_BDTR_MOE_  (1u << 15)

/* Base de TIMn (0 si no existe) */
static uintptr_t tim_base(uint8_t n)
{
  switch (n) {
#if defined(CM_VENDOR_ST)
  #if defined(TIM1_BASE)
    case 1:  return TIM1_BASE;
  #endif
  #if defined(TIM2_BASE)
    case 2:  return TIM2_BASE;
  #endif
  #if defined(TIM3_BASE)
    case 3:  return TIM3_BASE;
  #endif
  #if defined(TIM4_BASE)
    case 4:  return TIM4_BASE;
  #endif
  #if defined(TIM5_BASE)
    case 5:  return TIM5_BASE;
  #endif
  #if defined(TIM6_BASE)
    case 6:  return TIM6_BASE;
  #endif
  #if defined(TIM7_BASE)
    case 7:  return TIM7_BASE;
  #endif
  #if defined(TIM8_BASE)
    case 8:  return TIM8_BASE;
  #endif
  #if defined(TIM9_BASE)
    case 9:  return TIM9_BASE;
  #endif
  #if defined(TIM10_BASE)
    case 10: return TIM10_BASE;
  #endif
  #if defined(TIM11_BASE)
    case 11: return TIM11_BASE;
  #endif
  #if defined(TIM12_BASE)
    case 12: return TIM12_BASE;
  #endif
  #if defined(TIM13_BASE)
    case 13: return TIM13_BASE;
  #endif
  #if defined(TIM14_BASE)
    case 14: return TIM14_BASE;
  #endif
  #if defined(TIM15_BASE)
    case 15: return TIM15_BASE;
  #endif
  #if defined(TIM16_BASE)
    case 16: return TIM16_BASE;
  #endif
  #if defined(TIM17_BASE)
    case 17: return TIM17_BASE;
  #endif
#elif defined(CM_VENDOR_GD)
    /* GD32F4: mismas direcciones que STM32F4 (TIMERx = TIM(x+1)) */
    case 1:  return 0x40010000UL; case 2:  return 0x40000000UL; case 3:  return 0x40000400UL;
    case 4:  return 0x40000800UL; case 5:  return 0x40000C00UL; case 6:  return 0x40001000UL;
    case 7:  return 0x40001400UL; case 8:  return 0x40010400UL; case 9:  return 0x40014000UL;
    case 10: return 0x40014400UL; case 11: return 0x40014800UL; case 12: return 0x40001800UL;
    case 13: return 0x40001C00UL; case 14: return 0x40002000UL;
#else /* Artery */
    case 1:  return TMR1_BASE;  case 2:  return TMR2_BASE;  case 3:  return TMR3_BASE;
    case 4:  return TMR4_BASE;  case 5:  return TMR5_BASE;  case 6:  return TMR6_BASE;
    case 7:  return TMR7_BASE;  case 8:  return TMR8_BASE;  case 9:  return TMR9_BASE;
    case 10: return TMR10_BASE; case 11: return TMR11_BASE; case 12: return TMR12_BASE;
    case 13: return TMR13_BASE; case 14: return TMR14_BASE;
#endif
    default: return 0;
  }
}

static inline void en_bit(volatile uint32_t *reg, uint32_t bit)
{
  if (*reg & bit) return;
  uint32_t m = cmIrqSave();
  *reg |= bit;
  cmIrqRestore(m);
  (void)*reg;
  __DSB();
}

/* Reloj del temporizador n */
static void tim_clock_on(uint8_t n)
{
#if defined(CM_VENDOR_AT)
  static const crm_periph_clock_type clk[] = {
    (crm_periph_clock_type)0, CRM_TMR1_PERIPH_CLOCK, CRM_TMR2_PERIPH_CLOCK, CRM_TMR3_PERIPH_CLOCK,
    CRM_TMR4_PERIPH_CLOCK, CRM_TMR5_PERIPH_CLOCK, CRM_TMR6_PERIPH_CLOCK, CRM_TMR7_PERIPH_CLOCK,
    CRM_TMR8_PERIPH_CLOCK, CRM_TMR9_PERIPH_CLOCK, CRM_TMR10_PERIPH_CLOCK, CRM_TMR11_PERIPH_CLOCK,
    CRM_TMR12_PERIPH_CLOCK, CRM_TMR13_PERIPH_CLOCK, CRM_TMR14_PERIPH_CLOCK,
  };
  if (n >= 1 && n < NELEM(clk)) {
    uint32_t m = cmIrqSave();
    crm_periph_clock_enable(clk[n], TRUE);
    cmIrqRestore(m);
  }
#elif defined(CM_VENDOR_GD) || defined(STM32F4xx)
  /* RCC/RCU de F4: APB1ENR (0x40) TIM2..7 = bits 0..5, TIM12..14 = 6..8;
   * APB2ENR (0x44) TIM1 = 0, TIM8 = 1, TIM9..11 = 16..18 */
  volatile uint32_t *apb1 = (volatile uint32_t *)(0x40023800UL + 0x40);
  volatile uint32_t *apb2 = (volatile uint32_t *)(0x40023800UL + 0x44);
  if (n >= 2 && n <= 7)        en_bit(apb1, 1UL << (n - 2));
  else if (n >= 12 && n <= 14) en_bit(apb1, 1UL << (n - 6));
  else if (n == 1)             en_bit(apb2, 1UL << 0);
  else if (n == 8)             en_bit(apb2, 1UL << 1);
  else if (n >= 9 && n <= 11)  en_bit(apb2, 1UL << (n + 7));
#else
  /* STM32 L4/G4/WB/H5: nombres del header CMSIS */
  #if defined(RCC_APB1ENR1_TIM2EN)
    #define APB1_REG RCC->APB1ENR1
    #define APB1(n)  RCC_APB1ENR1_TIM##n##EN
  #else
    #define APB1_REG RCC->APB1LENR
    #define APB1(n)  RCC_APB1LENR_TIM##n##EN
  #endif
  switch (n) {
    case 1:  en_bit(&RCC->APB2ENR, RCC_APB2ENR_TIM1EN); break;
    case 2:  en_bit(&APB1_REG, APB1(2)); break;
  #if defined(TIM3_BASE)
    case 3:  en_bit(&APB1_REG, APB1(3)); break;
  #endif
  #if defined(TIM4_BASE)
    case 4:  en_bit(&APB1_REG, APB1(4)); break;
  #endif
  #if defined(TIM6_BASE)
    case 6:  en_bit(&APB1_REG, APB1(6)); break;
  #endif
  #if defined(TIM7_BASE)
    case 7:  en_bit(&APB1_REG, APB1(7)); break;
  #endif
  #if defined(TIM8_BASE)
    case 8:  en_bit(&RCC->APB2ENR, RCC_APB2ENR_TIM8EN); break;
  #endif
  #if defined(TIM15_BASE)
    case 15: en_bit(&RCC->APB2ENR, RCC_APB2ENR_TIM15EN); break;
  #endif
  #if defined(TIM16_BASE)
    case 16: en_bit(&RCC->APB2ENR, RCC_APB2ENR_TIM16EN); break;
  #endif
  #if defined(TIM17_BASE)
    case 17: en_bit(&RCC->APB2ENR, RCC_APB2ENR_TIM17EN); break;
  #endif
    default: break;
  }
#endif
}

/* Reloj de conteo: PCLK, o 2*PCLK si el divisor del APB no es 1 */
static uint32_t tim_clock_hz(uintptr_t base)
{
  uint32_t p = cmPeriphClock(base);
  return (p == SystemCoreClock) ? p : 2u * p;
}

static bool tim_is32(uint8_t n) { return n == 2 || n == 5; }
static bool tim_adv(uint8_t n)  { return n == 1 || n == 8 || n == 15 || n == 16 || n == 17; }

/* Programa PSC/ARR para 'freq' Hz. Devuelve ARR+1 (pasos de un periodo) */
static uint32_t tim_set_freq(cm_tim_t *t, uint8_t n, uint32_t freq)
{
  uint32_t clk = tim_clock_hz((uintptr_t)t);
  uint32_t counts = freq ? clk / freq : 0;
  if (counts < 2u) counts = 2u;
  uint32_t max = tim_is32(n) ? 0xFFFFFFFFu : 0x10000u;
  uint32_t psc = (counts - 1u) / max;                 /* el menor divisor que cabe */
  if (psc > 0xFFFFu) psc = 0xFFFFu;
  uint32_t top = counts / (psc + 1u);
  if (top < 2u) top = 2u;
  if (!tim_is32(n) && top > 0x10000u) top = 0x10000u;
  t->PSC = psc;
  t->ARR = top - 1u;
  return top;
}

/* ---------------------------------------------------------------- PWM */
static uint32_t pwm_freq[18];          /* frecuencia actual de cada TIMn */

bool cmPwmPin(uint8_t pin) { return pwm_find(pin) && tim_base(pwm_find(pin)->tim); }

bool cmPwmWrite(uint8_t pin, uint32_t freq, uint32_t value, uint8_t bits)
{
  const cm_pwm_pin_t *e = pwm_find(pin);
  if (!e) return false;
  cm_tim_t *t = (cm_tim_t *)tim_base(e->tim);
  if (!t) return false;
  uint8_t n = e->tim, ch = (uint8_t)(e->ch - 1u);
  uint32_t ccer = 1UL << (ch * 4u);

  if (pwm_freq[n] != freq || !(t->CR1 & TIM_CR1_CEN_)) {
    tim_clock_on(n);
    t->CR1 = 0;
    tim_set_freq(t, n, freq);
    t->EGR = TIM_EGR_UG_;
    if (tim_adv(n)) t->BDTR |= TIM_BDTR_MOE_;
    t->CR1 = TIM_CR1_ARPE_ | TIM_CR1_CEN_;
    pwm_freq[n] = freq;
  }
  uint32_t top = t->ARR + 1u;
  uint32_t ccr = (uint32_t)(((uint64_t)value * top) >> bits);
  t->CCR[ch] = ccr;
  if (!(t->CCER & ccer)) {
    uint32_t sh = (ch & 1u) * 8u;
    uint32_t m = cmIrqSave();
    t->CCMR[ch >> 1] = (t->CCMR[ch >> 1] & ~(0xFFUL << sh)) | ((0x6UL << 4 | 1UL << 3) << sh);  /* PWM1 + precarga */
    t->CCER |= ccer;
    cmIrqRestore(m);
#if CM_GPIO_STYLE == 2
    cmPinFunction(pin, 0, false, 0);         /* AT32F403A: salida AF push-pull */
#else
    cmPinFunction(pin, e->af, false, 0);
#endif
  }
  return true;
}

void cmPwmStop(uint8_t pin)
{
  const cm_pwm_pin_t *e = pwm_find(pin);
  if (!e) return;
  cm_tim_t *t = (cm_tim_t *)tim_base(e->tim);
  if (!t) return;
  uint32_t m = cmIrqSave();
  t->CCER &= ~(1UL << ((e->ch - 1u) * 4u));
  if ((t->CCER & 0x1111u) == 0) { t->CR1 = 0; pwm_freq[e->tim] = 0; }
  cmIrqRestore(m);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* ---------------------------------------------------- Timer periódico */
#if defined(STM32WB55xx)
  #define CM_TMR_N        17
  #define CM_TMR_IRQn     TIM1_TRG_COM_TIM17_IRQn
  #define CM_TMR_HANDLER  TIM1_TRG_COM_TIM17_IRQHandler
#elif defined(STM32H5xx)
  #define CM_TMR_N        6
  #define CM_TMR_IRQn     TIM6_IRQn
  #define CM_TMR_HANDLER  TIM6_IRQHandler
#elif defined(CM_VENDOR_GD)
  #define CM_TMR_N        6
  #define CM_TMR_IRQn     TIMER5_DAC_IRQn
  #define CM_TMR_HANDLER  TIMER5_DAC_IRQHandler
#elif defined(AT32F435_437)
  #define CM_TMR_N        6
  #define CM_TMR_IRQn     TMR6_DAC_GLOBAL_IRQn
  #define CM_TMR_HANDLER  TMR6_DAC_GLOBAL_IRQHandler
#elif defined(AT32F403A_407)
  #define CM_TMR_N        6
  #define CM_TMR_IRQn     TMR6_GLOBAL_IRQn
  #define CM_TMR_HANDLER  TMR6_GLOBAL_IRQHandler
#else
  #define CM_TMR_N        6
  #define CM_TMR_IRQn     TIM6_DAC_IRQn
  #define CM_TMR_HANDLER  TIM6_DAC_IRQHandler
#endif

static void (*volatile tmr_cb)(void);

bool cmTimerStart(uint32_t us, void (*cb)(void))
{
  cm_tim_t *t = (cm_tim_t *)tim_base(CM_TMR_N);
  if (!t || !cb || !us) return false;
  tim_clock_on(CM_TMR_N);
  t->CR1 = 0;
  t->DIER = 0;
  uint32_t clk = tim_clock_hz((uintptr_t)t);
  uint64_t ticks = ((uint64_t)clk * us) / 1000000u;   /* ciclos del periodo */
  if (ticks < 2u) ticks = 2u;
  uint32_t psc = (uint32_t)((ticks - 1u) / 0x10000u);
  if (psc > 0xFFFFu) psc = 0xFFFFu;
  uint32_t top = (uint32_t)(ticks / (psc + 1u));
  if (top < 1u) top = 1u;
  if (top > 0x10000u) top = 0x10000u;
  t->PSC = psc;
  t->ARR = top - 1u;
  t->EGR = TIM_EGR_UG_;
  t->SR = 0;
  tmr_cb = cb;
  t->DIER = TIM_DIER_UIE_;
  NVIC_SetPriority(CM_TMR_IRQn, 4);
  NVIC_ClearPendingIRQ(CM_TMR_IRQn);
  NVIC_EnableIRQ(CM_TMR_IRQn);
  t->CR1 = TIM_CR1_CEN_;
  return true;
}

void cmTimerStop(void)
{
  cm_tim_t *t = (cm_tim_t *)tim_base(CM_TMR_N);
  if (!t) return;
  t->CR1 = 0;
  t->DIER = 0;
  t->SR = 0;
  NVIC_DisableIRQ(CM_TMR_IRQn);
  tmr_cb = 0;
}

void CM_TMR_HANDLER(void)
{
  cm_tim_t *t = (cm_tim_t *)tim_base(CM_TMR_N);
  if (t->SR & TIM_SR_UIF_) {
    t->SR = ~TIM_SR_UIF_;
    void (*cb)(void) = tmr_cb;
    if (cb) cb();
  }
}

/* ======================================================================
 * ADC
 * ==================================================================== */
uint32_t cmAdcRefMv(void) { return 3300u; }

#if defined(CM_VENDOR_AT)
/* ----------------------------------------- Artery (driver del fabricante) */
static bool adc_ready;

static void adc_init(void)
{
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
#if defined(AT32F435_437)
  adc_common_config_type cc;
  adc_common_default_para_init(&cc);
  cc.combine_mode = ADC_INDEPENDENT_MODE;
  cc.div = ADC_HCLK_DIV_6;                       /* 288 MHz / 6 = 48 MHz (máx. 80) */
  cc.tempervintrv_state = TRUE;
  adc_common_config(&cc);
#else
  crm_adc_clock_div_set(CRM_ADC_DIV_12);         /* PCLK2 120 MHz / 12 = 10 MHz (máx. 28) */
  adc_combine_mode_select(ADC_INDEPENDENT_MODE);
  adc_tempersensor_vintrv_enable(TRUE);
#endif
  adc_base_config_type b;
  adc_base_default_para_init(&b);
  b.sequence_mode = FALSE;
  b.repeat_mode = FALSE;
  b.data_align = ADC_RIGHT_ALIGNMENT;
  b.ordinary_channel_length = 1;
  adc_base_config(ADC1, &b);
#if defined(AT32F435_437)
  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);
  adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);
  adc_enable(ADC1, TRUE);
  while (adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET) { }
#else
  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  adc_enable(ADC1, TRUE);
#endif
  adc_calibration_init(ADC1);
  while (adc_calibration_init_status_get(ADC1)) { }
  adc_calibration_start(ADC1);
  while (adc_calibration_status_get(ADC1)) { }
  adc_ready = true;
}

static int adc_conv(uint8_t ch)
{
  if (!adc_ready) adc_init();
#if defined(AT32F435_437)
  adc_ordinary_channel_set(ADC1, (adc_channel_select_type)ch, 1, ADC_SAMPLETIME_640_5);
#else
  adc_ordinary_channel_set(ADC1, (adc_channel_select_type)ch, 1, ADC_SAMPLETIME_239_5);
#endif
  adc_ordinary_software_trigger_enable(ADC1, TRUE);
  uint32_t t0 = micros();
#if defined(AT32F435_437)
  #define ADC_END_FLAG ADC_OCCE_FLAG
#else
  #define ADC_END_FLAG ADC_CCE_FLAG
#endif
  while (adc_flag_get(ADC1, ADC_END_FLAG) == RESET) if (micros() - t0 > 1000u) return CM_ADC_NONE;
  return (int)(adc_ordinary_conversion_data_get(ADC1) & 0xFFFu);
}

int32_t cmAdcTemperature(void)
{
  /* Valores típicos de los ejemplos de Artery: T = (V25 - V) / pendiente + 25 */
#if defined(AT32F435_437)
  const int32_t v25 = 1270, slope_uv = -4130;     /* mV, µV/°C */
#else
  const int32_t v25 = 1280, slope_uv = -4260;
#endif
  int raw = adc_conv(16);
  if (raw < 0) return CM_TEMP_NONE;
  int32_t mv = raw * 3300 / 4095;
  return (v25 - mv) * 100000 / slope_uv + 2500;
}

#elif defined(STM32F4xx) || defined(CM_VENDOR_GD)
/* ------------------------------------------------- ADC v1 (F4 y GD32F4) */
typedef struct {
  volatile uint32_t SR, CR1, CR2, SMPR1, SMPR2, JOFR[4], HTR, LTR, SQR1, SQR2, SQR3, JSQR, JDR[4], DR;
} cm_adc1_t;
#define ADC1_REGS     ((cm_adc1_t *)0x40012000UL)
#define ADC_CCR_REG   (*(volatile uint32_t *)0x40012304UL)   /* ADC_CCR / ADC_SYNCCTL */
#define ADC_SR_EOC_   (1u << 1)
#define ADC_CR2_ON_   (1u << 0)
#define ADC_CR2_CAL_  (1u << 2)      /* GD32: CLB */
#define ADC_CR2_SW_   (1u << 30)     /* SWSTART / SWRCST */
static bool adc_ready;

static void adc_init(void)
{
  en_bit((volatile uint32_t *)(0x40023800UL + 0x44), 1UL << 8);   /* APB2: ADC1 / ADC0 */
  /* reloj: PCLK2 / 4 (bits 16-17 = 01); bit 23 = sensor de temperatura y VREFINT */
  ADC_CCR_REG = (ADC_CCR_REG & ~(3UL << 16)) | (1UL << 16) | (1UL << 23);
  cm_adc1_t *a = ADC1_REGS;
  a->CR1 = 0;                                     /* 12 bits */
  a->SMPR1 = 0x07FFFFFFu;                         /* 480 ciclos en todos los canales */
  a->SMPR2 = 0x3FFFFFFFu;
  a->SQR1 = 0;
  a->CR2 = ADC_CR2_ON_;
  delayMicroseconds(20);
#if defined(CM_VENDOR_GD)
  a->CR2 |= ADC_CR2_CAL_;
  uint32_t t0 = micros();
  while ((a->CR2 & ADC_CR2_CAL_) && micros() - t0 < 2000u) { }
#endif
  adc_ready = true;
}

static int adc_conv(uint8_t ch)
{
  if (!adc_ready) adc_init();
  cm_adc1_t *a = ADC1_REGS;
  a->SQR3 = ch;
  (void)a->DR;
  a->SR = 0;
  a->CR2 |= ADC_CR2_SW_;
  uint32_t t0 = micros();
  while (!(a->SR & ADC_SR_EOC_)) if (micros() - t0 > 1000u) return CM_ADC_NONE;
  return (int)(a->DR & 0xFFFu);
}

int32_t cmAdcTemperature(void)
{
#if defined(STM32F4xx)
  /* Calibración de fábrica (30 °C y 110 °C a 3.3 V) */
  int32_t c1 = *(const uint16_t *)0x1FFF7A2CUL, c2 = *(const uint16_t *)0x1FFF7A2EUL;
  int32_t ref = *(const uint16_t *)0x1FFF7A2AUL;
  int vr = adc_conv(17), ts = adc_conv(16);
  if (vr <= 0 || ts < 0 || c2 == c1) return CM_TEMP_NONE;
  int32_t tsc = ts * ref / vr;                         /* a la VDDA de la calibración */
  return (tsc - c1) * (110 - 30) * 100 / (c2 - c1) + 3000;
#else
  /* GD32F4: valores típicos (V25 = 1.42 V, 4.35 mV/°C) */
  int ts = adc_conv(16);
  if (ts < 0) return CM_TEMP_NONE;
  int32_t mv = ts * 3300 / 4095;
  return (1420 - mv) * 100000 / 4350 + 2500;
#endif
}

#else
/* --------------------------------------------- ADC v2 (L4, G4, WB, H5) */
#if defined(ADC12_COMMON)
  #define ADC_COMMON_   ADC12_COMMON
#elif defined(ADC1_COMMON)
  #define ADC_COMMON_   ADC1_COMMON
#else
  #define ADC_COMMON_   ADC123_COMMON
#endif

#if defined(STM32H5xx)
  #define TS_CAL1_ADDR  0x08FFF814UL
  #define TS_CAL2_ADDR  0x08FFF818UL
  #define VREF_CAL_ADDR 0x08FFF810UL
  #define CAL_MV        3300
  #define TS_CAL2_C     130
  #define TS_CH         16
  #define VREF_CH       17
#else
  #define TS_CAL1_ADDR  0x1FFF75A8UL
  #define TS_CAL2_ADDR  0x1FFF75CAUL
  #define VREF_CAL_ADDR 0x1FFF75AAUL
  #define CAL_MV        3000
  #if defined(STM32L4xx)
    #define TS_CAL2_C   110
  #else
    #define TS_CAL2_C   130
  #endif
  #if defined(STM32G4xx)
    #define TS_CH       16
    #define VREF_CH     18
  #else
    #define TS_CH       17
    #define VREF_CH     0
  #endif
#endif
#if defined(ADC_CCR_TSEN)
  #define CCR_TS_     ADC_CCR_TSEN
#else
  #define CCR_TS_     ADC_CCR_VSENSESEL
#endif

static uint8_t adc_ready;        /* bit n = ADCn listo */

static ADC_TypeDef *adc_regs(uint8_t unit)
{
#if defined(ADC2)
  if (unit == 2) return ADC2;
#endif
  (void)unit;
  return ADC1;
}

static void adc_init(uint8_t unit)
{
  if (!adc_ready) {
#if defined(RCC_AHB2ENR_ADC12EN)
    en_bit(&RCC->AHB2ENR, RCC_AHB2ENR_ADC12EN);
#else
    en_bit(&RCC->AHB2ENR, RCC_AHB2ENR_ADCEN);
#endif
    /* reloj síncrono HCLK/4; sensor de temperatura y VREFINT encendidos */
    ADC_COMMON_->CCR = (ADC_COMMON_->CCR & ~ADC_CCR_CKMODE) | (3UL << ADC_CCR_CKMODE_Pos) | ADC_CCR_VREFEN | CCR_TS_;
  }
  ADC_TypeDef *a = adc_regs(unit);
  a->CR &= ~ADC_CR_DEEPPWD;
  a->CR |= ADC_CR_ADVREGEN;
  delayMicroseconds(30);
  a->CR &= ~ADC_CR_ADCALDIF;
  a->CR |= ADC_CR_ADCAL;
  uint32_t t0 = micros();
  while ((a->CR & ADC_CR_ADCAL) && micros() - t0 < 5000u) { }
  delayMicroseconds(5);
  a->ISR = ADC_ISR_ADRDY;
  a->CR |= ADC_CR_ADEN;
  t0 = micros();
  while (!(a->ISR & ADC_ISR_ADRDY) && micros() - t0 < 5000u) { }
  a->CFGR = ADC_CFGR_OVRMOD;                      /* 12 bits, una conversión, sobrescribe DR */
  a->SMPR1 = 0x3FFFFFFFu;                         /* 7 = 640.5 ciclos en todos los canales */
#if defined(STM32H5xx)
  a->SMPR2 = 0x3FFFFFFFu;                         /* canales 10..19 */
#else
  a->SMPR2 = 0x07FFFFFFu;                         /* canales 10..18 */
#endif
  adc_ready |= (uint8_t)(1u << unit);
}

static int adc_conv_unit(uint8_t unit, uint8_t ch)
{
  if (!(adc_ready & (1u << unit))) adc_init(unit);
  ADC_TypeDef *a = adc_regs(unit);
  a->SQR1 = (uint32_t)ch << ADC_SQR1_SQ1_Pos;     /* L = 0: un canal */
  a->ISR = ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR;
  a->CR |= ADC_CR_ADSTART;
  uint32_t t0 = micros();
  while (!(a->ISR & ADC_ISR_EOC)) if (micros() - t0 > 1000u) return CM_ADC_NONE;
  return (int)(a->DR & 0xFFFu);
}
#define adc_conv(ch) adc_conv_unit(1, ch)

int32_t cmAdcTemperature(void)
{
  int32_t c1 = *(const uint16_t *)TS_CAL1_ADDR, c2 = *(const uint16_t *)TS_CAL2_ADDR;
  int32_t ref = *(const uint16_t *)VREF_CAL_ADDR;
  int vr = adc_conv(VREF_CH), ts = adc_conv(TS_CH);
  if (vr <= 0 || ts < 0 || c2 == c1) return CM_TEMP_NONE;
  int32_t tsc = ts * ref / vr;
  return (tsc - c1) * (TS_CAL2_C - 30) * 100 / (c2 - c1) + 3000;
}
#endif

int cmAdcRead(uint8_t pin)
{
  const cm_adc_pin_t *e = adc_find(pin);
  if (!e) return CM_ADC_NONE;
  cmPinAnalog(pin);
#if defined(CM_VENDOR_ST) && !defined(STM32F4xx)
  return adc_conv_unit((uint8_t)(e->adc >> 5), (uint8_t)(e->adc & 31u));
#else
  return adc_conv((uint8_t)(e->adc & 31u));
#endif
}

#endif
