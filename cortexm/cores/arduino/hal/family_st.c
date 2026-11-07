/*
 * family_st.c - Relojes de GPIO y periféricos para STM32 F4/L4/G4/WB/H5.
 */
#include "Arduino.h"

#if defined(CM_VENDOR_ST)

/* Separación de 0x400 entre puertos: la numeración puerto*16+bit es válida */
_Static_assert(GPIOB_BASE - GPIOA_BASE == CM_GPIO_STRIDE, "separacion GPIO inesperada");
_Static_assert(GPIOC_BASE - GPIOA_BASE == 2 * CM_GPIO_STRIDE, "separacion GPIO inesperada");
#if defined(GPIOH_BASE)
_Static_assert(GPIOH_BASE - GPIOA_BASE == 7 * CM_GPIO_STRIDE, "separacion GPIO inesperada");
#endif

#if defined(STM32F4xx)
  #define GPIO_EN_REG   (RCC->AHB1ENR)
#else
  #define GPIO_EN_REG   (RCC->AHB2ENR)
#endif

static inline void set_and_sync(volatile uint32_t *reg, uint32_t bits)
{
  if ((*reg & bits) == bits) return;
  uint32_t m = cmIrqSave();
  *reg |= bits;
  cmIrqRestore(m);
  (void)*reg;                      /* retardo tras habilitar el reloj */
  __DSB();
}

void cmGpioClockEnable(uint32_t portIndex)
{
  set_and_sync(&GPIO_EN_REG, 1UL << portIndex);   /* GPIOxEN = bit del índice */
}

void cmPeriphClockEnable(uintptr_t base)
{
  switch (base) {
    case USART1_BASE: set_and_sync(&RCC->APB2ENR, RCC_APB2ENR_USART1EN); break;
#if defined(USART2_BASE)
    case USART2_BASE:
  #if defined(RCC_APB1ENR_USART2EN)
      set_and_sync(&RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  #elif defined(RCC_APB1ENR1_USART2EN)
      set_and_sync(&RCC->APB1ENR1, RCC_APB1ENR1_USART2EN);
  #elif defined(RCC_APB1LENR_USART2EN)
      set_and_sync(&RCC->APB1LENR, RCC_APB1LENR_USART2EN);
  #endif
      break;
#endif
#if defined(USART3_BASE)
    case USART3_BASE:
  #if defined(RCC_APB1ENR_USART3EN)
      set_and_sync(&RCC->APB1ENR, RCC_APB1ENR_USART3EN);
  #elif defined(RCC_APB1ENR1_USART3EN)
      set_and_sync(&RCC->APB1ENR1, RCC_APB1ENR1_USART3EN);
  #elif defined(RCC_APB1LENR_USART3EN)
      set_and_sync(&RCC->APB1LENR, RCC_APB1LENR_USART3EN);
  #endif
      break;
#endif
#if defined(SYSCFG_BASE) && defined(RCC_APB2ENR_SYSCFGEN)
    case SYSCFG_BASE: set_and_sync(&RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); break;
#endif
    default: break;
  }
}

/* Divisor APB: 0xx = /1, 100 = /2, 101 = /4, 110 = /8, 111 = /16 */
static uint32_t apb_div(uint32_t code)
{
  return (code & 4u) ? (SystemCoreClock >> ((code & 3u) + 1u)) : SystemCoreClock;
}

uint32_t cmPeriphClock(uintptr_t base)
{
  bool apb2 = (base >= APB2PERIPH_BASE && base < APB2PERIPH_BASE + 0x10000UL);
#if defined(STM32H5xx)
  uint32_t r = RCC->CFGR2;
  return apb_div(apb2 ? (r & RCC_CFGR2_PPRE2) >> RCC_CFGR2_PPRE2_Pos
                      : (r & RCC_CFGR2_PPRE1) >> RCC_CFGR2_PPRE1_Pos);
#else
  uint32_t r = RCC->CFGR;
  return apb_div(apb2 ? (r & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos
                      : (r & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos);
#endif
}

#endif
