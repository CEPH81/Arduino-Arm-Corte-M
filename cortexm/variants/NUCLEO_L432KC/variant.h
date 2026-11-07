/*
 * variant.h - Nucleo-32 L432KC
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x87u     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PB3
#define LED_BUILTIN_ON     HIGH

#define PIN_SERIAL_TX      PA2
#define PIN_SERIAL_RX      PA15

/* Rótulos del conector tipo Arduino Nano de la Nucleo-32 */
#define D0  PA10
#define D1  PA9
#define D2  PA12
#define D3  PB0
#define D4  PB7
#define D5  PB6
#define D6  PB1
#define D7  PC14
#define D8  PC15
#define D9  PA8
#define D10 PA11
#define D11 PB5
#define D12 PB4
#define D13 PB3
#define A0  PA0
#define A1  PA1
#define A2  PA3
#define A3  PA4
#define A4  PA5
#define A5  PA6
#define A6  PA7
#define A7  PA2   /* compartido con TX del ST-LINK (SB2) */

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define PIN_PWM1           PA8      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PA9
#define PIN_PWM3           PA10
#define PIN_TONE           PA11      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PB3
#define PIN_SPI_MISO       PB4
#define PIN_SPI_MOSI       PB5
#define PIN_SPI_SS         PA11
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB6
#define CM_EEPROM_ADDR     0x0803F800UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x800UL
#define CM_FLASH_UNIT      8

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
