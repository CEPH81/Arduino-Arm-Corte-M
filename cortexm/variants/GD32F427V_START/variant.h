/*
 * variant.h - GigaDevice GD32F427V-START (GD32F427VKT6, 3 MB flash, 256 KB RAM)
 * Número de pin = puerto*16 + bit  (usar PA0, PB3, ...).
 * Verificar en el esquema de tu placa: LED y botón según la guía de usuario.
 */
#ifndef VARIANT_H
#define VARIANT_H

#define CM_VALID_PORTS     0x1Fu     /* GPIOA..GPIOE (LQFP100) */
#define NUM_DIGITAL_PINS   80

#define LED_BUILTIN        PC6       /* LED1 de la START (verificar) */
#define LED_BUILTIN_ON     HIGH
#define USER_BTN           PA0       /* tecla Wakeup/User (verificar) */

/* Serial = USART0 en PA9 (TX) / PA10 (RX), el que va al GD-Link */
#define PIN_SERIAL_TX      PA9
#define PIN_SERIAL_RX      PA10

#ifdef __cplusplus
extern HardwareSerialCM Serial;
extern HardwareSerialCM Serial1;
#endif

#endif
