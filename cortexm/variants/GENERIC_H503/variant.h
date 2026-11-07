/*
 * variant.h - STM32H503 genérica
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x8Fu     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PA5
#define LED_BUILTIN_ON     HIGH
#define USER_BTN       PC13

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA0
#define A1  PA1
#define A2  PA3
#define A3  PB0
#define A4  PC1
#define A5  PC0
#define PIN_PWM1           PC6      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PC7
#define PIN_PWM3           PC8
#define PIN_TONE           PB10      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
/* sin CM_SPI_HW: SPI por software en estos pines */
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB6
#define CM_EEPROM_ADDR     0x0801E000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x2000UL
#define CM_FLASH_UNIT      16

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
