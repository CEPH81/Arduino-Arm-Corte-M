/*
 * variant.h - WeAct STM32G431CBU6
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x67u     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   112

#define LED_BUILTIN        PC6
#define LED_BUILTIN_ON     HIGH
#define USER_BTN       PC13

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA0
#define A1  PA1
#define A2  PA2
#define A3  PA3
#define A4  PB0
#define A5  PB1
#define PIN_PWM1           PB4      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB5
#define PIN_PWM3           PB6
#define PIN_TONE           PB10      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PA15
#define CM_EEPROM_ADDR     0x0801F800UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x800UL
#define CM_FLASH_UNIT      8

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
