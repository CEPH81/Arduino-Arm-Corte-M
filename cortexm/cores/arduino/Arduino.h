/*
 * Arduino.h - Núcleo Arduino-Arm-Cortex-M (CEPH81)
 * API estándar de Arduino (ArduinoCore-API) + capa propia por familia.
 */
#ifndef ARDUINO_H
#define ARDUINO_H

#include "api/ArduinoAPI.h"
#include "cortexm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define interrupts()      __enable_irq()
#define noInterrupts()    __disable_irq()

#define clockCyclesPerMicrosecond()  (SystemCoreClock / 1000000UL)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())

#define digitalPinToInterrupt(p)     (p)
#define digitalPinToPort(p)          cmPinPortIndex(p)
#define digitalPinToBitMask(p)       cmPinMask(p)
#define portOutputRegister(port)     CM_PORT_OUT_REG(port)
#define portInputRegister(port)      CM_PORT_IN_REG(port)
#define NOT_A_PIN                    CM_PIN_NONE
#define NOT_AN_INTERRUPT             (-1)

void cmTimeInit(void);

/* Extensiones de analog*: wiring_analog.cpp */
#define AR_DEFAULT  0
void     analogReadResolution(int bits);
void     analogWriteResolution(int bits);
void     analogWriteFrequency(uint32_t hz);
uint32_t analogReadMillivolts(pin_size_t pin);
int32_t  analogReadTemperature(void);       /* centésimas de °C; CM_TEMP_NONE si no hay */

#ifdef __cplusplus
} /* extern "C" */

using namespace arduino;
#include "HardwareSerial_CM.h"
#endif

/* Definiciones de la placa: pines, LED, Serial, número de pines... */
#include "variant.h"

#if !defined(NUM_DIGITAL_PINS) || !defined(CM_VALID_PORTS)
#error "La variante debe definir NUM_DIGITAL_PINS y CM_VALID_PORTS"
#endif

#endif /* ARDUINO_H */
