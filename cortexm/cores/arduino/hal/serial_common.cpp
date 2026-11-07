/*
 * serial_common.cpp - Lógica de buffers de HardwareSerialCM, común a todas
 * las familias. El hardware se toca solo a través de los métodos hw*().
 */
#include "Arduino.h"

/* true si no se puede esperar a la IRQ: interrupciones bloqueadas o
 * estamos dentro de un manejador (write() llamado desde una ISR). */
static inline bool cm_irq_blocked(void)
{
  return __get_PRIMASK() || (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
}

void HardwareSerialCM::begin(unsigned long baud, uint16_t config)
{
  if (_begun) end();
  _rxHead = _rxTail = 0;
  _txHead = _txTail = 0;
  hwBegin(baud, config);
  _begun = true;
}

void HardwareSerialCM::end()
{
  if (!_begun) return;
  flush();
  hwEnd();
  _begun = false;
}

int HardwareSerialCM::available()
{
  return (uint16_t)(_rxHead - _rxTail) & (SERIAL_RX_BUFFER_SIZE - 1);
}

int HardwareSerialCM::availableForWrite()
{
  return (SERIAL_TX_BUFFER_SIZE - 1) - ((uint16_t)(_txHead - _txTail) & (SERIAL_TX_BUFFER_SIZE - 1));
}

int HardwareSerialCM::peek()
{
  if (_rxHead == _rxTail) return -1;
  return _rxBuf[_rxTail];
}

int HardwareSerialCM::read()
{
  if (_rxHead == _rxTail) return -1;
  uint8_t c = _rxBuf[_rxTail];
  _rxTail = (_rxTail + 1u) & (SERIAL_RX_BUFFER_SIZE - 1);
  return c;
}

size_t HardwareSerialCM::write(uint8_t c)
{
  if (!_begun) return 0;

  /* Camino rápido: nada en cola y registro de datos libre */
  if (_txHead == _txTail && hwTxReady()) {
    hwPut(c);
    return 1;
  }

  uint16_t next = (_txHead + 1u) & (SERIAL_TX_BUFFER_SIZE - 1);
  while (next == _txTail) {
    /* Buffer lleno: si la IRQ no puede correr, vaciar un byte por sondeo */
    if (cm_irq_blocked() && hwTxReady()) {
      uint8_t b;
      if (txPop(&b)) hwPut(b);
    }
  }
  _txBuf[_txHead] = c;
  _txHead = next;
  hwTxIrq(true);
  return 1;
}

size_t HardwareSerialCM::write(const uint8_t *buf, size_t n)
{
  size_t i = 0;
  while (i < n && write(buf[i])) i++;
  return i;
}

void HardwareSerialCM::flush()
{
  if (!_begun) return;
  while (_txHead != _txTail) {
    if (cm_irq_blocked() && hwTxReady()) {
      uint8_t b;
      if (txPop(&b)) hwPut(b);
    }
  }
  while (!hwTxDone()) { }
}
