/*
 * serial_air105.cpp - UART Synopsys DesignWare 16550 del Air105 (UART0-3),
 * con FIFO de 16 bytes en RX y TX.
 *  - RX: interrupción "dato disponible" y "tiempo de espera" (umbral 1 byte);
 *    se vacía el FIFO entero.
 *  - TX: interrupción "FIFO vacío"; se rellena desde el buffer.
 * El divisor se calcula con PCLK (driver UART_Init de MegaHunt).
 */
#include "Arduino.h"

#if CM_USART_STYLE == 5

#define U  ((UART_TypeDef *)_base)

void HardwareSerialCM::hwBegin(unsigned long baud, uint16_t config)
{
  cmPeriphClockEnable(_base);
  cmPinFunction(_rxPin, _rxFn, false, 1);
  cmPinFunction(_txPin, _txFn, false, 0);

  UART_InitTypeDef c;
  uint32_t bits = config & SERIAL_DATA_MASK, parity = config & SERIAL_PARITY_MASK;
  c.UART_BaudRate   = baud;
  c.UART_WordLength = bits == SERIAL_DATA_7 ? UART_WordLength_7b :
                      bits == SERIAL_DATA_6 ? UART_WordLength_6b :
                      bits == SERIAL_DATA_5 ? UART_WordLength_5b : UART_WordLength_8b;
  c.UART_StopBits   = ((config & SERIAL_STOP_BIT_MASK) == SERIAL_STOP_BIT_2) ? UART_StopBits_2 : UART_StopBits_1;
  c.UART_Parity     = parity == SERIAL_PARITY_EVEN ? UART_Parity_Even :
                      parity == SERIAL_PARITY_ODD  ? UART_Parity_Odd  : UART_Parity_No;
  U->OFFSET_4.IER = 0;
  UART_Init(U, &c);
  /* FIFO activos y vacíos; TX avisa con el FIFO vacío, RX con 1 byte */
  U->OFFSET_8.FCR = UART_FCR_FIFOE | UART_FCR_RFIFOR | UART_FCR_XFIFOR;
  U->OFFSET_4.IER = UART_IER_ERBFI;

  NVIC_SetPriority((IRQn_Type)_irq, 5);
  NVIC_EnableIRQ((IRQn_Type)_irq);
}

void HardwareSerialCM::hwEnd()
{
  NVIC_DisableIRQ((IRQn_Type)_irq);
  U->OFFSET_4.IER = 0;
  pinMode(_txPin, INPUT);
  pinMode(_rxPin, INPUT);
}

bool HardwareSerialCM::hwTxReady()      { return (U->USR & UART_USR_TFNF) != 0; }
void HardwareSerialCM::hwPut(uint8_t c) { U->OFFSET_0.THR = c; }
bool HardwareSerialCM::hwTxDone()       { return (U->LSR & UART_LSR_TEMT) != 0; }

void HardwareSerialCM::hwTxIrq(bool on)
{
  uint32_t m = cmIrqSave();
  if (on) U->OFFSET_4.IER |= UART_IER_ETBEI; else U->OFFSET_4.IER &= ~UART_IER_ETBEI;
  cmIrqRestore(m);
}

void HardwareSerialCM::irqHandler()
{
  uint32_t iid = U->OFFSET_8.IIR & 0x0Fu;
  if (iid == 0x07u) (void)U->USR;               /* "busy detect": se borra leyendo USR */
  if (iid == 0x06u) (void)U->LSR;               /* error de línea: se borra leyendo LSR */
  while (U->LSR & UART_LSR_DR) rxPush((uint8_t)U->OFFSET_0.RBR);
  if (U->OFFSET_4.IER & UART_IER_ETBEI) {
    uint8_t c;
    while (U->USR & UART_USR_TFNF) {
      if (!txPop(&c)) { U->OFFSET_4.IER &= ~UART_IER_ETBEI; break; }
      U->OFFSET_0.THR = c;
    }
  }
}

#endif
