/*
 * variant.cpp - SWM34SVET6
 * Reloj: SystemInit() de Synwit. Por defecto PLL desde el cristal de
 * 12 MHz (12 / 3 * 4 * 60 / 8 = 120 MHz). Opción segura: RC interno.
 */
#include "Arduino.h"

CM_SERIAL(Serial, UART0, UART0_IRQn, UART0_Handler, PM1, PORTM_PIN1_UART0_TX, PM0, PORTM_PIN0_UART0_RX)
