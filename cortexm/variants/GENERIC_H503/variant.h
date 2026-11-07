/*
 * variant.h - STM32H503 genérica
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x8Fu     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PA5
#define LED_BUILTIN_ON     HIGH
#define USER_BTN       PC13

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10


#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
