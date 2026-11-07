/*
 * variant.cpp - CORE-Air105
 * Serial = UART0 (PA1 TX, PA0 RX, función 0 del multiplexor de patas).
 */
#include "Arduino.h"

CM_SERIAL(Serial, UART0, UART0_IRQn, UART0_IRQHandler, PA1, 0, PA0, 0)
