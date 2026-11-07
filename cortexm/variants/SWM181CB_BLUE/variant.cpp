/*
 * variant.cpp - SWM181CBT6
 * Reloj: SystemInit() de Synwit (RC interno de 48 MHz por defecto; se
 * elige en Herramientas > Reloj de CPU). No requiere cristal.
 * Interrupciones: el SWM181 enruta cada periférico a IRQ0..IRQ31 con
 * IRQ_Connect(). Serial usa IRQ0 (fuente IRQ0_15_UART0).
 */
#include "Arduino.h"

#define CM_SWM181_IRQ(slot, src)  (((src) << 8) | (slot))

CM_SERIAL(Serial, UART0, CM_SWM181_IRQ(IRQ0_IRQ, IRQ0_15_UART0), cm_uart0_isr,
          PA1, FUNMUX_UART0_TXD, PA0, FUNMUX_UART0_RXD)
extern "C" void IRQ0_Handler(void) __attribute__((alias("cm_uart0_isr")));
