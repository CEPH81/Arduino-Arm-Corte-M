/*
 * cortexm_device.h - Selección del header oficial del fabricante y de los
 * "estilos" de periférico que usa cada familia.
 *
 * boards.txt define la familia y el chip (p.ej. -DSTM32F4xx -DSTM32F407xx).
 * Las direcciones y registros vienen siempre del header del fabricante.
 *
 * Muchos chips ARM chinos copian el diseño de los periféricos de ST, así que
 * el núcleo agrupa por "estilo" y no por marca:
 *
 *   CM_GPIO_STYLE   1 = MODER/OTYPER/.../BSRR/AFR (STM32 F4/L4/G4/WB/H5, GD32F4, AT32F43x)
 *                   2 = CRL/CRH (STM32F1, AT32F403A)
 *                   3 = HC32 PORT (PCR/PFSR por pin)
 *                   4 = Synwit (GPIO DATA/DIR + PORT FUNC)
 *   CM_USART_STYLE  1 = SR/DR/BRR/CR1 (STM32F4, GD32F4, AT32)
 *                   2 = ISR/ICR/RDR/TDR (STM32 L4/G4/WB/H5)
 *                   3 = HC32 USART
 *                   4 = Synwit UART
 */
#ifndef CORTEXM_DEVICE_H
#define CORTEXM_DEVICE_H

/* ------------------------------ ST ---------------------------------- */
#if defined(STM32F4xx) || defined(STM32L4xx) || defined(STM32G4xx) || defined(STM32WBxx) || defined(STM32H5xx)
  #define CM_VENDOR_ST 1
  #if defined(STM32F4xx)
    #include "stm32f4xx.h"
  #elif defined(STM32L4xx)
    #include "stm32l4xx.h"
  #elif defined(STM32G4xx)
    #include "stm32g4xx.h"
  #elif defined(STM32WBxx)
    #include "stm32wbxx.h"
  #else
    #include "stm32h5xx.h"
  #endif
  #define CM_GPIO_STYLE   1
  #define CM_GPIO_BASE0   GPIOA_BASE
  #define CM_GPIO_STRIDE  0x400UL
  #if defined(STM32F4xx)
    #define CM_USART_STYLE 1
  #else
    #define CM_USART_STYLE 2
  #endif

/* ---------------------------- GigaDevice ---------------------------- */
#elif defined(GD32F4XX)
  #define CM_VENDOR_GD 1
  #include "gd32f4xx.h"
  #define CM_GPIO_STYLE   1
  #define CM_GPIO_BASE0   GPIO_BASE           /* GPIOA */
  #define CM_GPIO_STRIDE  0x400UL
  #define CM_USART_STYLE  1

/* ------------------------------ Artery ------------------------------ */
#elif defined(AT32F435_437)
  #define CM_VENDOR_AT 1
  #include "at32f435_437.h"
  #define CM_GPIO_STYLE   1
  #define CM_GPIO_BASE0   GPIOA_BASE
  #define CM_GPIO_STRIDE  0x400UL
  #define CM_USART_STYLE  1
#elif defined(AT32F403A_407)
  #define CM_VENDOR_AT 1
  #include "at32f403a_407.h"
  #define CM_GPIO_STYLE   2
  #define CM_GPIO_BASE0   GPIOA_BASE
  #define CM_GPIO_STRIDE  0x400UL
  #define CM_USART_STYLE  1

/* ------------------------------ HDSC -------------------------------- */
#elif defined(HC32F460)
  #define CM_VENDOR_HC 1
  #include "hc32_common.h"          /* incluye hc32f460.h */
  #include "hc32f460_clk.h"
  #include "hc32f460_efm.h"
  #include "hc32f460_sram.h"
  #include "hc32f460_gpio.h"
  #include "hc32f460_usart.h"
  #include "hc32f460_pwc.h"
  #define CM_GPIO_STYLE   3
  #define CM_USART_STYLE  3

/* ------------------------------ Synwit ------------------------------ */
#elif defined(CHIP_SWM181)
  #define CM_VENDOR_SWM 1
  #ifdef __cplusplus
  extern "C" {                      /* los headers de Synwit no traen extern "C" */
  #endif
  #include "SWM181.h"
  #ifdef __cplusplus
  }
  #endif
  #define CM_GPIO_STYLE   4
  #define CM_USART_STYLE  4
#elif defined(CHIP_SWM341)
  #define CM_VENDOR_SWM 1
  #ifdef __cplusplus
  extern "C" {
  #endif
  #include "SWM341.h"
  #ifdef __cplusplus
  }
  #endif
  #define CM_GPIO_STYLE   4
  #define CM_USART_STYLE  4

#else
  #error "Familia de microcontrolador no soportada todavía por este núcleo"
#endif

/* AT32 usa nombres propios para el reloj del sistema */
#if defined(CM_VENDOR_AT)
  #ifndef SystemCoreClockUpdate
    #define SystemCoreClockUpdate system_core_clock_update
  #endif
#endif

#endif /* CORTEXM_DEVICE_H */
