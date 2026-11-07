/*
 * cortexm_stdio.cpp - printf()/puts() de C salen por Serial (después de
 * Serial.begin()). Antes de begin() el texto se descarta.
 */
#include "Arduino.h"

extern "C" int _write(int fd, const char *b, int n)
{
  if ((fd == 1 || fd == 2) && Serial) Serial.write((const uint8_t *)b, (size_t)n);
  return n;
}
