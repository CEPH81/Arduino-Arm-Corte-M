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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
