/*
 * wiring_analog.cpp - analogRead/analogWrite/tone comunes a todas las
 * familias. El acceso al hardware está en hal/analog_<familia>.c
 * (cmAdcRead, cmPwmWrite, cmTimerStart...).
 *
 *  analogRead()   10 bits por defecto (como Arduino UNO); analogReadResolution(12)
 *                 da el valor completo del ADC (0..4095).
 *  analogWrite()  8 bits a 1 kHz por defecto; analogWriteResolution(1..16) y
 *                 analogWriteFrequency(Hz). Un pin sin PWM queda en LOW o HIGH.
 *  tone()         PWM al 50 % en pines con PWM; en los demás, el temporizador
 *                 de cmTimerStart() conmuta el pin (un tono a la vez).
 */
#include "Arduino.h"

static uint8_t  rd_bits = 10;
static uint8_t  wr_bits = 8;
static uint32_t wr_freq = 1000;

void analogReference(uint8_t mode) { (void)mode; }   /* solo hay AR_DEFAULT (VDDA) */
void analogReadResolution(int bits)  { if (bits >= 1 && bits <= 16) rd_bits = (uint8_t)bits; }
void analogWriteResolution(int bits) { if (bits >= 1 && bits <= 16) wr_bits = (uint8_t)bits; }
void analogWriteFrequency(uint32_t hz) { if (hz) wr_freq = hz; }

int analogRead(pin_size_t pin)
{
  int v = cmAdcRead(pin);
  if (v < 0) return 0;
  return (rd_bits <= CM_ADC_BITS) ? (v >> (CM_ADC_BITS - rd_bits)) : (v << (rd_bits - CM_ADC_BITS));
}

uint32_t analogReadMillivolts(pin_size_t pin)
{
  int v = cmAdcRead(pin);
  if (v < 0) return 0;
  return ((uint32_t)v * cmAdcRefMv() + 2047u) / 4095u;
}

int32_t analogReadTemperature(void) { return cmAdcTemperature(); }

void analogWrite(pin_size_t pin, int value)
{
  uint32_t max = (1UL << wr_bits) - 1u;
  uint32_t v = value < 0 ? 0u : (uint32_t)value;
  if (v >= max) v = max + 1u;                     /* 100 % */
  if (!cmPwmWrite(pin, wr_freq, v, wr_bits)) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, v > (max >> 1) ? HIGH : LOW);
  }
}

/* ---------------------------------------------------------------- tone */
static uint8_t           tone_pin = CM_PIN_NONE;
static bool              tone_sw;
static volatile uint32_t tone_ms;

static void tone_toggle(void) { cm_gpio_toggle(tone_pin); }

static void tone_tick(void)
{
  if (tone_ms && --tone_ms == 0) noTone(tone_pin);
}

void tone(uint8_t pin, unsigned int freq, unsigned long duration)
{
  if (!cmPinValid(pin)) return;
  if (freq == 0) { noTone(pin); return; }
  if (tone_pin != CM_PIN_NONE && tone_pin != pin) noTone(tone_pin);
  cm_tick_hook = 0;
  tone_pin = pin;
  if (cmPwmWrite(pin, freq, 128u, 8u)) {
    tone_sw = false;
  } else {
    pinMode(pin, OUTPUT);
    tone_sw = true;
    uint32_t half = 500000UL / freq;
    if (!cmTimerStart(half ? half : 1u, tone_toggle)) { tone_pin = CM_PIN_NONE; return; }
  }
  tone_ms = duration;
  if (duration) cm_tick_hook = tone_tick;
}

void noTone(uint8_t pin)
{
  if (pin != tone_pin || tone_pin == CM_PIN_NONE) return;
  cm_tick_hook = 0;
  if (tone_sw) cmTimerStop(); else cmPwmStop(pin);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  tone_pin = CM_PIN_NONE;
}
