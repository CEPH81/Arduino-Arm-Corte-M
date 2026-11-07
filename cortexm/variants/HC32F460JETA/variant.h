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

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA1
#define A1  PA2
#define A2  PA3
#define A3  PB0
#define A4  PB1
#define A5  PC0
#define PIN_PWM1           PB13      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB14
#define PIN_PWM3           PB15
#define PIN_TONE           PA8      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
/* sin CM_SPI_HW: SPI por software en estos pines */
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB6
#define CM_EEPROM_ADDR     0x0007E000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x2000UL
#define CM_FLASH_UNIT      4

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
