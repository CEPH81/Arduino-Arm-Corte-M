/*
 * variant.cpp - HC32F460JETA
 * Reloj (secuencia de los ejemplos de HDSC):
 *   XTAL 8 MHz / 1 * 50 / 2 = MPLL 200 MHz -> HCLK 200, PCLK0 200,
 *   PCLK1 100 (USART/SPI/timers), PCLK2/3 50, PCLK4 100, EXCLK 100.
 *   Flash: 5 ciclos de espera; SRAM: 2 ciclos. Sin cristal: HRC 16 MHz / 2.
 */
#include "Arduino.h"

/* Serial = USART1, IRQ 0-2 del NVIC (RX, TX vacío, error) */
CM_SERIAL(Serial, M4_USART1, Int000_IRQn, cm_usart1_isr, PA9, Func_Usart1_Tx, PA10, Func_Usart1_Rx)
extern "C" {
void IRQ000_Handler(void) __attribute__((alias("cm_usart1_isr")));
void IRQ001_Handler(void) __attribute__((alias("cm_usart1_isr")));
void IRQ002_Handler(void) __attribute__((alias("cm_usart1_isr")));
}

extern "C" void SystemClock_Config(void)
{
#if !defined(CM_CLOCK_SAFE)
  stc_clk_sysclk_cfg_t sys;
  sys.enHclkDiv  = ClkSysclkDiv1;
  sys.enExclkDiv = ClkSysclkDiv2;
  sys.enPclk0Div = ClkSysclkDiv1;
  sys.enPclk1Div = ClkSysclkDiv2;
  sys.enPclk2Div = ClkSysclkDiv4;
  sys.enPclk3Div = ClkSysclkDiv4;
  sys.enPclk4Div = ClkSysclkDiv2;
  CLK_SysClkConfig(&sys);

  stc_clk_xtal_cfg_t xtal;
  xtal.enMode = ClkXtalModeOsc;
  xtal.enDrv = ClkXtalLowDrv;
  xtal.enFastStartup = Enable;
  CLK_XtalConfig(&xtal);
  CLK_XtalCmd(Enable);

  stc_clk_mpll_cfg_t pll;
  pll.plln = 50u;
  pll.PllpDiv = 2u;
  pll.PllqDiv = 2u;
  pll.PllrDiv = 2u;
  uint32_t t = 0;
  while (CLK_GetFlagStatus(ClkFlagXTALRdy) != Set && ++t < 2000000UL) { }
  if (t < 2000000UL) {
    pll.pllmDiv = 1u;                       /* 8 MHz */
    CLK_SetPllSource(ClkPllSrcXTAL);
  } else {
    CLK_XtalCmd(Disable);
    pll.pllmDiv = 2u;                       /* HRC 16 MHz / 2 = 8 MHz */
    CLK_SetPllSource(ClkPllSrcHRC);
  }
  CLK_MpllConfig(&pll);

  EFM_Unlock();
  EFM_SetLatency(EFM_LATENCY_5);            /* antes de subir el reloj */
  EFM_Lock();

  stc_sram_config_t ram;
  ram.u8SramIdx     = Sram12Idx | Sram3Idx | SramHsIdx | SramRetIdx;
  ram.enSramRC      = SramCycle2;
  ram.enSramWC      = SramCycle2;
  ram.enSramEccMode = EccMode3;
  ram.enSramEccOp   = SramNmi;
  ram.enSramPyOp    = SramNmi;
  SRAM_Init(&ram);

  CLK_MpllCmd(Enable);
  while (CLK_GetFlagStatus(ClkFlagMPLLRdy) != Set) { }
  CLK_SetSysClkSource(CLKSysSrcMPLL);
#endif
}
