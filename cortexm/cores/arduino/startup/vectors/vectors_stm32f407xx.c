/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: ST startup_stm32f407xx.s
 * No editar a mano.
 */
#if defined(STM32F407xx)

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
void PVD_IRQHandler(void) CM_WEAK_ALIAS;
void TAMP_STAMP_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void FLASH_IRQHandler(void) CM_WEAK_ALIAS;
void RCC_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream0_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream6_IRQHandler(void) CM_WEAK_ALIAS;
void ADC_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_SCE_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI9_5_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_BRK_TIM9_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_UP_TIM10_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_TRG_COM_TIM11_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_CC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM2_IRQHandler(void) CM_WEAK_ALIAS;
void TIM3_IRQHandler(void) CM_WEAK_ALIAS;
void TIM4_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_ER_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_ER_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI2_IRQHandler(void) CM_WEAK_ALIAS;
void USART1_IRQHandler(void) CM_WEAK_ALIAS;
void USART2_IRQHandler(void) CM_WEAK_ALIAS;
void USART3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI15_10_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_Alarm_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_FS_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_BRK_TIM12_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_UP_TIM13_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_TRG_COM_TIM14_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_CC_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Stream7_IRQHandler(void) CM_WEAK_ALIAS;
void FSMC_IRQHandler(void) CM_WEAK_ALIAS;
void SDIO_IRQHandler(void) CM_WEAK_ALIAS;
void TIM5_IRQHandler(void) CM_WEAK_ALIAS;
void SPI3_IRQHandler(void) CM_WEAK_ALIAS;
void UART4_IRQHandler(void) CM_WEAK_ALIAS;
void UART5_IRQHandler(void) CM_WEAK_ALIAS;
void TIM6_DAC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM7_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream0_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream4_IRQHandler(void) CM_WEAK_ALIAS;
void ETH_IRQHandler(void) CM_WEAK_ALIAS;
void ETH_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_SCE_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_FS_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Stream7_IRQHandler(void) CM_WEAK_ALIAS;
void USART6_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_ER_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_HS_EP1_OUT_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_HS_EP1_IN_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_HS_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void OTG_HS_IRQHandler(void) CM_WEAK_ALIAS;
void DCMI_IRQHandler(void) CM_WEAK_ALIAS;
void HASH_RNG_IRQHandler(void) CM_WEAK_ALIAS;
void FPU_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(98) = {
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
  PVD_IRQHandler,                  /*  17  IRQ 1 */
  TAMP_STAMP_IRQHandler,           /*  18  IRQ 2 */
  RTC_WKUP_IRQHandler,             /*  19  IRQ 3 */
  FLASH_IRQHandler,                /*  20  IRQ 4 */
  RCC_IRQHandler,                  /*  21  IRQ 5 */
  EXTI0_IRQHandler,                /*  22  IRQ 6 */
  EXTI1_IRQHandler,                /*  23  IRQ 7 */
  EXTI2_IRQHandler,                /*  24  IRQ 8 */
  EXTI3_IRQHandler,                /*  25  IRQ 9 */
  EXTI4_IRQHandler,                /*  26  IRQ 10 */
  DMA1_Stream0_IRQHandler,         /*  27  IRQ 11 */
  DMA1_Stream1_IRQHandler,         /*  28  IRQ 12 */
  DMA1_Stream2_IRQHandler,         /*  29  IRQ 13 */
  DMA1_Stream3_IRQHandler,         /*  30  IRQ 14 */
  DMA1_Stream4_IRQHandler,         /*  31  IRQ 15 */
  DMA1_Stream5_IRQHandler,         /*  32  IRQ 16 */
  DMA1_Stream6_IRQHandler,         /*  33  IRQ 17 */
  ADC_IRQHandler,                  /*  34  IRQ 18 */
  CAN1_TX_IRQHandler,              /*  35  IRQ 19 */
  CAN1_RX0_IRQHandler,             /*  36  IRQ 20 */
  CAN1_RX1_IRQHandler,             /*  37  IRQ 21 */
  CAN1_SCE_IRQHandler,             /*  38  IRQ 22 */
  EXTI9_5_IRQHandler,              /*  39  IRQ 23 */
  TIM1_BRK_TIM9_IRQHandler,        /*  40  IRQ 24 */
  TIM1_UP_TIM10_IRQHandler,        /*  41  IRQ 25 */
  TIM1_TRG_COM_TIM11_IRQHandler,   /*  42  IRQ 26 */
  TIM1_CC_IRQHandler,              /*  43  IRQ 27 */
  TIM2_IRQHandler,                 /*  44  IRQ 28 */
  TIM3_IRQHandler,                 /*  45  IRQ 29 */
  TIM4_IRQHandler,                 /*  46  IRQ 30 */
  I2C1_EV_IRQHandler,              /*  47  IRQ 31 */
  I2C1_ER_IRQHandler,              /*  48  IRQ 32 */
  I2C2_EV_IRQHandler,              /*  49  IRQ 33 */
  I2C2_ER_IRQHandler,              /*  50  IRQ 34 */
  SPI1_IRQHandler,                 /*  51  IRQ 35 */
  SPI2_IRQHandler,                 /*  52  IRQ 36 */
  USART1_IRQHandler,               /*  53  IRQ 37 */
  USART2_IRQHandler,               /*  54  IRQ 38 */
  USART3_IRQHandler,               /*  55  IRQ 39 */
  EXTI15_10_IRQHandler,            /*  56  IRQ 40 */
  RTC_Alarm_IRQHandler,            /*  57  IRQ 41 */
  OTG_FS_WKUP_IRQHandler,          /*  58  IRQ 42 */
  TIM8_BRK_TIM12_IRQHandler,       /*  59  IRQ 43 */
  TIM8_UP_TIM13_IRQHandler,        /*  60  IRQ 44 */
  TIM8_TRG_COM_TIM14_IRQHandler,   /*  61  IRQ 45 */
  TIM8_CC_IRQHandler,              /*  62  IRQ 46 */
  DMA1_Stream7_IRQHandler,         /*  63  IRQ 47 */
  FSMC_IRQHandler,                 /*  64  IRQ 48 */
  SDIO_IRQHandler,                 /*  65  IRQ 49 */
  TIM5_IRQHandler,                 /*  66  IRQ 50 */
  SPI3_IRQHandler,                 /*  67  IRQ 51 */
  UART4_IRQHandler,                /*  68  IRQ 52 */
  UART5_IRQHandler,                /*  69  IRQ 53 */
  TIM6_DAC_IRQHandler,             /*  70  IRQ 54 */
  TIM7_IRQHandler,                 /*  71  IRQ 55 */
  DMA2_Stream0_IRQHandler,         /*  72  IRQ 56 */
  DMA2_Stream1_IRQHandler,         /*  73  IRQ 57 */
  DMA2_Stream2_IRQHandler,         /*  74  IRQ 58 */
  DMA2_Stream3_IRQHandler,         /*  75  IRQ 59 */
  DMA2_Stream4_IRQHandler,         /*  76  IRQ 60 */
  ETH_IRQHandler,                  /*  77  IRQ 61 */
  ETH_WKUP_IRQHandler,             /*  78  IRQ 62 */
  CAN2_TX_IRQHandler,              /*  79  IRQ 63 */
  CAN2_RX0_IRQHandler,             /*  80  IRQ 64 */
  CAN2_RX1_IRQHandler,             /*  81  IRQ 65 */
  CAN2_SCE_IRQHandler,             /*  82  IRQ 66 */
  OTG_FS_IRQHandler,               /*  83  IRQ 67 */
  DMA2_Stream5_IRQHandler,         /*  84  IRQ 68 */
  DMA2_Stream6_IRQHandler,         /*  85  IRQ 69 */
  DMA2_Stream7_IRQHandler,         /*  86  IRQ 70 */
  USART6_IRQHandler,               /*  87  IRQ 71 */
  I2C3_EV_IRQHandler,              /*  88  IRQ 72 */
  I2C3_ER_IRQHandler,              /*  89  IRQ 73 */
  OTG_HS_EP1_OUT_IRQHandler,       /*  90  IRQ 74 */
  OTG_HS_EP1_IN_IRQHandler,        /*  91  IRQ 75 */
  OTG_HS_WKUP_IRQHandler,          /*  92  IRQ 76 */
  OTG_HS_IRQHandler,               /*  93  IRQ 77 */
  DCMI_IRQHandler,                 /*  94  IRQ 78 */
  0,                               /*  95  IRQ 79 */
  HASH_RNG_IRQHandler,             /*  96  IRQ 80 */
  FPU_IRQHandler,                  /*  97  IRQ 81 */
};

#endif
