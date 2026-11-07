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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
