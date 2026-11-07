/*
 * variant.h - Placa de desarrollo Synwit SWM34SVET6 (serie SWM341,
 * Cortex-M33 con FPU, 512 KB flash, 64 KB SRAM + SDRAM interna).
 * Puertos: A, B, C, D, E, M y N (PM0..PM15 = 80..95, PN0..PN15 = 96..111).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x7Fu     /* A B C D E M N */
#define NUM_DIGITAL_PINS   112

#define LED_BUILTIN        PA9       /* LED de los ejemplos de Synwit (verificar) */
#define LED_BUILTIN_ON     HIGH

/* UART0 en PM0 (RX) / PM1 (TX), como los ejemplos oficiales */
#define PIN_SERIAL_RX      PM0
#define PIN_SERIAL_TX      PM1

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PC6
#define A1  PC5
#define A2  PC4
#define A3  PC1
#define A4  PC0
#define A5  PA15
#define PIN_PWM1           PB3      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB1
#define PIN_PWM3           PB15
#define PIN_TONE           PD9      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PB4
#define PIN_SPI_MISO       PB5
#define PIN_SPI_MOSI       PB6
#define PIN_SPI_SS         PB7
/* sin CM_SPI_HW: SPI por software en estos pines */
#define PIN_WIRE_SDA       PA0      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PA1
#define CM_EEPROM_ADDR     0x0007F000UL   /* página reservada en ldscript.ld */
#define CM_EEPROM_PAGE     0x1000UL
#define CM_FLASH_UNIT      16

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
