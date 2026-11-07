/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: ST startup_stm32g431xx.s
 * No editar a mano.
 */
#if defined(STM32G431xx)

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
void RTC_TAMP_LSECSS_IRQHandler(void) CM_WEAK_ALIAS;
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
void ADC1_2_IRQHandler(void) CM_WEAK_ALIAS;
void USB_HP_IRQHandler(void) CM_WEAK_ALIAS;
void USB_LP_IRQHandler(void) CM_WEAK_ALIAS;
void FDCAN1_IT0_IRQHandler(void) CM_WEAK_ALIAS;
void FDCAN1_IT1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI9_5_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_BRK_TIM15_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_UP_TIM16_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_TRG_COM_TIM17_IRQHandler(void) CM_WEAK_ALIAS;
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
void USBWakeUp_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_BRK_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_UP_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_TRG_COM_IRQHandler(void) CM_WEAK_ALIAS;
void TIM8_CC_IRQHandler(void) CM_WEAK_ALIAS;
void LPTIM1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI3_IRQHandler(void) CM_WEAK_ALIAS;
void UART4_IRQHandler(void) CM_WEAK_ALIAS;
void TIM6_DAC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM7_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void UCPD1_IRQHandler(void) CM_WEAK_ALIAS;
void COMP1_2_3_IRQHandler(void) CM_WEAK_ALIAS;
void COMP4_IRQHandler(void) CM_WEAK_ALIAS;
void CRS_IRQHandler(void) CM_WEAK_ALIAS;
void SAI1_IRQHandler(void) CM_WEAK_ALIAS;
void FPU_IRQHandler(void) CM_WEAK_ALIAS;
void RNG_IRQHandler(void) CM_WEAK_ALIAS;
void LPUART1_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_ER_IRQHandler(void) CM_WEAK_ALIAS;
void DMAMUX_OVR_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void CORDIC_IRQHandler(void) CM_WEAK_ALIAS;
void FMAC_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(118) = {
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
  RTC_TAMP_LSECSS_IRQHandler,      /*  18  IRQ 2 */
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
  0,                               /*  33  IRQ 17 */
  ADC1_2_IRQHandler,               /*  34  IRQ 18 */
  USB_HP_IRQHandler,               /*  35  IRQ 19 */
  USB_LP_IRQHandler,               /*  36  IRQ 20 */
  FDCAN1_IT0_IRQHandler,           /*  37  IRQ 21 */
  FDCAN1_IT1_IRQHandler,           /*  38  IRQ 22 */
  EXTI9_5_IRQHandler,              /*  39  IRQ 23 */
  TIM1_BRK_TIM15_IRQHandler,       /*  40  IRQ 24 */
  TIM1_UP_TIM16_IRQHandler,        /*  41  IRQ 25 */
  TIM1_TRG_COM_TIM17_IRQHandler,   /*  42  IRQ 26 */
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
  USBWakeUp_IRQHandler,            /*  58  IRQ 42 */
  TIM8_BRK_IRQHandler,             /*  59  IRQ 43 */
  TIM8_UP_IRQHandler,              /*  60  IRQ 44 */
  TIM8_TRG_COM_IRQHandler,         /*  61  IRQ 45 */
  TIM8_CC_IRQHandler,              /*  62  IRQ 46 */
  0,                               /*  63  IRQ 47 */
  0,                               /*  64  IRQ 48 */
  LPTIM1_IRQHandler,               /*  65  IRQ 49 */
  0,                               /*  66  IRQ 50 */
  SPI3_IRQHandler,                 /*  67  IRQ 51 */
  UART4_IRQHandler,                /*  68  IRQ 52 */
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
  UCPD1_IRQHandler,                /*  79  IRQ 63 */
  COMP1_2_3_IRQHandler,            /*  80  IRQ 64 */
  COMP4_IRQHandler,                /*  81  IRQ 65 */
  0,                               /*  82  IRQ 66 */
  0,                               /*  83  IRQ 67 */
  0,                               /*  84  IRQ 68 */
  0,                               /*  85  IRQ 69 */
  0,                               /*  86  IRQ 70 */
  0,                               /*  87  IRQ 71 */
  0,                               /*  88  IRQ 72 */
  0,                               /*  89  IRQ 73 */
  0,                               /*  90  IRQ 74 */
  CRS_IRQHandler,                  /*  91  IRQ 75 */
  SAI1_IRQHandler,                 /*  92  IRQ 76 */
  0,                               /*  93  IRQ 77 */
  0,                               /*  94  IRQ 78 */
  0,                               /*  95  IRQ 79 */
  0,                               /*  96  IRQ 80 */
  FPU_IRQHandler,                  /*  97  IRQ 81 */
  0,                               /*  98  IRQ 82 */
  0,                               /*  99  IRQ 83 */
  0,                               /* 100  IRQ 84 */
  0,                               /* 101  IRQ 85 */
  0,                               /* 102  IRQ 86 */
  0,                               /* 103  IRQ 87 */
  0,                               /* 104  IRQ 88 */
  0,                               /* 105  IRQ 89 */
  RNG_IRQHandler,                  /* 106  IRQ 90 */
  LPUART1_IRQHandler,              /* 107  IRQ 91 */
  I2C3_EV_IRQHandler,              /* 108  IRQ 92 */
  I2C3_ER_IRQHandler,              /* 109  IRQ 93 */
  DMAMUX_OVR_IRQHandler,           /* 110  IRQ 94 */
  0,                               /* 111  IRQ 95 */
  0,                               /* 112  IRQ 96 */
  DMA2_Channel6_IRQHandler,        /* 113  IRQ 97 */
  0,                               /* 114  IRQ 98 */
  0,                               /* 115  IRQ 99 */
  CORDIC_IRQHandler,               /* 116  IRQ 100 */
  FMAC_IRQHandler,                 /* 117  IRQ 101 */
};

#endif
