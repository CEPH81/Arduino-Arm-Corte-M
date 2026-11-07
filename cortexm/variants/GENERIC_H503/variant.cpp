/*
 * variant.cpp - STM32H503 genérica
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


/* HSI 64 MHz (/2 de fábrica = 32 MHz) -> PLL1 -> 250 MHz en VOS0.
 * Codificación H5 (system_stm32h5xx.c): M = campo, N = campo+1, P = campo+1. */
void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  PWR->VOSCR = (PWR->VOSCR & ~PWR_VOSCR_VOS) | (3UL << PWR_VOSCR_VOS_Pos);   /* VOS0 */
  if (!cm_wait(&PWR->VOSSR, PWR_VOSSR_VOSRDY, PWR_VOSSR_VOSRDY)) return;
  FLASH->ACR = (FLASH->ACR & ~(FLASH_ACR_LATENCY | FLASH_ACR_WRHIGHFREQ))
             | FLASH_ACR_LATENCY_5WS | (2UL << FLASH_ACR_WRHIGHFREQ_Pos) | FLASH_ACR_PRFTEN;
  cm_wait(&FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);
  /* ref = 32 MHz / 8 = 4 MHz (rango 4-8 MHz) ; VCO = 4 * 125 = 500 ; P /2 = 250 */
  RCC->PLL1CFGR = (1UL << RCC_PLL1CFGR_PLL1SRC_Pos)          /* 01 = HSI */
                | (2UL << RCC_PLL1CFGR_PLL1RGE_Pos)          /* 4..8 MHz */
                | (8UL << RCC_PLL1CFGR_PLL1M_Pos)
                | RCC_PLL1CFGR_PLL1PEN;                       /* VCOSEL=0 (ancho) */
  RCC->PLL1DIVR = ((125UL - 1UL) << RCC_PLL1DIVR_PLL1N_Pos)
                | ((2UL - 1UL) << RCC_PLL1DIVR_PLL1P_Pos)
                | ((2UL - 1UL) << RCC_PLL1DIVR_PLL1Q_Pos)
                | ((2UL - 1UL) << RCC_PLL1DIVR_PLL1R_Pos);
  RCC->CR |= RCC_CR_PLL1ON;
  if (!cm_wait(&RCC->CR, RCC_CR_PLL1RDY, RCC_CR_PLL1RDY)) return;
  RCC->CFGR2 &= ~(RCC_CFGR2_HPRE | RCC_CFGR2_PPRE1 | RCC_CFGR2_PPRE2 | RCC_CFGR2_PPRE3);
  RCC->CFGR1 = (RCC->CFGR1 & ~RCC_CFGR1_SW) | (3UL << RCC_CFGR1_SW_Pos);  /* PLL1 */
  cm_wait(&RCC->CFGR1, RCC_CFGR1_SWS, 3UL << RCC_CFGR1_SWS_Pos);
#endif
  ICACHE->CR |= ICACHE_CR_EN;                                /* caché de instrucciones */
}

}
