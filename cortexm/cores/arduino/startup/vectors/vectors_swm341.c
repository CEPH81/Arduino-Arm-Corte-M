/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: Synwit startup_SWM341.s
 * No editar a mano.
 */
#if defined(CHIP_SWM341)

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
void UART0_Handler(void) CM_WEAK_ALIAS;
void UART1_Handler(void) CM_WEAK_ALIAS;
void TIMR0_Handler(void) CM_WEAK_ALIAS;
void TIMR1_Handler(void) CM_WEAK_ALIAS;
void DMA_Handler(void) CM_WEAK_ALIAS;
void SPI0_Handler(void) CM_WEAK_ALIAS;
void PWM0_Handler(void) CM_WEAK_ALIAS;
void WDT_Handler(void) CM_WEAK_ALIAS;
void UART2_Handler(void) CM_WEAK_ALIAS;
void PWM1_Handler(void) CM_WEAK_ALIAS;
void ADC0_Handler(void) CM_WEAK_ALIAS;
void BTIMR0_Handler(void) CM_WEAK_ALIAS;
void HALL0_Handler(void) CM_WEAK_ALIAS;
void PWM2_Handler(void) CM_WEAK_ALIAS;
void PWMBRK_Handler(void) CM_WEAK_ALIAS;
void I2C0_Handler(void) CM_WEAK_ALIAS;
void CAN0_Handler(void) CM_WEAK_ALIAS;
void SPI1_Handler(void) CM_WEAK_ALIAS;
void RTC_Handler(void) CM_WEAK_ALIAS;
void PWM3_Handler(void) CM_WEAK_ALIAS;
void TIMR2_Handler(void) CM_WEAK_ALIAS;
void UART3_Handler(void) CM_WEAK_ALIAS;
void TIMR3_Handler(void) CM_WEAK_ALIAS;
void ADC1_Handler(void) CM_WEAK_ALIAS;
void BOD_Handler(void) CM_WEAK_ALIAS;
void CORDIC_Handler(void) CM_WEAK_ALIAS;
void BTIMR1_Handler(void) CM_WEAK_ALIAS;
void PWM4_Handler(void) CM_WEAK_ALIAS;
void HALL3_Handler(void) CM_WEAK_ALIAS;
void BTIMR2_Handler(void) CM_WEAK_ALIAS;
void I2C1_Handler(void) CM_WEAK_ALIAS;
void BTIMR3_Handler(void) CM_WEAK_ALIAS;
void ACMP_Handler(void) CM_WEAK_ALIAS;
void XTALSTOP_Handler(void) CM_WEAK_ALIAS;
void FSPI_Handler(void) CM_WEAK_ALIAS;
void GPIOA_Handler(void) CM_WEAK_ALIAS;
void GPIOB_Handler(void) CM_WEAK_ALIAS;
void GPIOC_Handler(void) CM_WEAK_ALIAS;
void GPIOD_Handler(void) CM_WEAK_ALIAS;
void GPIOM_Handler(void) CM_WEAK_ALIAS;
void GPION_Handler(void) CM_WEAK_ALIAS;
void GPIOA0_Handler(void) CM_WEAK_ALIAS;
void GPIOA1_Handler(void) CM_WEAK_ALIAS;
void GPIOA5_Handler(void) CM_WEAK_ALIAS;
void GPIOA6_Handler(void) CM_WEAK_ALIAS;
void GPIOA10_Handler(void) CM_WEAK_ALIAS;
void GPIOA11_Handler(void) CM_WEAK_ALIAS;
void GPIOA12_Handler(void) CM_WEAK_ALIAS;
void GPIOA13_Handler(void) CM_WEAK_ALIAS;
void GPIOB0_Handler(void) CM_WEAK_ALIAS;
void GPIOB1_Handler(void) CM_WEAK_ALIAS;
void GPIOB2_Handler(void) CM_WEAK_ALIAS;
void GPIOC0_Handler(void) CM_WEAK_ALIAS;
void GPIOC1_Handler(void) CM_WEAK_ALIAS;
void GPIOC2_Handler(void) CM_WEAK_ALIAS;
void GPIOC3_Handler(void) CM_WEAK_ALIAS;
void GPIOC4_Handler(void) CM_WEAK_ALIAS;
void GPIOD3_Handler(void) CM_WEAK_ALIAS;
void GPIOD4_Handler(void) CM_WEAK_ALIAS;
void GPIOD5_Handler(void) CM_WEAK_ALIAS;
void GPIOD6_Handler(void) CM_WEAK_ALIAS;
void GPIOD7_Handler(void) CM_WEAK_ALIAS;
void GPIOD8_Handler(void) CM_WEAK_ALIAS;
void GPIOC9_Handler(void) CM_WEAK_ALIAS;
void GPIOC10_Handler(void) CM_WEAK_ALIAS;
void GPIOC11_Handler(void) CM_WEAK_ALIAS;
void GPIOC12_Handler(void) CM_WEAK_ALIAS;
void GPIOM0_Handler(void) CM_WEAK_ALIAS;
void GPIOM1_Handler(void) CM_WEAK_ALIAS;
void GPIOM2_Handler(void) CM_WEAK_ALIAS;
void GPIOM3_Handler(void) CM_WEAK_ALIAS;
void GPIOM4_Handler(void) CM_WEAK_ALIAS;
void DIV_Handler(void) CM_WEAK_ALIAS;
void LCD_Handler(void) CM_WEAK_ALIAS;
void GPIOE_Handler(void) CM_WEAK_ALIAS;
void JPEG_Handler(void) CM_WEAK_ALIAS;
void SDIO_Handler(void) CM_WEAK_ALIAS;
void USB_Handler(void) CM_WEAK_ALIAS;
void CAN1_Handler(void) CM_WEAK_ALIAS;
void TIMR4_Handler(void) CM_WEAK_ALIAS;
void BTIMR4_Handler(void) CM_WEAK_ALIAS;
void BTIMR5_Handler(void) CM_WEAK_ALIAS;
void BTIMR6_Handler(void) CM_WEAK_ALIAS;
void BTIMR7_Handler(void) CM_WEAK_ALIAS;
void BTIMR8_Handler(void) CM_WEAK_ALIAS;
void BTIMR9_Handler(void) CM_WEAK_ALIAS;
void BTIMR10_Handler(void) CM_WEAK_ALIAS;
void BTIMR11_Handler(void) CM_WEAK_ALIAS;
void DMA2D_Handler(void) CM_WEAK_ALIAS;
void QEI_Handler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(106) = {
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
  UART0_Handler,                   /*  16  IRQ 0 */
  UART1_Handler,                   /*  17  IRQ 1 */
  TIMR0_Handler,                   /*  18  IRQ 2 */
  TIMR1_Handler,                   /*  19  IRQ 3 */
  DMA_Handler,                     /*  20  IRQ 4 */
  SPI0_Handler,                    /*  21  IRQ 5 */
  PWM0_Handler,                    /*  22  IRQ 6 */
  WDT_Handler,                     /*  23  IRQ 7 */
  UART2_Handler,                   /*  24  IRQ 8 */
  PWM1_Handler,                    /*  25  IRQ 9 */
  ADC0_Handler,                    /*  26  IRQ 10 */
  BTIMR0_Handler,                  /*  27  IRQ 11 */
  HALL0_Handler,                   /*  28  IRQ 12 */
  PWM2_Handler,                    /*  29  IRQ 13 */
  PWMBRK_Handler,                  /*  30  IRQ 14 */
  I2C0_Handler,                    /*  31  IRQ 15 */
  CAN0_Handler,                    /*  32  IRQ 16 */
  SPI1_Handler,                    /*  33  IRQ 17 */
  RTC_Handler,                     /*  34  IRQ 18 */
  PWM3_Handler,                    /*  35  IRQ 19 */
  TIMR2_Handler,                   /*  36  IRQ 20 */
  UART3_Handler,                   /*  37  IRQ 21 */
  TIMR3_Handler,                   /*  38  IRQ 22 */
  ADC1_Handler,                    /*  39  IRQ 23 */
  BOD_Handler,                     /*  40  IRQ 24 */
  CORDIC_Handler,                  /*  41  IRQ 25 */
  BTIMR1_Handler,                  /*  42  IRQ 26 */
  PWM4_Handler,                    /*  43  IRQ 27 */
  HALL3_Handler,                   /*  44  IRQ 28 */
  BTIMR2_Handler,                  /*  45  IRQ 29 */
  I2C1_Handler,                    /*  46  IRQ 30 */
  BTIMR3_Handler,                  /*  47  IRQ 31 */
  ACMP_Handler,                    /*  48  IRQ 32 */
  XTALSTOP_Handler,                /*  49  IRQ 33 */
  FSPI_Handler,                    /*  50  IRQ 34 */
  GPIOA_Handler,                   /*  51  IRQ 35 */
  GPIOB_Handler,                   /*  52  IRQ 36 */
  GPIOC_Handler,                   /*  53  IRQ 37 */
  GPIOD_Handler,                   /*  54  IRQ 38 */
  GPIOM_Handler,                   /*  55  IRQ 39 */
  GPION_Handler,                   /*  56  IRQ 40 */
  GPIOA0_Handler,                  /*  57  IRQ 41 */
  GPIOA1_Handler,                  /*  58  IRQ 42 */
  GPIOA5_Handler,                  /*  59  IRQ 43 */
  GPIOA6_Handler,                  /*  60  IRQ 44 */
  GPIOA10_Handler,                 /*  61  IRQ 45 */
  GPIOA11_Handler,                 /*  62  IRQ 46 */
  GPIOA12_Handler,                 /*  63  IRQ 47 */
  GPIOA13_Handler,                 /*  64  IRQ 48 */
  GPIOB0_Handler,                  /*  65  IRQ 49 */
  GPIOB1_Handler,                  /*  66  IRQ 50 */
  GPIOB2_Handler,                  /*  67  IRQ 51 */
  GPIOC0_Handler,                  /*  68  IRQ 52 */
  GPIOC1_Handler,                  /*  69  IRQ 53 */
  GPIOC2_Handler,                  /*  70  IRQ 54 */
  GPIOC3_Handler,                  /*  71  IRQ 55 */
  GPIOC4_Handler,                  /*  72  IRQ 56 */
  GPIOD3_Handler,                  /*  73  IRQ 57 */
  GPIOD4_Handler,                  /*  74  IRQ 58 */
  GPIOD5_Handler,                  /*  75  IRQ 59 */
  GPIOD6_Handler,                  /*  76  IRQ 60 */
  GPIOD7_Handler,                  /*  77  IRQ 61 */
  GPIOD8_Handler,                  /*  78  IRQ 62 */
  GPIOC9_Handler,                  /*  79  IRQ 63 */
  GPIOC10_Handler,                 /*  80  IRQ 64 */
  GPIOC11_Handler,                 /*  81  IRQ 65 */
  GPIOC12_Handler,                 /*  82  IRQ 66 */
  GPIOM0_Handler,                  /*  83  IRQ 67 */
  GPIOM1_Handler,                  /*  84  IRQ 68 */
  GPIOM2_Handler,                  /*  85  IRQ 69 */
  GPIOM3_Handler,                  /*  86  IRQ 70 */
  GPIOM4_Handler,                  /*  87  IRQ 71 */
  DIV_Handler,                     /*  88  IRQ 72 */
  LCD_Handler,                     /*  89  IRQ 73 */
  GPIOE_Handler,                   /*  90  IRQ 74 */
  JPEG_Handler,                    /*  91  IRQ 75 */
  SDIO_Handler,                    /*  92  IRQ 76 */
  USB_Handler,                     /*  93  IRQ 77 */
  CAN1_Handler,                    /*  94  IRQ 78 */
  TIMR4_Handler,                   /*  95  IRQ 79 */
  BTIMR4_Handler,                  /*  96  IRQ 80 */
  BTIMR5_Handler,                  /*  97  IRQ 81 */
  BTIMR6_Handler,                  /*  98  IRQ 82 */
  BTIMR7_Handler,                  /*  99  IRQ 83 */
  BTIMR8_Handler,                  /* 100  IRQ 84 */
  BTIMR9_Handler,                  /* 101  IRQ 85 */
  BTIMR10_Handler,                 /* 102  IRQ 86 */
  BTIMR11_Handler,                 /* 103  IRQ 87 */
  DMA2D_Handler,                   /* 104  IRQ 88 */
  QEI_Handler,                     /* 105  IRQ 89 */
};

#endif
