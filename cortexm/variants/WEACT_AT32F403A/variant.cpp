/*
 * variant.cpp - WeAct AT32F403ACGU7
 * Reloj: HEXT 8 MHz / 2 * 60 = 240 MHz (APB1/APB2 120 MHz), como la
 * plantilla oficial de Artery. Sin cristal: HICK/2 * 60 = 240 MHz.
 * En AT32F403A (estilo F1) la función 0 = salida AF push-pull; USART1 en
 * PA9/PA10 no necesita remapeo.
 */
#include "Arduino.h"

CM_SERIAL(Serial,  USART1, USART1_IRQn, USART1_IRQHandler, PA9, 0, PA10, 1)
CM_SERIAL(Serial1, USART2, USART2_IRQn, USART2_IRQHandler, PA2, 0, PA3,  1)

extern "C" void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  crm_reset();
  crm_pll_clock_source_type src = CRM_PLL_SOURCE_HEXT_DIV;
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
  if (crm_hext_stable_wait() == ERROR) {
    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, FALSE);
    src = CRM_PLL_SOURCE_HICK;
  }
  crm_pll_config(src, CRM_PLL_MULT_60, CRM_PLL_OUTPUT_RANGE_GT72MHZ);
  if (src == CRM_PLL_SOURCE_HEXT_DIV) crm_hext_clock_div_set(CRM_HEXT_DIV_2);
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
  while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) { }

  crm_ahb_div_set(CRM_AHB_DIV_1);
  crm_apb2_div_set(CRM_APB2_DIV_2);
  crm_apb1_div_set(CRM_APB1_DIV_2);
  crm_auto_step_mode_enable(TRUE);
  crm_sysclk_switch(CRM_SCLK_PLL);
  while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) { }
  crm_auto_step_mode_enable(FALSE);
#endif
}
