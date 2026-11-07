/*
 * NTC_Thermistor.h - Termistor NTC con la ecuacion Beta.
 *
 * Divisor por defecto (NTC_HIGH_SIDE): 3V3 -> NTC -> pin -> R fija -> GND.
 * La tensión se lee con analogReadMillivolts() (0 a cmAdcRefMv() mV:
 * 3300 mV en casi todas las placas, 3760 mV en el Air105).
 */
#ifndef NTC_THERMISTOR_H
#define NTC_THERMISTOR_H

#include <Arduino.h>

enum NTC_Topology {
    NTC_HIGH_SIDE,   /* 3V3 -> NTC -> pin -> R fija -> GND   (como el original) */
    NTC_LOW_SIDE     /* 3V3 -> R fija -> pin -> NTC -> GND */
};

class NTC_Thermistor
{
public:
    NTC_Thermistor(pin_size_t pin, float rFixed = 10000.0f, float r0 = 10000.0f,
                   float beta = 3950.0f, float vcc = 3.3f, NTC_Topology topo = NTC_HIGH_SIDE)
        : _pin(pin), _rFixed(rFixed), _r0(r0), _beta(beta), _vccMv(vcc * 1000.0f), _topo(topo) { }

    void  begin(void) { pinMode(_pin, INPUT); }
    float readCelsius(uint8_t samples = 32);   /* promedia 'samples' lecturas; NAN si satura */
    float readResistance(uint8_t samples = 32);

private:
    pin_size_t   _pin;
    float        _rFixed, _r0, _beta, _vccMv;
    NTC_Topology _topo;
};

#endif
