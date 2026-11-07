/*
 * variant.cpp - GD32F427V-START
 * El reloj lo configura SystemInit() (system_gd32f4xx.c) según el menú
 * "Reloj de CPU"; aquí solo se declaran los puertos serie.
 * Nota: en GD32 los USART se numeran desde 0 (USART0 = USART1 de ST).
 */
#include "Arduino.h"

CM_SERIAL(Serial,  USART0, USART0_IRQn, USART0_IRQHandler, PA9, 7, PA10, 7)
CM_SERIAL(Serial1, USART1, USART1_IRQn, USART1_IRQHandler, PA2, 7, PA3,  7)
