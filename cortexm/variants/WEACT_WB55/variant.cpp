/*
 * variant.cpp - WeAct STM32WB55CGU6
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


/* HSE 32 MHz -> PLL -> 64 MHz (CPU1, HCLK4 = 64 MHz). */
void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  RCC->CR |= RCC_CR_HSEON;
  if (!cm_wait(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY)) return;
  FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_ICEN | FLASH_ACR_DCEN
             | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;
  cm_wait(&FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_3WS);
  /* Codificación WB (system_stm32wbxx.c): M = campo+1, R = campo+1.
   * VCO = 32/2 * 8 = 128 MHz ; SYSCLK = /2 = 64 MHz */
  RCC->PLLCFGR = (3UL << RCC_PLLCFGR_PLLSRC_Pos)            /* 11 = HSE */
               | (1UL << RCC_PLLCFGR_PLLM_Pos)              /* /2 */
               | (8UL << RCC_PLLCFGR_PLLN_Pos)
               | (1UL << RCC_PLLCFGR_PLLR_Pos)              /* /2 */
               | RCC_PLLCFGR_PLLREN;
  RCC->CR |= RCC_CR_PLLON;
  if (!cm_wait(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) return;
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2))
            | (3UL << RCC_CFGR_SW_Pos);                     /* 11 = PLL */
  cm_wait(&RCC->CFGR, RCC_CFGR_SWS, 3UL << RCC_CFGR_SWS_Pos);
#else
  FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;  /* MSI 4 MHz */
#endif
}

}
