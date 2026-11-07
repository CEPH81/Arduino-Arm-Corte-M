/*
 * variant.h - Placa CORE-Air105 V1.0 de Hezhou (LuatOS): MegaHunt Air105
 * (MH1903S), Cortex-M4F a 204 MHz, 640 KB SRAM, 4 MB flash QSPI.
 * Puertos A-F (PF0..PF15 = 80..95). Carga por el USB-serie (UART0).
 * Ojo: al cargar un programa Arduino se borra LuatOS (se recupera con Luatools).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x3Fu     /* A B C D E F */
#define NUM_DIGITAL_PINS   96

/* LEDs de la placa (D1, D2, D3), encienden en alto */
#define LED_BUILTIN        PD14
#define LED_BUILTIN_ON     HIGH
#define LED_D1             PD14
#define LED_D2             PD15
#define LED_D3             PC3       /* comparte pata con SWDIO */
#define PIN_BOOT           PA10      /* botón BOOT (verificar el nivel al pulsar) */
#define USER_BTN           PA10

/* UART0 en PA0 (RX) / PA1 (TX): va al conversor USB-serie de la placa */
#define PIN_SERIAL_RX      PA0
#define PIN_SERIAL_TX      PA1

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PC0
#define A1  PC1
#define A2  PC4
#define A3  PC5
#define PIN_PWM1           PB0      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB1
#define PIN_PWM3           PC6
#define PIN_TONE           PC7      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PB2
#define PIN_SPI_MISO       PB5
#define PIN_SPI_MOSI       PB4
#define PIN_SPI_SS         PB3
/* sin CM_SPI_HW: SPI por software en estos pines */
#define PIN_WIRE_SDA       PE7      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PE6
#define CM_EEPROM_ADDR     0x013FF000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x1000UL
#define CM_FLASH_UNIT      4

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
