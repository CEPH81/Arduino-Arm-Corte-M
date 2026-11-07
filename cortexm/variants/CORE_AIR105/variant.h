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

#ifdef __cplusplus
extern HardwareSerialCM Serial;
#endif

#endif
