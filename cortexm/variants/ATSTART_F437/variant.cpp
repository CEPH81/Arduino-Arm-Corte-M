/*
 * variant.cpp - AT-START-F437
 * Reloj: HEXT 8 MHz -> PLL 288 MHz (AHB 288, APB1/APB2 144 MHz), igual que
 * la plantilla oficial de Artery. Sin cristal se usa HICK como fuente del PLL.
 */
#include "Arduino.h"

CM_SERIAL(Serial,  USART1, USART1_IRQn, USART1_IRQHandler, PA9, 7, PA10, 7)
CM_SERIAL(Serial1, USART2, USART2_IRQn, USART2_IRQHandler, PA2, 7, PA3,  7)

extern "C" void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  crm_reset();
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
  pwc_ldo_output_voltage_set(PWC_LDO_OUTPUT_1V3);
  flash_clock_divider_set(FLASH_CLOCK_DIV_3);

  crm_pll_clock_source_type src = CRM_PLL_SOURCE_HEXT;
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
  if (crm_hext_stable_wait() == ERROR) {
    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, FALSE);
    src = CRM_PLL_SOURCE_HICK;
  }
  /* 8 MHz * 144 / (1 * 4) = 288 MHz */
  crm_pll_config(src, 144, 1, CRM_PLL_FR_4);
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
