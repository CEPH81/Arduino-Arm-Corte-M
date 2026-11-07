/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: Artery startup_at32f403a_407.s
 * No editar a mano.
 */
#if defined(AT32F403A_407)

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
void WWDT_IRQHandler(void) CM_WEAK_ALIAS;
void PVM_IRQHandler(void) CM_WEAK_ALIAS;
void TAMPER_IRQHandler(void) CM_WEAK_ALIAS;
void RTC_IRQHandler(void) CM_WEAK_ALIAS;
void FLASH_IRQHandler(void) CM_WEAK_ALIAS;
void CRM_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT0_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT1_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT2_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT3_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel4_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel5_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel6_IRQHandler(void) CM_WEAK_ALIAS;
void DMA1_Channel7_IRQHandler(void) CM_WEAK_ALIAS;
void ADC1_2_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_H_CAN1_TX_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_L_CAN1_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN1_SE_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT9_5_IRQHandler(void) CM_WEAK_ALIAS;
void TMR1_BRK_TMR9_IRQHandler(void) CM_WEAK_ALIAS;
void TMR1_OVF_TMR10_IRQHandler(void) CM_WEAK_ALIAS;
void TMR1_TRG_HALL_TMR11_IRQHandler(void) CM_WEAK_ALIAS;
void TMR1_CH_IRQHandler(void) CM_WEAK_ALIAS;
void TMR2_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void TMR3_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void TMR4_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_EVT_IRQHandler(void) CM_WEAK_ALIAS;
void I2C1_ERR_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_EVT_IRQHandler(void) CM_WEAK_ALIAS;
void I2C2_ERR_IRQHandler(void) CM_WEAK_ALIAS;
void SPI1_IRQHandler(void) CM_WEAK_ALIAS;
void SPI2_I2S2EXT_IRQHandler(void) CM_WEAK_ALIAS;
void USART1_IRQHandler(void) CM_WEAK_ALIAS;
void USART2_IRQHandler(void) CM_WEAK_ALIAS;
void USART3_IRQHandler(void) CM_WEAK_ALIAS;
void EXINT15_10_IRQHandler(void) CM_WEAK_ALIAS;
void RTCAlarm_IRQHandler(void) CM_WEAK_ALIAS;
void USBFSWakeUp_IRQHandler(void) CM_WEAK_ALIAS;
void TMR8_BRK_TMR12_IRQHandler(void) CM_WEAK_ALIAS;
void TMR8_OVF_TMR13_IRQHandler(void) CM_WEAK_ALIAS;
void TMR8_TRG_HALL_TMR14_IRQHandler(void) CM_WEAK_ALIAS;
void TMR8_CH_IRQHandler(void) CM_WEAK_ALIAS;
void ADC3_IRQHandler(void) CM_WEAK_ALIAS;
void XMC_IRQHandler(void) CM_WEAK_ALIAS;
void SDIO1_IRQHandler(void) CM_WEAK_ALIAS;
void TMR5_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void SPI3_I2S3EXT_IRQHandler(void) CM_WEAK_ALIAS;
void UART4_IRQHandler(void) CM_WEAK_ALIAS;
void UART5_IRQHandler(void) CM_WEAK_ALIAS;
void TMR6_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void TMR7_GLOBAL_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel1_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel2_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel3_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel4_5_IRQHandler(void) CM_WEAK_ALIAS;
void SDIO2_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_EVT_IRQHandler(void) CM_WEAK_ALIAS;
void I2C3_ERR_IRQHandler(void) CM_WEAK_ALIAS;
void SPI4_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_TX_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_RX0_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_RX1_IRQHandler(void) CM_WEAK_ALIAS;
void CAN2_SE_IRQHandler(void) CM_WEAK_ALIAS;
void ACC_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_MAPH_IRQHandler(void) CM_WEAK_ALIAS;
void USBFS_MAPL_IRQHandler(void) CM_WEAK_ALIAS;
void DMA2_Channel6_7_IRQHandler(void) CM_WEAK_ALIAS;
void USART6_IRQHandler(void) CM_WEAK_ALIAS;
void UART7_IRQHandler(void) CM_WEAK_ALIAS;
void UART8_IRQHandler(void) CM_WEAK_ALIAS;
void EMAC_IRQHandler(void) CM_WEAK_ALIAS;
void EMAC_WKUP_IRQHandler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(97) = {
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
  WWDT_IRQHandler,                 /*  16  IRQ 0 */
  PVM_IRQHandler,                  /*  17  IRQ 1 */
  TAMPER_IRQHandler,               /*  18  IRQ 2 */
  RTC_IRQHandler,                  /*  19  IRQ 3 */
  FLASH_IRQHandler,                /*  20  IRQ 4 */
  CRM_IRQHandler,                  /*  21  IRQ 5 */
  EXINT0_IRQHandler,               /*  22  IRQ 6 */
  EXINT1_IRQHandler,               /*  23  IRQ 7 */
  EXINT2_IRQHandler,               /*  24  IRQ 8 */
  EXINT3_IRQHandler,               /*  25  IRQ 9 */
  EXINT4_IRQHandler,               /*  26  IRQ 10 */
  DMA1_Channel1_IRQHandler,        /*  27  IRQ 11 */
  DMA1_Channel2_IRQHandler,        /*  28  IRQ 12 */
  DMA1_Channel3_IRQHandler,        /*  29  IRQ 13 */
  DMA1_Channel4_IRQHandler,        /*  30  IRQ 14 */
  DMA1_Channel5_IRQHandler,        /*  31  IRQ 15 */
  DMA1_Channel6_IRQHandler,        /*  32  IRQ 16 */
  DMA1_Channel7_IRQHandler,        /*  33  IRQ 17 */
  ADC1_2_IRQHandler,               /*  34  IRQ 18 */
  USBFS_H_CAN1_TX_IRQHandler,      /*  35  IRQ 19 */
  USBFS_L_CAN1_RX0_IRQHandler,     /*  36  IRQ 20 */
  CAN1_RX1_IRQHandler,             /*  37  IRQ 21 */
  CAN1_SE_IRQHandler,              /*  38  IRQ 22 */
  EXINT9_5_IRQHandler,             /*  39  IRQ 23 */
  TMR1_BRK_TMR9_IRQHandler,        /*  40  IRQ 24 */
  TMR1_OVF_TMR10_IRQHandler,       /*  41  IRQ 25 */
  TMR1_TRG_HALL_TMR11_IRQHandler,  /*  42  IRQ 26 */
  TMR1_CH_IRQHandler,              /*  43  IRQ 27 */
  TMR2_GLOBAL_IRQHandler,          /*  44  IRQ 28 */
  TMR3_GLOBAL_IRQHandler,          /*  45  IRQ 29 */
  TMR4_GLOBAL_IRQHandler,          /*  46  IRQ 30 */
  I2C1_EVT_IRQHandler,             /*  47  IRQ 31 */
  I2C1_ERR_IRQHandler,             /*  48  IRQ 32 */
  I2C2_EVT_IRQHandler,             /*  49  IRQ 33 */
  I2C2_ERR_IRQHandler,             /*  50  IRQ 34 */
  SPI1_IRQHandler,                 /*  51  IRQ 35 */
  SPI2_I2S2EXT_IRQHandler,         /*  52  IRQ 36 */
  USART1_IRQHandler,               /*  53  IRQ 37 */
  USART2_IRQHandler,               /*  54  IRQ 38 */
  USART3_IRQHandler,               /*  55  IRQ 39 */
  EXINT15_10_IRQHandler,           /*  56  IRQ 40 */
  RTCAlarm_IRQHandler,             /*  57  IRQ 41 */
  USBFSWakeUp_IRQHandler,          /*  58  IRQ 42 */
  TMR8_BRK_TMR12_IRQHandler,       /*  59  IRQ 43 */
  TMR8_OVF_TMR13_IRQHandler,       /*  60  IRQ 44 */
  TMR8_TRG_HALL_TMR14_IRQHandler,  /*  61  IRQ 45 */
  TMR8_CH_IRQHandler,              /*  62  IRQ 46 */
  ADC3_IRQHandler,                 /*  63  IRQ 47 */
  XMC_IRQHandler,                  /*  64  IRQ 48 */
  SDIO1_IRQHandler,                /*  65  IRQ 49 */
  TMR5_GLOBAL_IRQHandler,          /*  66  IRQ 50 */
  SPI3_I2S3EXT_IRQHandler,         /*  67  IRQ 51 */
  UART4_IRQHandler,                /*  68  IRQ 52 */
  UART5_IRQHandler,                /*  69  IRQ 53 */
  TMR6_GLOBAL_IRQHandler,          /*  70  IRQ 54 */
  TMR7_GLOBAL_IRQHandler,          /*  71  IRQ 55 */
  DMA2_Channel1_IRQHandler,        /*  72  IRQ 56 */
  DMA2_Channel2_IRQHandler,        /*  73  IRQ 57 */
  DMA2_Channel3_IRQHandler,        /*  74  IRQ 58 */
  DMA2_Channel4_5_IRQHandler,      /*  75  IRQ 59 */
  SDIO2_IRQHandler,                /*  76  IRQ 60 */
  I2C3_EVT_IRQHandler,             /*  77  IRQ 61 */
  I2C3_ERR_IRQHandler,             /*  78  IRQ 62 */
  SPI4_IRQHandler,                 /*  79  IRQ 63 */
  0,                               /*  80  IRQ 64 */
  0,                               /*  81  IRQ 65 */
  0,                               /*  82  IRQ 66 */
  0,                               /*  83  IRQ 67 */
  CAN2_TX_IRQHandler,              /*  84  IRQ 68 */
  CAN2_RX0_IRQHandler,             /*  85  IRQ 69 */
  CAN2_RX1_IRQHandler,             /*  86  IRQ 70 */
  CAN2_SE_IRQHandler,              /*  87  IRQ 71 */
  ACC_IRQHandler,                  /*  88  IRQ 72 */
  USBFS_MAPH_IRQHandler,           /*  89  IRQ 73 */
  USBFS_MAPL_IRQHandler,           /*  90  IRQ 74 */
  DMA2_Channel6_7_IRQHandler,      /*  91  IRQ 75 */
  USART6_IRQHandler,               /*  92  IRQ 76 */
  UART7_IRQHandler,                /*  93  IRQ 77 */
  UART8_IRQHandler,                /*  94  IRQ 78 */
  EMAC_IRQHandler,                 /*  95  IRQ 79 */
  EMAC_WKUP_IRQHandler,            /*  96  IRQ 80 */
};

#endif
