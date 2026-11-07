/*
 * EEPROM.h - EEPROM emulada en la flash interna (Arduino-Arm-Cortex-M).
 *
 * API de AVR: EEPROM.read/write/update/get/put, EEPROM[i] y length().
 * Tamaño: 1024 bytes. Una página de flash reservada al final del chip
 * (CM_EEPROM_ADDR en variant.h, ya quitada del espacio del programa).
 *
 * Cómo gasta la flash: cada byte que cambia se añade como un registro nuevo
 * (dirección + valor) detrás de la última copia; solo cuando la página se
 * llena se borra y se escribe una copia limpia. Así una página de 2 KB
 * aguanta ~128 cambios por borrado, y una de 128 KB (STM32F4) ~16 000.
 * Escribir el mismo valor no gasta nada.
 *
 * Al borrar la página la CPU se detiene unos ms (hasta ~2 s en las páginas
 * de 128/256 KB de STM32F4/GD32F4). Un corte de luz justo en ese momento
 * borra los datos guardados.
 */
#ifndef CM_EEPROM_H
#define CM_EEPROM_H

#include <Arduino.h>

#if !defined(CM_EEPROM_ADDR)
#error "Esta placa no tiene EEPROM emulada (la variante no define CM_EEPROM_ADDR)"
#endif

#define CM_EEPROM_SIZE  1024

struct EERef {
  EERef(int i) : index(i) { }
  uint8_t operator*() const;
  operator uint8_t() const { return **this; }
  EERef &operator=(const EERef &r) { return *this = *r; }
  EERef &operator=(uint8_t v);
  EERef &operator+=(uint8_t v) { return *this = (uint8_t)(**this + v); }
  EERef &operator-=(uint8_t v) { return *this = (uint8_t)(**this - v); }
  EERef &operator|=(uint8_t v) { return *this = (uint8_t)(**this | v); }
  EERef &operator&=(uint8_t v) { return *this = (uint8_t)(**this & v); }
  EERef &operator++() { return *this += 1; }
  EERef &operator--() { return *this -= 1; }
  uint8_t operator++(int) { uint8_t r = **this; ++(*this); return r; }
  uint8_t operator--(int) { uint8_t r = **this; --(*this); return r; }
  EERef &update(uint8_t v) { return *this = v; }
  int index;
};

class EEPROMClass {
public:
  void     begin(size_t = 0) { load(); }       /* opcional (compatibilidad con ESP32) */
  bool     commit() { return _ok; }            /* opcional: cada write() ya se guarda */
  uint8_t  read(int idx);
  void     write(int idx, uint8_t val);
  void     update(int idx, uint8_t val) { write(idx, val); }
  EERef    operator[](int idx) { return EERef(idx); }
  uint16_t length() { return CM_EEPROM_SIZE; }
  bool     ok() { load(); return _ok; }        /* false si la flash dio error */
  void     readBytes(int idx, void *data, size_t len);
  bool     writeBytes(int idx, const void *data, size_t len);
  /* Borra la página y deja todo en 0xFF */
  bool     clear();

  template <typename T> T &get(int idx, T &t) { readBytes(idx, &t, sizeof(T)); return t; }
  template <typename T> const T &put(int idx, const T &t) { writeBytes(idx, &t, sizeof(T)); return t; }

private:
  void load();
  bool append(uint16_t addr, uint8_t val);
  bool compact();
  uint8_t  _cache[CM_EEPROM_SIZE];
  uint32_t _log = 0;          /* desplazamiento del siguiente registro libre */
  bool     _loaded = false;
  bool     _ok = true;
};

extern EEPROMClass EEPROM;

#endif
