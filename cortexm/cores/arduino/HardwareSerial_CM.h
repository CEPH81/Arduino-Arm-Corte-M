/*
 * HardwareSerial_CM.h - Serial por UART con interrupciones y buffers
 * circulares de RX y TX (write() no bloquea mientras haya espacio).
 *
 * La lógica de buffers es común (hal/serial_common.cpp). Cada familia
 * implementa solo el acceso al hardware (hw*) y irqHandler().
 *
 * Cada variante instancia sus puertos y engancha la IRQ con:
 *   CM_SERIAL(Serial, USART1, USART1_IRQn, USART1_IRQHandler, PA9, 7, PA10, 7)
 *             nombre  periférico  IRQ       manejador        TX  fn RX  fn
 */
#ifndef HARDWARESERIAL_CM_H
#define HARDWARESERIAL_CM_H

#ifdef __cplusplus

#include "api/HardwareSerial.h"
#include "cortexm.h"

#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 256
#endif
#ifndef SERIAL_TX_BUFFER_SIZE
#define SERIAL_TX_BUFFER_SIZE 128
#endif
static_assert((SERIAL_RX_BUFFER_SIZE & (SERIAL_RX_BUFFER_SIZE - 1)) == 0, "RX buffer debe ser potencia de 2");
static_assert((SERIAL_TX_BUFFER_SIZE & (SERIAL_TX_BUFFER_SIZE - 1)) == 0, "TX buffer debe ser potencia de 2");

class HardwareSerialCM : public arduino::HardwareSerial {
public:
  HardwareSerialCM(uintptr_t base, int irq, uint8_t txPin, uint8_t txFn, uint8_t rxPin, uint8_t rxFn)
    : _base(base), _irq(irq), _txPin(txPin), _txFn(txFn), _rxPin(rxPin), _rxFn(rxFn) {}

  void begin(unsigned long baud) override { begin(baud, SERIAL_8N1); }
  void begin(unsigned long baud, uint16_t config) override;
  void end() override;
  int available() override;
  int availableForWrite() override;
  int peek() override;
  int read() override;
  void flush() override;
  size_t write(uint8_t c) override;
  size_t write(const uint8_t *buf, size_t n) override;
  using Print::write;
  operator bool() override { return _begun; }

  /* Llamado desde el manejador de IRQ de la variante */
  void irqHandler();

  /* Usados por irqHandler() de cada familia */
  inline void rxPush(uint8_t c) {
    uint16_t next = (_rxHead + 1u) & (SERIAL_RX_BUFFER_SIZE - 1);
    if (next != _rxTail) { _rxBuf[_rxHead] = c; _rxHead = next; }   /* lleno: se descarta */
  }
  inline bool txPop(uint8_t *c) {
    if (_txHead == _txTail) return false;
    *c = _txBuf[_txTail];
    _txTail = (_txTail + 1u) & (SERIAL_TX_BUFFER_SIZE - 1);
    return true;
  }

private:
  /* --- Implementados por la familia (hal/serial_<familia>.cpp) --- */
  void hwBegin(unsigned long baud, uint16_t config);
  void hwEnd();
  bool hwTxReady();            /* registro de datos de TX libre */
  void hwPut(uint8_t c);       /* escribe un byte en el registro de TX */
  void hwTxIrq(bool on);       /* habilita/deshabilita la IRQ de "TX vacío" */
  bool hwTxDone();             /* último bit ya salió por el pin */

  uintptr_t _base;
  int _irq;
  uint8_t _txPin, _txFn, _rxPin, _rxFn;
  bool _begun = false;

  volatile uint16_t _rxHead = 0, _rxTail = 0;
  volatile uint16_t _txHead = 0, _txTail = 0;
  uint8_t _rxBuf[SERIAL_RX_BUFFER_SIZE];
  uint8_t _txBuf[SERIAL_TX_BUFFER_SIZE];
};

#define CM_SERIAL(name, periph, irqn, handler, tx, txfn, rx, rxfn)            \
  HardwareSerialCM name((uintptr_t)(periph), (int)(irqn), tx, txfn, rx, rxfn); \
  extern "C" void handler(void) { name.irqHandler(); }

#endif /* __cplusplus */
#endif
