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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
