/*
 * variant.h - WeAct STM32WB55CGU6
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x97u     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PE4
#define LED_BUILTIN_ON     HIGH

#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

/* La flash se limita a 512 KB: la mitad superior queda para el
 * firmware del radio (CPU2) si más adelante se usa BLE. */

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
