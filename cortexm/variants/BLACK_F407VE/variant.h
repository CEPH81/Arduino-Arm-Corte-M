/*
 * variant.h - STM32F407VET6 "Black" (F4VE)
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x1Fu     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   80

#define LED_BUILTIN        PA6
#define LED_BUILTIN2   PA7
#define LED_BUILTIN_ON     LOW
#define USER_BTN       PE4

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* Botones: K0 = PE4, K1 = PE3, WK_UP = PA0 (activo alto) */
#define USER_BTN2      PE3
#define WKUP_BTN       PA0
/* Flash SPI W25Q16 en SPI1: PB3 SCK, PB4 MISO, PB5 MOSI, PB0 CS */
#define FLASH_CS       PB0

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA1
#define A1  PA2
#define A2  PA3
#define A3  PA4
#define A4  PA5
#define A5  PB1
#define PIN_PWM1           PC6      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PC7
#define PIN_PWM3           PC8
#define PIN_TONE           PC9      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PB3
#define PIN_SPI_MISO       PB4
#define PIN_SPI_MOSI       PB5
#define PIN_SPI_SS         PB0
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB9      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB8
#define CM_EEPROM_ADDR     0x08060000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x20000UL
#define CM_FLASH_UNIT      4
#define CM_EEPROM_SECTOR   7

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
