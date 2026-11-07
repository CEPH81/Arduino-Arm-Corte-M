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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
