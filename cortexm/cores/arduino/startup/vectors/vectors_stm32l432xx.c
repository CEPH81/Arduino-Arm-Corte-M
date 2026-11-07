/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: ST startup_stm32l432xx.s
 * No editar a mano.
 */
#if defined(STM32L432xx)

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
void WWDG_IRQHandler(void) CM_WEAK_ALIAS;
void PVD_PVM_IRQHandler(void) CM_WEAK_ALIAS;
void TAMP_STAMP_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void FLASH_IRQHandler(void) CM_WEAK_ALIAS;
void RCC_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void ADC1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_SCE_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI9_5_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_BRK_TIM15_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_UP_TIM16_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_TRG_COM_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_CC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM2_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_ER_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void USART1_IRQHandler(void) CM_WEAK_ALIAS;
void USART2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI15_10_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_Alarm_IRQHandler(void) CM_WEAK_ALIAS;
void SPI3_IRQHandler(void) CM_WEAK_ALIAS;
void TIM6_DAC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM7_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void COMP_IRQHandler(void) CM_WEAK_ALIAS;
void LPTIM1_IRQHandler(void) CM_WEAK_ALIAS;
void LPTIM2_IRQHandler(void) CM_WEAK_ALIAS;
void USB_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void LPUART1_IRQHandler(void) CM_WEAK_ALIAS;
void QUADSPI_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_ER_IRQHandler(void) CM_WEAK_ALIAS;
void SAI1_IRQHandler(void) CM_WEAK_ALIAS;
void SWPMI1_IRQHandler(void) CM_WEAK_ALIAS;
void TSC_IRQHandler(void) CM_WEAK_ALIAS;
void RNG_IRQHandler(void) CM_WEAK_ALIAS;
void FPU_IRQHandler(void) CM_WEAK_ALIAS;
void CRS_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(99) = {
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
  WWDG_IRQHandler,                 /*  16  IRQ 0 */
  PVD_PVM_IRQHandler,              /*  17  IRQ 1 */
  TAMP_STAMP_IRQHandler,           /*  18  IRQ 2 */
  RTC_WKUP_IRQHandler,             /*  19  IRQ 3 */
  FLASH_IRQHandler,                /*  20  IRQ 4 */
  RCC_IRQHandler,                  /*  21  IRQ 5 */
  EXTI0_IRQHandler,                /*  22  IRQ 6 */
  EXTI1_IRQHandler,                /*  23  IRQ 7 */
  EXTI2_IRQHandler,                /*  24  IRQ 8 */
  EXTI3_IRQHandler,                /*  25  IRQ 9 */
  EXTI4_IRQHandler,                /*  26  IRQ 10 */
  DMA1_Channel1_IRQHandler,        /*  27  IRQ 11 */
  DMA1_Channel2_IRQHandler,        /*  28  IRQ 12 */
  DMA1_Channel3_IRQHandler,        /*  29  IRQ 13 */
  DMA1_Channel4_IRQHandler,        /*  30  IRQ 14 */
  DMA1_Channel5_IRQHandler,        /*  31  IRQ 15 */
  DMA1_Channel6_IRQHandler,        /*  32  IRQ 16 */
  DMA1_Channel7_IRQHandler,        /*  33  IRQ 17 */
  ADC1_IRQHandler,                 /*  34  IRQ 18 */
  CAN1_TX_IRQHandler,              /*  35  IRQ 19 */
  CAN1_RX0_IRQHandler,             /*  36  IRQ 20 */
  CAN1_RX1_IRQHandler,             /*  37  IRQ 21 */
  CAN1_SCE_IRQHandler,             /*  38  IRQ 22 */
  EXTI9_5_IRQHandler,              /*  39  IRQ 23 */
  TIM1_BRK_TIM15_IRQHandler,       /*  40  IRQ 24 */
  TIM1_UP_TIM16_IRQHandler,        /*  41  IRQ 25 */
  TIM1_TRG_COM_IRQHandler,         /*  42  IRQ 26 */
  TIM1_CC_IRQHandler,              /*  43  IRQ 27 */
  TIM2_IRQHandler,                 /*  44  IRQ 28 */
  0,                               /*  45  IRQ 29 */
  0,                               /*  46  IRQ 30 */
  I2C1_EV_IRQHandler,              /*  47  IRQ 31 */
  I2C1_ER_IRQHandler,              /*  48  IRQ 32 */
  0,                               /*  49  IRQ 33 */
  0,                               /*  50  IRQ 34 */
  SPI1_IRQHandler,                 /*  51  IRQ 35 */
  0,                               /*  52  IRQ 36 */
  USART1_IRQHandler,               /*  53  IRQ 37 */
  USART2_IRQHandler,               /*  54  IRQ 38 */
  0,                               /*  55  IRQ 39 */
  EXTI15_10_IRQHandler,            /*  56  IRQ 40 */
  RTC_Alarm_IRQHandler,            /*  57  IRQ 41 */
  0,                               /*  58  IRQ 42 */
  0,                               /*  59  IRQ 43 */
  0,                               /*  60  IRQ 44 */
  0,                               /*  61  IRQ 45 */
  0,                               /*  62  IRQ 46 */
  0,                               /*  63  IRQ 47 */
  0,                               /*  64  IRQ 48 */
  0,                               /*  65  IRQ 49 */
  0,                               /*  66  IRQ 50 */
  SPI3_IRQHandler,                 /*  67  IRQ 51 */
  0,                               /*  68  IRQ 52 */
  0,                               /*  69  IRQ 53 */
  TIM6_DAC_IRQHandler,             /*  70  IRQ 54 */
  TIM7_IRQHandler,                 /*  71  IRQ 55 */
  DMA2_Channel1_IRQHandler,        /*  72  IRQ 56 */
  DMA2_Channel2_IRQHandler,        /*  73  IRQ 57 */
  DMA2_Channel3_IRQHandler,        /*  74  IRQ 58 */
  DMA2_Channel4_IRQHandler,        /*  75  IRQ 59 */
  DMA2_Channel5_IRQHandler,        /*  76  IRQ 60 */
  0,                               /*  77  IRQ 61 */
  0,                               /*  78  IRQ 62 */
  0,                               /*  79  IRQ 63 */
  COMP_IRQHandler,                 /*  80  IRQ 64 */
  LPTIM1_IRQHandler,               /*  81  IRQ 65 */
  LPTIM2_IRQHandler,               /*  82  IRQ 66 */
  USB_IRQHandler,                  /*  83  IRQ 67 */
  DMA2_Channel6_IRQHandler,        /*  84  IRQ 68 */
  DMA2_Channel7_IRQHandler,        /*  85  IRQ 69 */
  LPUART1_IRQHandler,              /*  86  IRQ 70 */
  QUADSPI_IRQHandler,              /*  87  IRQ 71 */
  I2C3_EV_IRQHandler,              /*  88  IRQ 72 */
  I2C3_ER_IRQHandler,              /*  89  IRQ 73 */
  SAI1_IRQHandler,                 /*  90  IRQ 74 */
  0,                               /*  91  IRQ 75 */
  SWPMI1_IRQHandler,               /*  92  IRQ 76 */
  TSC_IRQHandler,                  /*  93  IRQ 77 */
  0,                               /*  94  IRQ 78 */
  0,                               /*  95  IRQ 79 */
  RNG_IRQHandler,                  /*  96  IRQ 80 */
  FPU_IRQHandler,                  /*  97  IRQ 81 */
  CRS_IRQHandler,                  /*  98  IRQ 82 */
};

#endif
