/*
 * variant.h - WeAct STM32WB55CGU6
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x97u     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PE4
#define LED_BUILTIN_ON     HIGH

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* La flash se limita a 512 KB: la mitad superior queda para el
 * firmware del radio (CPU2) si más adelante se usa BLE. */

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA0
#define A1  PA1
#define A2  PA2
#define A3  PA3
#define PIN_PWM1           PA8      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PA15
#define PIN_PWM3           PB3
#define PIN_TONE           PB4      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB9      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB8
#define CM_EEPROM_ADDR     0x0807F000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x1000UL
#define CM_FLASH_UNIT      8

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
