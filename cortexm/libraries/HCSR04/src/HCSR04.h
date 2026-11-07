/*
 * HCSR04.h - Sensor ultrasonico HC-SR04.
 * Portado de la biblioteca HCSR04_CH579 (núcleo CH579 de CEPH81).
 *
 * El ancho del pulso Echo se mide con micros() (SysTick): no ocupa
 * temporizadores. Tras el disparo se esperan 300 us ("blanking") para no
 * leer el final de la propia ráfaga.
 *
 *   HCSR04 sensor(PIN_TRIG, PIN_ECHO);       // Trig, Echo
 *   sensor.begin();
 *   uint16_t cm = sensor.readDistanceCm();   // HCSR04_NO_ECHO si no hubo eco
 *
 * Echo de un modulo a 5 V: use un divisor 1k/2k (estos chips trabajan a 3.3 V).
 */
#ifndef CM_HCSR04_H
#define CM_HCSR04_H

#include <Arduino.h>

#define HCSR04_NO_ECHO  0xFFFFu

class HCSR04
{
public:
    HCSR04(pin_size_t trigPin, pin_size_t echoPin) : _trig(trigPin), _echo(echoPin) { }

    void begin(void);

    /* Distancia en cm, o HCSR04_NO_ECHO si no hubo eco (fuera de rango,
     * sensor desconectado o cableado erroneo). */
    uint16_t readDistanceCm(void);

    /* Ancho del pulso Echo en microsegundos, o 0 si hubo timeout.
     * stage (opcional): 0 = timeout esperando la subida, 1 = esperando la
     * bajada, 2 = medicion correcta. */
    uint32_t readEchoUs(uint8_t *stage = nullptr);

private:
    pin_size_t _trig, _echo;
};

#endif
