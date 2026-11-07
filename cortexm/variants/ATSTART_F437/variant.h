/*
 * variant.h - Artery AT-START-F437 (AT32F437ZMT7, 4032 KB flash, 384 KB RAM)
 * Sirve también para otras placas AT32F435/437 (ajustar ldscript.ld).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0xFFu     /* GPIOA..GPIOH (LQFP144) */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PD13      /* LED2 rojo */
#define LED2_BUILTIN       PD14      /* LED3 amarillo */
#define LED3_BUILTIN       PD15      /* LED4 verde */
#define LED_BUILTIN_ON     LOW
#define USER_BTN           PA0

/* Serial = USART1 en PA9/PA10, conectado al puerto COM virtual del AT-Link */
#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA0
#define A1  PA1
#define A2  PA4
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
#define CM_SPI_HW          SPI1_BASE   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB9      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB8
#define CM_EEPROM_ADDR     0x083EF000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x1000UL
#define CM_FLASH_UNIT      4

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
