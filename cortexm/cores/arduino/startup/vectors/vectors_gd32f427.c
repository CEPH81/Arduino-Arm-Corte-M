/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: GigaDevice startup_gd32f407_427.S
 * No editar a mano.
 */
#if defined(GD32F427)

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
void WWDGT_IRQHandler(void) CM_WEAK_ALIAS;
void LVD_IRQHandler(void) CM_WEAK_ALIAS;
void TAMPER_STAMP_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void FMC_IRQHandler(void) CM_WEAK_ALIAS;
void RCU_CTC_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI0_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI1_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI3_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel0_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void ADC_IRQHandler(void) CM_WEAK_ALIAS;
void CAN0_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN0_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN0_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN0_EWMC_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI5_9_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER0_BRK_TIMER8_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER0_UP_TIMER9_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER0_TRG_CMT_TIMER10_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER0_Channel_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER1_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER2_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER3_IRQHandler(void) CM_WEAK_ALIAS;
void I2C0_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C0_ER_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_ER_IRQHandler(void) CM_WEAK_ALIAS;
void SPI0_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void USART0_IRQHandler(void) CM_WEAK_ALIAS;
void USART1_IRQHandler(void) CM_WEAK_ALIAS;
void USART2_IRQHandler(void) CM_WEAK_ALIAS;
void EXTI10_15_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_Alarm_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER7_BRK_TIMER11_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER7_UP_TIMER12_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER7_TRG_CMT_TIMER13_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER7_Channel_IRQHandler(void) CM_WEAK_ALIAS;
void DMA0_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void EXMC_IRQHandler(void) CM_WEAK_ALIAS;
void SDIO_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER4_IRQHandler(void) CM_WEAK_ALIAS;
void SPI2_IRQHandler(void) CM_WEAK_ALIAS;
void UART3_IRQHandler(void) CM_WEAK_ALIAS;
void UART4_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER5_DAC_IRQHandler(void) CM_WEAK_ALIAS;
void TIMER6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel0_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void ENET_IRQHandler(void) CM_WEAK_ALIAS;
void ENET_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_EWMC_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void USART5_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_EV_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_ER_IRQHandler(void) CM_WEAK_ALIAS;
void USBHS_EP1_Out_IRQHandler(void) CM_WEAK_ALIAS;
void USBHS_EP1_In_IRQHandler(void) CM_WEAK_ALIAS;
void USBHS_WKUP_IRQHandler(void) CM_WEAK_ALIAS;
void USBHS_IRQHandler(void) CM_WEAK_ALIAS;
void DCI_IRQHandler(void) CM_WEAK_ALIAS;
void TRNG_IRQHandler(void) CM_WEAK_ALIAS;
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
  WWDGT_IRQHandler,                /*  16  IRQ 0 */
  LVD_IRQHandler,                  /*  17  IRQ 1 */
  TAMPER_STAMP_IRQHandler,         /*  18  IRQ 2 */
  RTC_WKUP_IRQHandler,             /*  19  IRQ 3 */
  FMC_IRQHandler,                  /*  20  IRQ 4 */
  RCU_CTC_IRQHandler,              /*  21  IRQ 5 */
  EXTI0_IRQHandler,                /*  22  IRQ 6 */
  EXTI1_IRQHandler,                /*  23  IRQ 7 */
  EXTI2_IRQHandler,                /*  24  IRQ 8 */
  EXTI3_IRQHandler,                /*  25  IRQ 9 */
  EXTI4_IRQHandler,                /*  26  IRQ 10 */
  DMA0_Channel0_IRQHandler,        /*  27  IRQ 11 */
  DMA0_Channel1_IRQHandler,        /*  28  IRQ 12 */
  DMA0_Channel2_IRQHandler,        /*  29  IRQ 13 */
  DMA0_Channel3_IRQHandler,        /*  30  IRQ 14 */
  DMA0_Channel4_IRQHandler,        /*  31  IRQ 15 */
  DMA0_Channel5_IRQHandler,        /*  32  IRQ 16 */
  DMA0_Channel6_IRQHandler,        /*  33  IRQ 17 */
  ADC_IRQHandler,                  /*  34  IRQ 18 */
  CAN0_TX_IRQHandler,              /*  35  IRQ 19 */
  CAN0_RX0_IRQHandler,             /*  36  IRQ 20 */
  CAN0_RX1_IRQHandler,             /*  37  IRQ 21 */
  CAN0_EWMC_IRQHandler,            /*  38  IRQ 22 */
  EXTI5_9_IRQHandler,              /*  39  IRQ 23 */
  TIMER0_BRK_TIMER8_IRQHandler,    /*  40  IRQ 24 */
  TIMER0_UP_TIMER9_IRQHandler,     /*  41  IRQ 25 */
  TIMER0_TRG_CMT_TIMER10_IRQHandler, /*  42  IRQ 26 */
  TIMER0_Channel_IRQHandler,       /*  43  IRQ 27 */
  TIMER1_IRQHandler,               /*  44  IRQ 28 */
  TIMER2_IRQHandler,               /*  45  IRQ 29 */
  TIMER3_IRQHandler,               /*  46  IRQ 30 */
  I2C0_EV_IRQHandler,              /*  47  IRQ 31 */
  I2C0_ER_IRQHandler,              /*  48  IRQ 32 */
  I2C1_EV_IRQHandler,              /*  49  IRQ 33 */
  I2C1_ER_IRQHandler,              /*  50  IRQ 34 */
  SPI0_IRQHandler,                 /*  51  IRQ 35 */
  SPI1_IRQHandler,                 /*  52  IRQ 36 */
  USART0_IRQHandler,               /*  53  IRQ 37 */
  USART1_IRQHandler,               /*  54  IRQ 38 */
  USART2_IRQHandler,               /*  55  IRQ 39 */
  EXTI10_15_IRQHandler,            /*  56  IRQ 40 */
  RTC_Alarm_IRQHandler,            /*  57  IRQ 41 */
  USBFS_WKUP_IRQHandler,           /*  58  IRQ 42 */
  TIMER7_BRK_TIMER11_IRQHandler,   /*  59  IRQ 43 */
  TIMER7_UP_TIMER12_IRQHandler,    /*  60  IRQ 44 */
  TIMER7_TRG_CMT_TIMER13_IRQHandler, /*  61  IRQ 45 */
  TIMER7_Channel_IRQHandler,       /*  62  IRQ 46 */
  DMA0_Channel7_IRQHandler,        /*  63  IRQ 47 */
  EXMC_IRQHandler,                 /*  64  IRQ 48 */
  SDIO_IRQHandler,                 /*  65  IRQ 49 */
  TIMER4_IRQHandler,               /*  66  IRQ 50 */
  SPI2_IRQHandler,                 /*  67  IRQ 51 */
  UART3_IRQHandler,                /*  68  IRQ 52 */
  UART4_IRQHandler,                /*  69  IRQ 53 */
  TIMER5_DAC_IRQHandler,           /*  70  IRQ 54 */
  TIMER6_IRQHandler,               /*  71  IRQ 55 */
  DMA1_Channel0_IRQHandler,        /*  72  IRQ 56 */
  DMA1_Channel1_IRQHandler,        /*  73  IRQ 57 */
  DMA1_Channel2_IRQHandler,        /*  74  IRQ 58 */
  DMA1_Channel3_IRQHandler,        /*  75  IRQ 59 */
  DMA1_Channel4_IRQHandler,        /*  76  IRQ 60 */
  ENET_IRQHandler,                 /*  77  IRQ 61 */
  ENET_WKUP_IRQHandler,            /*  78  IRQ 62 */
  CAN1_TX_IRQHandler,              /*  79  IRQ 63 */
  CAN1_RX0_IRQHandler,             /*  80  IRQ 64 */
  CAN1_RX1_IRQHandler,             /*  81  IRQ 65 */
  CAN1_EWMC_IRQHandler,            /*  82  IRQ 66 */
  USBFS_IRQHandler,                /*  83  IRQ 67 */
  DMA1_Channel5_IRQHandler,        /*  84  IRQ 68 */
  DMA1_Channel6_IRQHandler,        /*  85  IRQ 69 */
  DMA1_Channel7_IRQHandler,        /*  86  IRQ 70 */
  USART5_IRQHandler,               /*  87  IRQ 71 */
  I2C2_EV_IRQHandler,              /*  88  IRQ 72 */
  I2C2_ER_IRQHandler,              /*  89  IRQ 73 */
  USBHS_EP1_Out_IRQHandler,        /*  90  IRQ 74 */
  USBHS_EP1_In_IRQHandler,         /*  91  IRQ 75 */
  USBHS_WKUP_IRQHandler,           /*  92  IRQ 76 */
  USBHS_IRQHandler,                /*  93  IRQ 77 */
  DCI_IRQHandler,                  /*  94  IRQ 78 */
  0,                               /*  95  IRQ 79 */
  TRNG_IRQHandler,                 /*  96  IRQ 80 */
  FPU_IRQHandler,                  /*  97  IRQ 81 */
};

#endif
