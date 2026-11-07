/*
 * variant.cpp - Nucleo-32 L432KC
 * Reloj del sistema y puertos serie de la placa.
 */
#include "Arduino.h"

CM_SERIAL(Serial, USART2, USART2_IRQn, USART2_IRQHandler, PA2, 7, PA15, 3)

extern "C" {

/* Espera (con límite) a que (*reg & mask) == val. Devuelve false si vence. */
static bool cm_wait(volatile uint32_t *reg, uint32_t mask, uint32_t val)
{
  for (uint32_t n = 0; n < 2000000UL; n++) {
    if ((*reg & mask) == val) return true;
  }
  return false;
}


/* MSI 4 MHz -> PLL -> 80 MHz (rango 1). La Nucleo-32 no tiene HSE. */
void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_4WS;
  cm_wait(&FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
  /* VCO = 4 MHz /1 *40 = 160 MHz ; SYSCLK = /2 = 80 MHz */
  RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_MSI | (0UL << RCC_PLLCFGR_PLLM_Pos)
               | (40UL << RCC_PLLCFGR_PLLN_Pos) | (0UL << RCC_PLLCFGR_PLLR_Pos)
               | RCC_PLLCFGR_PLLREN;
  RCC->CR |= RCC_CR_PLLON;
  if (!cm_wait(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) return;
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2)) | RCC_CFGR_SW_PLL;
  cm_wait(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL);
#else
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;   /* MSI 4 MHz */
#endif
}

}
