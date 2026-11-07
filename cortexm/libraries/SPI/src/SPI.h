/*
 * SPI.h - SPI maestro para el núcleo Arduino-Arm-Cortex-M.
 *
 * Pines: PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI y PIN_SPI_SS de la variante
 * (SS lo maneja el sketch). Si la variante define CM_SPI_HW se usa el SPI1
 * del chip (registros estilo ST: STM32 F4/L4/G4/WB, GD32F4, AT32); si no,
 * el SPI se genera por software en esos mismos pines (hasta ~2-8 MHz según
 * el chip). Modos 0-3, MSB/LSB primero.
 */
#ifndef CM_SPI_H
#define CM_SPI_H

#include <Arduino.h>
#include "api/HardwareSPI.h"

#ifndef SS
#define SS    PIN_SPI_SS
#define MOSI  PIN_SPI_MOSI
#define MISO  PIN_SPI_MISO
#define SCK   PIN_SPI_SCK
#endif

class CortexMSPI : public arduino::HardwareSPI
{
public:
  CortexMSPI(pin_size_t sck, pin_size_t miso, pin_size_t mosi);
  void begin() override;
  void end() override;
  void beginTransaction(arduino::SPISettings settings) override;
  void endTransaction(void) override { }
  uint8_t  transfer(uint8_t data) override;
  uint16_t transfer16(uint16_t data) override;
  void     transfer(void *buf, size_t count) override;
  void     transfer(const void *txbuf, void *rxbuf, size_t count);
  void usingInterrupt(int) override { }
  void notUsingInterrupt(int) override { }
  void attachInterrupt() override { }
  void detachInterrupt() override { }

  /* API clásica de AVR */
  void setBitOrder(BitOrder order) { apply(_clock, order, _mode); }
  void setDataMode(uint8_t mode)   { apply(_clock, _lsb ? LSBFIRST : MSBFIRST, mode); }
  void setClockDivider(uint8_t div) { apply(SystemCoreClock / (div ? div : 2u), _lsb ? LSBFIRST : MSBFIRST, _mode); }
  /* Pines del SPI por software (antes de begin()) */
  void setPins(pin_size_t sck, pin_size_t miso, pin_size_t mosi) { _sck = sck; _miso = miso; _mosi = mosi; }

private:
  void apply(uint32_t clock, BitOrder order, uint8_t mode);
  uint8_t soft(uint8_t b);
  pin_size_t _sck, _miso, _mosi;
  uint32_t   _clock = 4000000;
  uint8_t    _mode = 0;
  bool       _lsb = false;
  bool       _begun = false;
  uint16_t   _half_us = 0;
};

extern CortexMSPI SPI;

#define SPI_CLOCK_DIV2    2
#define SPI_CLOCK_DIV4    4
#define SPI_CLOCK_DIV8    8
#define SPI_CLOCK_DIV16   16
#define SPI_CLOCK_DIV32   32
#define SPI_CLOCK_DIV64   64
#define SPI_CLOCK_DIV128  128

#endif
