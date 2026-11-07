/*
 * variant.cpp - WeAct STM32G431CBU6
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


/* HSE 8 MHz -> PLL -> 170 MHz (rango 1 "boost"). APB1 = APB2 = 170 MHz. */
void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  uint32_t src = RCC_PLLCFGR_PLLSRC_HSE, m = HSE_VALUE / 4000000UL;   /* ref 4 MHz */
  RCC->CR |= RCC_CR_HSEON;
  if (!cm_wait(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY)) {
    RCC->CR &= ~RCC_CR_HSEON;
    src = RCC_PLLCFGR_PLLSRC_HSI; m = 4;                               /* HSI 16/4 */
  }
  RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN; (void)RCC->APB1ENR1;
  PWR->CR5 &= ~PWR_CR5_R1MODE;                                         /* boost */
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_4WS;
  cm_wait(&FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
  /* VCO = 4 MHz * 85 = 340 MHz ; SYSCLK = /2 = 170 MHz */
  RCC->PLLCFGR = src | ((m - 1UL) << RCC_PLLCFGR_PLLM_Pos) | (85UL << RCC_PLLCFGR_PLLN_Pos)
               | (0UL << RCC_PLLCFGR_PLLR_Pos) | RCC_PLLCFGR_PLLREN;
  RCC->CR |= RCC_CR_PLLON;
  if (!cm_wait(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY)) return;
  /* Transición: AHB /2 durante el cambio a >80 MHz (manual de referencia) */
  RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2))
            | RCC_CFGR_HPRE_DIV2 | RCC_CFGR_SW_PLL;
  cm_wait(&RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL);
  for (volatile uint32_t i = 0; i < 200; i++) { }                      /* >1 us */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV1;
#else
  FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;   /* HSI 16 MHz */
#endif
}

}
