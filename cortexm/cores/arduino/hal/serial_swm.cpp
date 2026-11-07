/*
 * serial_swm.cpp - UART de Synwit (FIFO de 8 bytes en RX y TX).
 *  - RX: interrupción con el FIFO no vacío (umbral 0); se vacía entero.
 *  - TX: interrupción "FIFO con <= 3 bytes"; se rellena desde el buffer.
 * SWM181: las IRQ se asignan con IRQ_Connect(); 'irq' lleva el número de
 * IRQ en los bits 0-7 y la fuente del IRQMUX en los bits 8-15
 * (macro CM_SWM181_IRQ). SWM341: vector fijo UARTn_Handler.
 */
#include "Arduino.h"

#if CM_USART_STYLE == 4

#define U  ((UART_TypeDef *)_base)

void HardwareSerialCM::hwBegin(unsigned long baud, uint16_t config)
{
  cmPinFunction(_rxPin, _rxFn, false, 1);
  cmPinFunction(_txPin, _txFn, false, 0);

  UART_InitStructure c;
  memset(&c, 0, sizeof c);
  uint32_t parity = config & SERIAL_PARITY_MASK;
  c.Baudrate       = baud;
  c.DataBits       = UART_DATA_8BIT;
  c.Parity         = parity == SERIAL_PARITY_EVEN ? UART_PARITY_EVEN :
                     parity == SERIAL_PARITY_ODD  ? UART_PARITY_ODD  : UART_PARITY_NONE;
  c.StopBits       = ((config & SERIAL_STOP_BIT_MASK) == SERIAL_STOP_BIT_2) ? UART_STOP_2BIT : UART_STOP_1BIT;
  c.RXThreshold    = 0;
  c.RXThresholdIEn = 1;
  c.TXThreshold    = 3;
  c.TXThresholdIEn = 0;
  c.TimeoutTime    = 10;
  c.TimeoutIEn     = 0;
  UART_Init(U, &c);

#if defined(CHIP_SWM181)
  IRQ_Connect((uint32_t)_irq >> 8, (uint32_t)_irq & 0xFFu, 1);
#else
  NVIC_SetPriority((IRQn_Type)_irq, 5);
  NVIC_EnableIRQ((IRQn_Type)_irq);
#endif
  UART_Open(U);
}

void HardwareSerialCM::hwEnd()
{
#if defined(CHIP_SWM181)
  NVIC_DisableIRQ((IRQn_Type)((uint32_t)_irq & 0xFFu));
#else
  NVIC_DisableIRQ((IRQn_Type)_irq);
#endif
  UART_INTDis(U, UART_IT_RX_THR | UART_IT_TX_THR);
  UART_Close(U);
  pinMode(_txPin, INPUT);
  pinMode(_rxPin, INPUT);
}

bool HardwareSerialCM::hwTxReady()      { return !UART_IsTXFIFOFull(U); }
void HardwareSerialCM::hwPut(uint8_t c) { UART_WriteByte(U, c); }
bool HardwareSerialCM::hwTxDone()       { return !UART_IsTXBusy(U); }

void HardwareSerialCM::hwTxIrq(bool on)
{
  uint32_t m = cmIrqSave();
  if (on) UART_INTEn(U, UART_IT_TX_THR); else UART_INTDis(U, UART_IT_TX_THR);
  cmIrqRestore(m);
}

void HardwareSerialCM::irqHandler()
{
  uint32_t d;
  while (!UART_IsRXFIFOEmpty(U)) {
    if (UART_ReadByte(U, &d) == 0) rxPush((uint8_t)d);       /* 0 = sin error de paridad */
  }
  if (UART_INTStat(U, UART_IT_TX_THR)) {
    uint8_t c;
    while (!UART_IsTXFIFOFull(U)) {
      if (!txPop(&c)) { UART_INTDis(U, UART_IT_TX_THR); break; }
      UART_WriteByte(U, c);
    }
  }
}

#endif
