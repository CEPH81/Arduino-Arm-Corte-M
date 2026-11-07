/*
 * variant.h - WeAct AT32F403ACGU7 (formato "Black Pill", 1 MB flash, 96 KB RAM)
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x0Fu     /* GPIOA..GPIOD (QFN48: PA, PB, PC13-15, PD0-1) */
#define NUM_DIGITAL_PINS   64

#define LED_BUILTIN        PC13      /* LED azul (verificar) */
#define LED_BUILTIN_ON     LOW
#define USER_BTN           PA0       /* tecla KEY */

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA1
#define A1  PA2
#define A2  PA3
#define A3  PB0
#define PIN_PWM1           PB8      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB9
#define PIN_PWM3           PA8
#define PIN_TONE           PB1      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          0
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB6
#define CM_EEPROM_ADDR     0x080FF800UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x800UL
#define CM_FLASH_UNIT      4

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
