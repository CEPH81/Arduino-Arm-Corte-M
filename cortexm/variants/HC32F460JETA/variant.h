/*
 * variant.h - Placa de desarrollo HC32F460JETA (HDSC, Cortex-M4F 200 MHz,
 * 512 KB flash, 192 KB RAM, encapsulado de 48 pines).
 * Pines: PA0-PA15, PB0-PB15 (según encapsulado), PC13-PC15, PH0-PH2.
 * Verificar LED y pines del USB-serie en el esquema de tu placa.
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x87u     /* A, B, C y H (índice 7) */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PC13      /* verificar */
#define LED_BUILTIN_ON     LOW
#define USER_BTN           PA0       /* verificar */

/* El HC32 puede llevar el USART a casi cualquier pin (funciones 32-39) */
#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
