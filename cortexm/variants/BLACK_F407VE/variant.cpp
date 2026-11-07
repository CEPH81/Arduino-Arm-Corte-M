/*
 * variant.cpp - STM32F407VET6 "Black" (F4VE)
 * Reloj del sistema y puertos serie de la placa.
 */
#include "Arduino.h"

CM_SERIAL(Serial, USART1, USART1_IRQn, USART1_IRQHandler, PA9, 7, PA10, 7)

extern "C" {

/* Espera (con límite) a que (*reg & mask) == val. Devuelve false si vence. */
static bool cm_wait(volatile uint32_t *reg, uint32_t mask, uint32_t val)
{
  for (uint32_t n = 0; n < 2000000UL; n++) {
    if ((*reg & mask) == val) return true;
  }
  return false;
}


/* HSE 8 MHz -> PLL -> 168 MHz (AHB 168, APB1 42, APB2 84). Si el cristal no
 * arranca se usa HSI 16 MHz como fuente del PLL con el mismo resultado. */
void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  uint32_t src = RCC_PLLCFGR_PLLSRC_HSE, m = HSE_VALUE / 1000000UL;
  RCC->CR |= RCC_CR_HSEON;
  if (!cm_wait(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY)) {
    RCC->CR &= ~RCC_CR_HSEON;
    src = RCC_PLLCFGR_PLLSRC_HSI; m = 16;
  }
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; (void)RCC->APB1ENR;
  PWR->CR |= PWR_CR_VOS;                                   /* escala 1 */
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2))
            | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
  /* VCO = 1 MHz * 336 = 336 MHz ; SYSCLK = /2 = 168 ; USB = /7 = 48 */
  RCC->PLLCFGR = (m << RCC_PLLCFGR_PLLM_Pos) | (336UL << RCC_PLLCFGR_PLLN_Pos)
               | (0UL << RCC_PLLCFGR_PLLP_Pos) | src | (7UL << RCC_PLLCFGR_PLLQ_Pos);
  RCC->CR |= RCC_CR_PLLON;
  if (!cm_wait(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) return;
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
  cm_wait(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL);
#else
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;   /* HSI 16 MHz */
#endif
}

}
