/*
 * EEPROM.cpp - EEPROM emulada (Arduino-Arm-Cortex-M).
 *
 * Página:  [cabecera][copia de 1024 bytes][registros...]
 *   cabecera  una unidad de escritura con la marca "CMEE": se escribe la
 *             última, cuando la copia ya está completa.
 *   registro  una unidad de escritura (4, 8 o 16 bytes, CM_FLASH_UNIT):
 *             dirección (16 bits), valor, comprobación; el resto en 0xFF.
 */
#include "EEPROM.h"

EEPROMClass EEPROM;

#define UNIT    ((uint32_t)(CM_FLASH_UNIT < 4 ? 4 : CM_FLASH_UNIT))
#define PAGE    ((uint32_t)CM_EEPROM_PAGE)
#define BASE    ((uint32_t)CM_EEPROM_ADDR)
#define SNAP    UNIT                                   /* desplazamiento de la copia */
#define LOG0    (SNAP + CM_EEPROM_SIZE)                /* primer registro */
#define MAGIC   0x45454D43UL                           /* "CMEE" */

static_assert(LOG0 + 64u * UNIT <= PAGE, "página de EEPROM demasiado chica");

static inline uint8_t chk(uint16_t a, uint8_t v) { return (uint8_t)(a ^ (a >> 8) ^ v ^ 0xA5u); }
static inline uint32_t rd32(uint32_t off) { return *(const volatile uint32_t *)(BASE + off); }

void EEPROMClass::load()
{
  if (_loaded) return;
  _loaded = true;
  memset(_cache, 0xFF, sizeof _cache);
  _log = LOG0;
  if (rd32(0) != MAGIC) {
    /* página nueva (borrada) o copia a medias: se empieza de cero */
    bool blank = true;
    for (uint32_t o = 0; o < LOG0 && blank; o += 4u) blank = rd32(o) == 0xFFFFFFFFu;
    if (!blank) _ok = compact();
    return;
  }
  memcpy(_cache, (const void *)(BASE + SNAP), CM_EEPROM_SIZE);
  for (; _log + UNIT <= PAGE; _log += UNIT) {
    uint32_t w = rd32(_log);
    if (w == 0xFFFFFFFFu) break;                      /* fin del registro */
    uint16_t a = (uint16_t)w;
    uint8_t v = (uint8_t)(w >> 16), c = (uint8_t)(w >> 24);
    if (c == chk(a, v) && a < CM_EEPROM_SIZE) _cache[a] = v;
  }
}

bool EEPROMClass::compact()
{
  uint32_t hdr[UNIT / 4];
  memset(hdr, 0xFF, sizeof hdr);
  hdr[0] = MAGIC;
  bool ok = cmFlashErase(BASE)
         && cmFlashWrite(BASE + SNAP, _cache, CM_EEPROM_SIZE)
         && cmFlashWrite(BASE, hdr, UNIT);
  _log = LOG0;
  return ok;
}

bool EEPROMClass::append(uint16_t a, uint8_t v)
{
  if (_log + UNIT > PAGE) return compact();          /* la copia ya lleva el valor nuevo */
  uint32_t rec[UNIT / 4];
  memset(rec, 0xFF, sizeof rec);
  rec[0] = (uint32_t)a | ((uint32_t)v << 16) | ((uint32_t)chk(a, v) << 24);
  bool ok = cmFlashWrite(BASE + _log, rec, UNIT);
  _log += UNIT;
  return ok;
}

uint8_t EEPROMClass::read(int idx)
{
  load();
  if (idx < 0 || idx >= CM_EEPROM_SIZE) return 0xFF;
  return _cache[idx];
}

void EEPROMClass::write(int idx, uint8_t val)
{
  load();
  if (idx < 0 || idx >= CM_EEPROM_SIZE || _cache[idx] == val) return;
  _cache[idx] = val;
  _ok = append((uint16_t)idx, val) && _ok;
}

void EEPROMClass::readBytes(int idx, void *data, size_t len)
{
  uint8_t *p = (uint8_t *)data;
  for (size_t i = 0; i < len; i++) p[i] = read(idx + (int)i);
}

bool EEPROMClass::writeBytes(int idx, const void *data, size_t len)
{
  const uint8_t *p = (const uint8_t *)data;
  for (size_t i = 0; i < len; i++) write(idx + (int)i, p[i]);
  return _ok;
}

bool EEPROMClass::clear()
{
  load();
  memset(_cache, 0xFF, sizeof _cache);
  _ok = compact();
  return _ok;
}

uint8_t EERef::operator*() const { return EEPROM.read(index); }
EERef &EERef::operator=(uint8_t v) { EEPROM.write(index, v); return *this; }
