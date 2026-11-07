/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: ST startup_stm32h503xx.s
 * No editar a mano.
 */
#if defined(STM32H503xx)

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
void PVD_AVD_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_IRQHandler(void) CM_WEAK_ALIAS;
void TAMP_IRQHandler(void) CM_WEAK_ALIAS;
void RAMCFG_IRQHandler(void) CM_WEAK_ALIAS;
void FLASH_IRQHandler(void) CM_WEAK_ALIAS;
void GTZC_IRQHandler(void) CM_WEAK_ALIAS;
void RCC_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI4_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI5_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI6_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI7_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI8_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI9_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI10_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI11_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI12_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI13_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI14_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI15_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel0_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA1_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void IWDG_IRQHandler(void) CM_WEAK_ALIAS;
void ADC1_IRQHandler(void) CM_WEAK_ALIAS;
void DAC1_IRQHandler(void) CM_WEAK_ALIAS;
void FDCAN1_IT0_IRQHandler(void) CM_WEAK_ALIAS;
void FDCAN1_IT1_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_BRK_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_UP_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_TRG_COM_IRQHandler(void) CM_WEAK_ALIAS;
void TIM1_CC_IRQHandler(void) CM_WEAK_ALIAS;
void TIM2_IRQHandler(void) CM_WEAK_ALIAS;
void TIM3_IRQHandler(void) CM_WEAK_ALIAS;
void TIM6_IRQHandler(void) CM_WEAK_ALIAS;
void TIM7_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_ER_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_ER_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI2_IRQHandler(void) CM_WEAK_ALIAS;
void SPI3_IRQHandler(void) CM_WEAK_ALIAS;
void USART1_IRQHandler(void) CM_WEAK_ALIAS;
void USART2_IRQHandler(void) CM_WEAK_ALIAS;
void USART3_IRQHandler(void) CM_WEAK_ALIAS;
void LPUART1_IRQHandler(void) CM_WEAK_ALIAS;
void LPTIM1_IRQHandler(void) CM_WEAK_ALIAS;
void LPTIM2_IRQHandler(void) CM_WEAK_ALIAS;
void USB_DRD_FS_IRQHandler(void) CM_WEAK_ALIAS;
void CRS_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel0_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void GPDMA2_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void FPU_IRQHandler(void) CM_WEAK_ALIAS;
void ICACHE_IRQHandler(void) CM_WEAK_ALIAS;
void DTS_IRQHandler(void) CM_WEAK_ALIAS;
void RNG_IRQHandler(void) CM_WEAK_ALIAS;
void HASH_IRQHandler(void) CM_WEAK_ALIAS;
void I3C1_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I3C1_ER_IRQHandler(void) CM_WEAK_ALIAS;
void I3C2_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I3C2_ER_IRQHandler(void) CM_WEAK_ALIAS;
void COMP1_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(150) = {
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
  PVD_AVD_IRQHandler,              /*  17  IRQ 1 */
  RTC_IRQHandler,                  /*  18  IRQ 2 */
  0,                               /*  19  IRQ 3 */
  TAMP_IRQHandler,                 /*  20  IRQ 4 */
  RAMCFG_IRQHandler,               /*  21  IRQ 5 */
  FLASH_IRQHandler,                /*  22  IRQ 6 */
  0,                               /*  23  IRQ 7 */
  GTZC_IRQHandler,                 /*  24  IRQ 8 */
  RCC_IRQHandler,                  /*  25  IRQ 9 */
  0,                               /*  26  IRQ 10 */
  EXTI0_IRQHandler,                /*  27  IRQ 11 */
  EXTI1_IRQHandler,                /*  28  IRQ 12 */
  EXTI2_IRQHandler,                /*  29  IRQ 13 */
  EXTI3_IRQHandler,                /*  30  IRQ 14 */
  EXTI4_IRQHandler,                /*  31  IRQ 15 */
  EXTI5_IRQHandler,                /*  32  IRQ 16 */
  EXTI6_IRQHandler,                /*  33  IRQ 17 */
  EXTI7_IRQHandler,                /*  34  IRQ 18 */
  EXTI8_IRQHandler,                /*  35  IRQ 19 */
  EXTI9_IRQHandler,                /*  36  IRQ 20 */
  EXTI10_IRQHandler,               /*  37  IRQ 21 */
  EXTI11_IRQHandler,               /*  38  IRQ 22 */
  EXTI12_IRQHandler,               /*  39  IRQ 23 */
  EXTI13_IRQHandler,               /*  40  IRQ 24 */
  EXTI14_IRQHandler,               /*  41  IRQ 25 */
  EXTI15_IRQHandler,               /*  42  IRQ 26 */
  GPDMA1_Channel0_IRQHandler,      /*  43  IRQ 27 */
  GPDMA1_Channel1_IRQHandler,      /*  44  IRQ 28 */
  GPDMA1_Channel2_IRQHandler,      /*  45  IRQ 29 */
  GPDMA1_Channel3_IRQHandler,      /*  46  IRQ 30 */
  GPDMA1_Channel4_IRQHandler,      /*  47  IRQ 31 */
  GPDMA1_Channel5_IRQHandler,      /*  48  IRQ 32 */
  GPDMA1_Channel6_IRQHandler,      /*  49  IRQ 33 */
  GPDMA1_Channel7_IRQHandler,      /*  50  IRQ 34 */
  IWDG_IRQHandler,                 /*  51  IRQ 35 */
  0,                               /*  52  IRQ 36 */
  ADC1_IRQHandler,                 /*  53  IRQ 37 */
  DAC1_IRQHandler,                 /*  54  IRQ 38 */
  FDCAN1_IT0_IRQHandler,           /*  55  IRQ 39 */
  FDCAN1_IT1_IRQHandler,           /*  56  IRQ 40 */
  TIM1_BRK_IRQHandler,             /*  57  IRQ 41 */
  TIM1_UP_IRQHandler,              /*  58  IRQ 42 */
  TIM1_TRG_COM_IRQHandler,         /*  59  IRQ 43 */
  TIM1_CC_IRQHandler,              /*  60  IRQ 44 */
  TIM2_IRQHandler,                 /*  61  IRQ 45 */
  TIM3_IRQHandler,                 /*  62  IRQ 46 */
  0,                               /*  63  IRQ 47 */
  0,                               /*  64  IRQ 48 */
  TIM6_IRQHandler,                 /*  65  IRQ 49 */
  TIM7_IRQHandler,                 /*  66  IRQ 50 */
  I2C1_EV_IRQHandler,              /*  67  IRQ 51 */
  I2C1_ER_IRQHandler,              /*  68  IRQ 52 */
  I2C2_EV_IRQHandler,              /*  69  IRQ 53 */
  I2C2_ER_IRQHandler,              /*  70  IRQ 54 */
  SPI1_IRQHandler,                 /*  71  IRQ 55 */
  SPI2_IRQHandler,                 /*  72  IRQ 56 */
  SPI3_IRQHandler,                 /*  73  IRQ 57 */
  USART1_IRQHandler,               /*  74  IRQ 58 */
  USART2_IRQHandler,               /*  75  IRQ 59 */
  USART3_IRQHandler,               /*  76  IRQ 60 */
  0,                               /*  77  IRQ 61 */
  0,                               /*  78  IRQ 62 */
  LPUART1_IRQHandler,              /*  79  IRQ 63 */
  LPTIM1_IRQHandler,               /*  80  IRQ 64 */
  0,                               /*  81  IRQ 65 */
  0,                               /*  82  IRQ 66 */
  0,                               /*  83  IRQ 67 */
  0,                               /*  84  IRQ 68 */
  0,                               /*  85  IRQ 69 */
  LPTIM2_IRQHandler,               /*  86  IRQ 70 */
  0,                               /*  87  IRQ 71 */
  0,                               /*  88  IRQ 72 */
  0,                               /*  89  IRQ 73 */
  USB_DRD_FS_IRQHandler,           /*  90  IRQ 74 */
  CRS_IRQHandler,                  /*  91  IRQ 75 */
  0,                               /*  92  IRQ 76 */
  0,                               /*  93  IRQ 77 */
  0,                               /*  94  IRQ 78 */
  0,                               /*  95  IRQ 79 */
  0,                               /*  96  IRQ 80 */
  0,                               /*  97  IRQ 81 */
  0,                               /*  98  IRQ 82 */
  0,                               /*  99  IRQ 83 */
  0,                               /* 100  IRQ 84 */
  0,                               /* 101  IRQ 85 */
  0,                               /* 102  IRQ 86 */
  0,                               /* 103  IRQ 87 */
  0,                               /* 104  IRQ 88 */
  0,                               /* 105  IRQ 89 */
  GPDMA2_Channel0_IRQHandler,      /* 106  IRQ 90 */
  GPDMA2_Channel1_IRQHandler,      /* 107  IRQ 91 */
  GPDMA2_Channel2_IRQHandler,      /* 108  IRQ 92 */
  GPDMA2_Channel3_IRQHandler,      /* 109  IRQ 93 */
  GPDMA2_Channel4_IRQHandler,      /* 110  IRQ 94 */
  GPDMA2_Channel5_IRQHandler,      /* 111  IRQ 95 */
  GPDMA2_Channel6_IRQHandler,      /* 112  IRQ 96 */
  GPDMA2_Channel7_IRQHandler,      /* 113  IRQ 97 */
  0,                               /* 114  IRQ 98 */
  0,                               /* 115  IRQ 99 */
  0,                               /* 116  IRQ 100 */
  0,                               /* 117  IRQ 101 */
  0,                               /* 118  IRQ 102 */
  FPU_IRQHandler,                  /* 119  IRQ 103 */
  ICACHE_IRQHandler,               /* 120  IRQ 104 */
  0,                               /* 121  IRQ 105 */
  0,                               /* 122  IRQ 106 */
  0,                               /* 123  IRQ 107 */
  0,                               /* 124  IRQ 108 */
  0,                               /* 125  IRQ 109 */
  0,                               /* 126  IRQ 110 */
  0,                               /* 127  IRQ 111 */
  0,                               /* 128  IRQ 112 */
  DTS_IRQHandler,                  /* 129  IRQ 113 */
  RNG_IRQHandler,                  /* 130  IRQ 114 */
  0,                               /* 131  IRQ 115 */
  0,                               /* 132  IRQ 116 */
  HASH_IRQHandler,                 /* 133  IRQ 117 */
  0,                               /* 134  IRQ 118 */
  0,                               /* 135  IRQ 119 */
  0,                               /* 136  IRQ 120 */
  0,                               /* 137  IRQ 121 */
  0,                               /* 138  IRQ 122 */
  I3C1_EV_IRQHandler,              /* 139  IRQ 123 */
  I3C1_ER_IRQHandler,              /* 140  IRQ 124 */
  0,                               /* 141  IRQ 125 */
  0,                               /* 142  IRQ 126 */
  0,                               /* 143  IRQ 127 */
  0,                               /* 144  IRQ 128 */
  0,                               /* 145  IRQ 129 */
  0,                               /* 146  IRQ 130 */
  I3C2_EV_IRQHandler,              /* 147  IRQ 131 */
  I3C2_ER_IRQHandler,              /* 148  IRQ 132 */
  COMP1_IRQHandler,                /* 149  IRQ 133 */
};

#endif
