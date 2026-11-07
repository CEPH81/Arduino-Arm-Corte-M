/*
 * variant.h - GigaDevice GD32F427V-START (GD32F427VKT6, 3 MB flash, 256 KB RAM)
 * Número de pin = puerto*16 + bit  (usar PA0, PB3, ...).
 * Verificar en el esquema de tu placa: LED y botón según la guía de usuario.
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x1Fu     /* GPIOA..GPIOE (LQFP100) */
#define NUM_DIGITAL_PINS   80

#define LED_BUILTIN        PC6       /* LED1 de la START (verificar) */
#define LED_BUILTIN_ON     HIGH
#define USER_BTN           PA0       /* tecla Wakeup/User (verificar) */

/* Serial = USART0 en PA9 (TX) / PA10 (RX), el que va al GD-Link */
#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA1
#define A1  PA2
#define A2  PA3
#define A3  PC0
#define A4  PC1
#define A5  PC2
#define PIN_PWM1           PC7      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PC8
#define PIN_PWM3           PC9
#define PIN_TONE           PB10      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PA5
#define PIN_SPI_MISO       PA6
#define PIN_SPI_MOSI       PA7
#define PIN_SPI_SS         PA4
#define CM_SPI_HW          0x40013000UL   /* SPI por hardware */
#define CM_SPI_AF          5
#define PIN_WIRE_SDA       PB7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB6
#define CM_EEPROM_ADDR     0x082C0000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x40000UL
#define CM_FLASH_UNIT      4
#define CM_EEPROM_SECTOR   15

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
