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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
