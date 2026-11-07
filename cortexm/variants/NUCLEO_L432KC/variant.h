/*
 * variant.h - Nucleo-32 L432KC
 * Número de pin = puerto*16 + bit  (usar los nombres PA0, PB3, ... o D0..Dn).
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x87u     /* bit n = GPIO n existe */
#define NUM_DIGITAL_PINS   128

#define LED_BUILTIN        PB3
#define LED_BUILTIN_ON     HIGH

#define PIN_SERIAL_TX      PA2
#define PIN_SERIAL_RX      PA15

/* Rótulos del conector tipo Arduino Nano de la Nucleo-32 */
#define D0  PA10
#define D1  PA9
#define D2  PA12
#define D3  PB0
#define D4  PB7
#define D5  PB6
#define D6  PB1
#define D7  PC14
#define D8  PC15
#define D9  PA8
#define D10 PA11
#define D11 PB5
#define D12 PB4
#define D13 PB3
#define A0  PA0
#define A1  PA1
#define A2  PA3
#define A3  PA4
#define A4  PA5
#define A5  PA6
#define A6  PA7
#define A7  PA2   /* compartido con TX del ST-LINK (SB2) */

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
