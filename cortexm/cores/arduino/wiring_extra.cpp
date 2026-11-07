/*
 * wiring_extra.cpp - shiftIn/shiftOut y pulseIn genéricos.
 */
#include "Arduino.h"

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val)
{
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t b = (bitOrder == LSBFIRST) ? (val >> i) & 1u : (val >> (7 - i)) & 1u;
    digitalWrite(dataPin, b ? HIGH : LOW);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder)
{
  uint8_t v = 0;
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(clockPin, HIGH);
    if (bitOrder == LSBFIRST) v |= (uint8_t)(digitalRead(dataPin) << i);
    else                      v |= (uint8_t)(digitalRead(dataPin) << (7 - i));
    digitalWrite(clockPin, LOW);
  }
  return v;
}

unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  if (!cmPinValid(pin)) return 0;
  uint32_t want = state ? 1u : 0u;
  uint32_t t0 = micros();
  while (cm_gpio_read(pin) == want)  if (micros() - t0 > timeout) return 0;  /* pulso previo */
  while (cm_gpio_read(pin) != want)  if (micros() - t0 > timeout) return 0;  /* espera inicio */
  uint32_t start = micros();
  while (cm_gpio_read(pin) == want)  if (micros() - t0 > timeout) return 0;  /* mide */
  return micros() - start;
}

unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  return pulseInLong(pin, state, timeout);
}
