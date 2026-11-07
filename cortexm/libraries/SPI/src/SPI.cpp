/*
 * SPI.cpp - SPI maestro (Arduino-Arm-Cortex-M).
 *
 * Hardware (CM_SPI_HW): CR1/CR2/SR/DR de ST, que GigaDevice y Artery
 * repiten bit a bit. En STM32 L4/G4/WB el tamaño del dato va en CR2.DS y el
 * registro DR se accede en 8 bits para no mandar 16.
 */
#include "SPI.h"

CortexMSPI SPI(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);

CortexMSPI::CortexMSPI(pin_size_t sck, pin_size_t miso, pin_size_t mosi)
  : _sck(sck), _miso(miso), _mosi(mosi) { }

#if defined(CM_SPI_HW)
typedef struct { volatile uint32_t CR1, CR2, SR, DR; } cm_spi_t;
#define SPIx      ((cm_spi_t *)(CM_SPI_HW))
#define CR1_MSTR  (1u << 2)
#define CR1_SPE   (1u << 6)
#define CR1_LSB   (1u << 7)
#define CR1_SSI   (1u << 8)
#define CR1_SSM   (1u << 9)
#define SR_RXNE   (1u << 0)
#define SR_TXE    (1u << 1)
#define SR_BSY    (1u << 7)
#define DR8       (*(volatile uint8_t *)&SPIx->DR)

static void spi_clock_on(void)
{
#if defined(CM_VENDOR_AT)
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE);
#elif defined(CM_VENDOR_GD)
  *(volatile uint32_t *)(0x40023800UL + 0x44) |= 1UL << 12;   /* RCU_APB2EN.SPI0EN */
#else
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
#endif
  __DSB();
}
#endif

void CortexMSPI::begin()
{
#if defined(CM_SPI_HW)
  spi_clock_on();
  #if CM_GPIO_STYLE == 2
  cmPinFunction(_sck, 0, false, 0);           /* AT32F403A: salida AF */
  cmPinFunction(_mosi, 0, false, 0);
  cmPinFunction(_miso, 1, false, 1);          /* entrada con pull-up */
  #else
  cmPinFunction(_sck, CM_SPI_AF, false, 0);
  cmPinFunction(_mosi, CM_SPI_AF, false, 0);
  cmPinFunction(_miso, CM_SPI_AF, false, 1);
  #endif
#else
  pinMode(_sck, OUTPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_miso, INPUT_PULLUP);
#endif
  _begun = true;
  apply(_clock, _lsb ? LSBFIRST : MSBFIRST, _mode);
}

void CortexMSPI::end()
{
#if defined(CM_SPI_HW)
  while (SPIx->SR & SR_BSY) { }
  SPIx->CR1 = 0;
#endif
  pinMode(_sck, INPUT);
  pinMode(_mosi, INPUT);
  pinMode(_miso, INPUT);
  _begun = false;
}

void CortexMSPI::beginTransaction(arduino::SPISettings s)
{
  apply(s.getClockFreq(), s.getBitOrder(), (uint8_t)s.getDataMode());
}

void CortexMSPI::apply(uint32_t clock, BitOrder order, uint8_t mode)
{
  _clock = clock ? clock : 1000000u;
  _lsb = (order == LSBFIRST);
  _mode = mode & 3u;
  if (!_begun) return;
#if defined(CM_SPI_HW)
  /* divisor = 2^(BR+1): el más chico que no pase de 'clock' */
  uint32_t pclk = cmPeriphClock(CM_SPI_HW), br = 0;
  while (br < 7u && (pclk >> (br + 1u)) > _clock) br++;
  while (SPIx->SR & SR_BSY) { }
  SPIx->CR1 = 0;
  #if defined(SPI_CR2_DS_Pos) && defined(SPI_CR2_FRXTH)
  SPIx->CR2 = (7UL << SPI_CR2_DS_Pos) | SPI_CR2_FRXTH;     /* 8 bits */
  #endif
  SPIx->CR1 = CR1_MSTR | CR1_SSM | CR1_SSI | (br << 3) | (_lsb ? CR1_LSB : 0u) | _mode;
  SPIx->CR1 |= CR1_SPE;
  (void)SPIx->DR;
#else
  digitalWrite(_sck, (_mode & 2u) ? HIGH : LOW);        /* CPOL */
  uint32_t half = 500000UL / _clock;
  _half_us = (uint16_t)(half > 0xFFFFu ? 0xFFFFu : half);
#endif
}

uint8_t CortexMSPI::transfer(uint8_t b)
{
#if defined(CM_SPI_HW)
  while (!(SPIx->SR & SR_TXE)) { }
  DR8 = b;
  while (!(SPIx->SR & SR_RXNE)) { }
  return DR8;
#else
  return soft(b);
#endif
}

#if !defined(CM_SPI_HW)
/* Un byte por software. CPHA = 0: el dato se toma en el primer flanco;
 * CPHA = 1: en el segundo. */
uint8_t CortexMSPI::soft(uint8_t out)
{
  bool cpol = _mode & 2u, cpha = _mode & 1u;
  uint8_t in = 0;
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bit = _lsb ? (uint8_t)((out >> i) & 1u) : (uint8_t)((out >> (7 - i)) & 1u);
    if (!cpha) digitalWrite(_mosi, bit ? HIGH : LOW);
    if (_half_us) delayMicroseconds(_half_us);
    digitalWrite(_sck, cpol ? LOW : HIGH);              /* primer flanco */
    if (cpha) digitalWrite(_mosi, bit ? HIGH : LOW);
    else      in |= (uint8_t)((digitalRead(_miso) ? 1u : 0u) << (_lsb ? i : 7 - i));
    if (_half_us) delayMicroseconds(_half_us);
    digitalWrite(_sck, cpol ? HIGH : LOW);              /* segundo flanco */
    if (cpha) in |= (uint8_t)((digitalRead(_miso) ? 1u : 0u) << (_lsb ? i : 7 - i));
  }
  return in;
}
#else
uint8_t CortexMSPI::soft(uint8_t b) { return transfer(b); }
#endif

uint16_t CortexMSPI::transfer16(uint16_t d)
{
  if (_lsb) {
    uint16_t lo = transfer((uint8_t)d);
    return (uint16_t)(lo | (uint16_t)transfer((uint8_t)(d >> 8)) << 8);
  }
  uint16_t hi = transfer((uint8_t)(d >> 8));
  return (uint16_t)((hi << 8) | transfer((uint8_t)d));
}

void CortexMSPI::transfer(void *buf, size_t count)
{
  uint8_t *p = (uint8_t *)buf;
  while (count--) { *p = transfer(*p); p++; }
}

void CortexMSPI::transfer(const void *txbuf, void *rxbuf, size_t count)
{
  const uint8_t *t = (const uint8_t *)txbuf;
  uint8_t *r = (uint8_t *)rxbuf;
  for (size_t i = 0; i < count; i++) {
    uint8_t v = transfer(t ? t[i] : 0xFFu);
    if (r) r[i] = v;
  }
}
