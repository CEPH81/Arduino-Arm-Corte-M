/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: MegaHunt startup_air105.s (SDK MDK de Hezhou)
 * No editar a mano.
 */
#if defined(AIR105)

#include "../cm_startup.h"

void NMI_Handler(void) CM_WEAK_ALIAS;
void HardFault_Handler(void) CM_WEAK_ALIAS;
void MemManage_Handler(void) CM_WEAK_ALIAS;
void BusFault_Handler(void) CM_WEAK_ALIAS;
void UsageFault_Handler(void) CM_WEAK_ALIAS;
void SVC_Handler(void) CM_WEAK_ALIAS;
void DebugMon_Handler(void) CM_WEAK_ALIAS;
void PendSV_Handler(void) CM_WEAK_ALIAS;
void SysTick_Handler(void) CM_WEAK_ALIAS;
void DMA0_IRQHandler(void) CM_WEAK_ALIAS;
void USB_IRQHandler(void) CM_WEAK_ALIAS;
void USBDMA_IRQHandler(void) CM_WEAK_ALIAS;
void LCD_IRQHandler(void) CM_WEAK_ALIAS;
void SCI0_IRQHandler(void) CM_WEAK_ALIAS;
void UART0_IRQHandler(void) CM_WEAK_ALIAS;
void UART1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI0_IRQHandler(void) CM_WEAK_ALIAS;
void CRYPT0_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_0_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_1_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_2_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI2_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_IRQHandler(void) CM_WEAK_ALIAS;
void SENSOR_IRQHandler(void) CM_WEAK_ALIAS;
void TRNG_IRQHandler(void) CM_WEAK_ALIAS;
void ADC0_IRQHandler(void) CM_WEAK_ALIAS;
void SSC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_4_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_5_IRQHandler(void) CM_WEAK_ALIAS;
void KEYBOARD_IRQHandler(void) CM_WEAK_ALIAS;
void MSR_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI3_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI2_IRQHandler(void) CM_WEAK_ALIAS;
void SCI2_IRQHandler(void) CM_WEAK_ALIAS;
void UART2_IRQHandler(void) CM_WEAK_ALIAS;
void UART3_IRQHandler(void) CM_WEAK_ALIAS;
void QSPI_IRQHandler(void) CM_WEAK_ALIAS;
void I2C0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI4_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI5_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_6_IRQHandler(void) CM_WEAK_ALIAS;
void TIM0_7_IRQHandler(void) CM_WEAK_ALIAS;
void DCMI_IRQHandler(void) CM_WEAK_ALIAS;
void QR_IRQHandler(void) CM_WEAK_ALIAS;
void GPU_IRQHandler(void) CM_WEAK_ALIAS;
void AWD_IRQHandler(void) CM_WEAK_ALIAS;
void DAC_IRQHandler(void) CM_WEAK_ALIAS;
void SPI5_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(68) = {
  (cm_vector_t)&_estack,
  Reset_Handler,                   /*   1 */
  NMI_Handler,                     /*   2 */
  HardFault_Handler,               /*   3 */
  MemManage_Handler,               /*   4 */
  BusFault_Handler,                /*   5 */
  UsageFault_Handler,              /*   6 */
  0,                               /*   7 */
  0,                               /*   8 */
  0,                               /*   9 */
  0,                               /*  10 */
  SVC_Handler,                     /*  11 */
  DebugMon_Handler,                /*  12 */
  0,                               /*  13 */
  PendSV_Handler,                  /*  14 */
  SysTick_Handler,                 /*  15 */
  DMA0_IRQHandler,                 /*  16  IRQ 0 */
  USB_IRQHandler,                  /*  17  IRQ 1 */
  USBDMA_IRQHandler,               /*  18  IRQ 2 */
  LCD_IRQHandler,                  /*  19  IRQ 3 */
  SCI0_IRQHandler,                 /*  20  IRQ 4 */
  UART0_IRQHandler,                /*  21  IRQ 5 */
  UART1_IRQHandler,                /*  22  IRQ 6 */
  SPI0_IRQHandler,                 /*  23  IRQ 7 */
  CRYPT0_IRQHandler,               /*  24  IRQ 8 */
  TIM0_0_IRQHandler,               /*  25  IRQ 9 */
  TIM0_1_IRQHandler,               /*  26  IRQ 10 */
  TIM0_2_IRQHandler,               /*  27  IRQ 11 */
  TIM0_3_IRQHandler,               /*  28  IRQ 12 */
  EXTI0_IRQHandler,                /*  29  IRQ 13 */
  EXTI1_IRQHandler,                /*  30  IRQ 14 */
  EXTI2_IRQHandler,                /*  31  IRQ 15 */
  RTC_IRQHandler,                  /*  32  IRQ 16 */
  SENSOR_IRQHandler,               /*  33  IRQ 17 */
  TRNG_IRQHandler,                 /*  34  IRQ 18 */
  ADC0_IRQHandler,                 /*  35  IRQ 19 */
  SSC_IRQHandler,                  /*  36  IRQ 20 */
  TIM0_4_IRQHandler,               /*  37  IRQ 21 */
  TIM0_5_IRQHandler,               /*  38  IRQ 22 */
  KEYBOARD_IRQHandler,             /*  39  IRQ 23 */
  MSR_IRQHandler,                  /*  40  IRQ 24 */
  EXTI3_IRQHandler,                /*  41  IRQ 25 */
  SPI1_IRQHandler,                 /*  42  IRQ 26 */
  SPI2_IRQHandler,                 /*  43  IRQ 27 */
  0,                               /*  44  IRQ 28 */
  SCI2_IRQHandler,                 /*  45  IRQ 29 */
  0,                               /*  46  IRQ 30 */
  0,                               /*  47  IRQ 31 */
  UART2_IRQHandler,                /*  48  IRQ 32 */
  UART3_IRQHandler,                /*  49  IRQ 33 */
  0,                               /*  50  IRQ 34 */
  QSPI_IRQHandler,                 /*  51  IRQ 35 */
  I2C0_IRQHandler,                 /*  52  IRQ 36 */
  EXTI4_IRQHandler,                /*  53  IRQ 37 */
  EXTI5_IRQHandler,                /*  54  IRQ 38 */
  TIM0_6_IRQHandler,               /*  55  IRQ 39 */
  TIM0_7_IRQHandler,               /*  56  IRQ 40 */
  0,                               /*  57  IRQ 41 */
  DCMI_IRQHandler,                 /*  58  IRQ 42 */
  0,                               /*  59  IRQ 43 */
  0,                               /*  60  IRQ 44 */
  0,                               /*  61  IRQ 45 */
  QR_IRQHandler,                   /*  62  IRQ 46 */
  GPU_IRQHandler,                  /*  63  IRQ 47 */
  0,                               /*  64  IRQ 48 */
  AWD_IRQHandler,                  /*  65  IRQ 49 */
  DAC_IRQHandler,                  /*  66  IRQ 50 */
  SPI5_IRQHandler,                 /*  67  IRQ 51 */
};

#endif
