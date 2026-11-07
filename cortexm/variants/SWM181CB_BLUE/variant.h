/*
 * variant.h - Placa azul USB-C de 40 pines con Synwit SWM181CBT6
 * (Cortex-M0, 48 MHz, 64 KB flash, 16 KB RAM). LEDs en PD0, PB9 y PB8.
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x1Fu     /* GPIOA..GPIOE */
#define NUM_DIGITAL_PINS   80

#define LED_BUILTIN        PD0
#define LED2_BUILTIN       PB9
#define LED3_BUILTIN       PB8
#define LED_BUILTIN_ON     HIGH      /* verificar */

/* UART0 por FUNMUX (como los ejemplos de Synwit). Verificar a qué pines
 * va el conversor USB-serie de tu placa y cambiarlos en variant.cpp. */
#define PIN_SERIAL_RX      PA0
#define PIN_SERIAL_TX      PA1

/* ---- Periféricos (ejemplos y bibliotecas) ---------------------------- */
#define A0  PA12
#define A1  PA15
#define A2  PC6
#define A3  PC7
#define PIN_PWM1           PB9      /* PWM por hardware (LED RGB, Fade) */
#define PIN_PWM2           PB8
#define PIN_PWM3           PB10
#define PIN_TONE           PB11      /* tone(): zumbador pasivo */
#define PIN_SPI_SCK        PB4
#define PIN_SPI_MISO       PB5
#define PIN_SPI_MOSI       PB6
#define PIN_SPI_SS         PB7
/* sin CM_SPI_HW: SPI por software en estos pines */
#define PIN_WIRE_SDA       PB2      /* I2C por software (cualquier pin) */
#define PIN_WIRE_SCL       PB3
/* sin CM_EEPROM_ADDR: la EEPROM emulada no está disponible */

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
