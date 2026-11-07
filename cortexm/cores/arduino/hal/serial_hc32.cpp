/*
 * serial_hc32.cpp - USART del HC32F460.
 * Usa tres IRQ consecutivas del NVIC a partir de 'irq' (de la variante):
 *   irq+0 = RX lleno (RI), irq+1 = TX vacío (TI), irq+2 = error (EI).
 * El INTC conecta cada evento a su IRQ y la variante hace que los tres
 * vectores apunten al mismo manejador (CM_HC32_SERIAL_IRQS).
 */
#include "Arduino.h"

#if CM_USART_STYLE == 3

extern "C" void cmHc32IrqRoute(uint32_t irqn, uint32_t src, uint32_t prio);

#define U          ((M4_USART_TypeDef *)_base)
#define SR_PE      (1UL << 0)
#define SR_FE      (1UL << 1)
#define SR_ORE     (1UL << 3)
#define SR_RXNE    (1UL << 5)
#define SR_TC      (1UL << 6)
#define SR_TXE     (1UL << 7)
#define CR1_TXEIE  (1UL << 7)

/* Eventos RI de cada USART (EI = RI-1, TI = RI+1) */
static uint32_t usart_ri_event(uintptr_t base)
{
  if (base == (uintptr_t)M4_USART1) return INT_USART1_RI;
  if (base == (uintptr_t)M4_USART2) return INT_USART2_RI;
  if (base == (uintptr_t)M4_USART3) return INT_USART3_RI;
  return INT_USART4_RI;
}

void HardwareSerialCM::hwBegin(unsigned long baud, uint16_t config)
{
  cmPeriphClockEnable(_base);
  cmPinFunction(_txPin, _txFn, false, 0);
  cmPinFunction(_rxPin, _rxFn, false, 1);

  stc_usart_uart_init_t c;
  memset(&c, 0, sizeof c);
  uint32_t parity = config & SERIAL_PARITY_MASK;
  bool hasParity  = (parity == SERIAL_PARITY_EVEN || parity == SERIAL_PARITY_ODD);
  c.enClkMode    = UsartIntClkCkNoOutput;
  c.enClkDiv     = UsartClkDiv_1;
  c.enDataLength = hasParity ? UsartDataBits9 : UsartDataBits8;   /* paridad incluida */
  c.enDirection  = UsartDataLsbFirst;
  c.enStopBit    = ((config & SERIAL_STOP_BIT_MASK) == SERIAL_STOP_BIT_2) ? UsartTwoStopBit : UsartOneStopBit;
  c.enParity     = !hasParity ? UsartParityNone : (parity == SERIAL_PARITY_ODD ? UsartParityOdd : UsartParityEven);
  c.enSampleMode = UsartSampleBit8;
  c.enDetectMode = UsartStartBitFallEdge;
  c.enHwFlow     = UsartRtsEnable;
  USART_UART_Init(U, &c);
  /* Si el divisor no alcanza con PCLK/1, probar divisores mayores */
  if (USART_SetBaudrate(U, baud) != Ok) {
    USART_FuncCmd(U, UsartRx, Disable);
    c.enClkDiv = UsartClkDiv_16;
    USART_UART_Init(U, &c);
    USART_SetBaudrate(U, baud);
  }

  uint32_t ri = usart_ri_event(_base);
  cmHc32IrqRoute((uint32_t)_irq + 0u, ri,       5);   /* RX lleno */
  cmHc32IrqRoute((uint32_t)_irq + 1u, ri + 1u,  5);   /* TX vacío */
  cmHc32IrqRoute((uint32_t)_irq + 2u, ri - 1u,  5);   /* error    */

  USART_FuncCmd(U, UsartRxInt, Enable);
  USART_FuncCmd(U, UsartRx, Enable);
  USART_FuncCmd(U, UsartTx, Enable);
}

void HardwareSerialCM::hwEnd()
{
  for (uint32_t i = 0; i < 3u; i++) NVIC_DisableIRQ((IRQn_Type)(_irq + (int)i));
  USART_FuncCmd(U, UsartRxInt, Disable);
  USART_FuncCmd(U, UsartTxEmptyInt, Disable);
  USART_FuncCmd(U, UsartRx, Disable);
  USART_FuncCmd(U, UsartTx, Disable);
  pinMode(_txPin, INPUT);
  pinMode(_rxPin, INPUT);
}

bool HardwareSerialCM::hwTxReady()      { return (U->SR & SR_TXE) != 0; }
void HardwareSerialCM::hwPut(uint8_t c) { U->DR = c; }                 /* TDR = bits 0-8 */
bool HardwareSerialCM::hwTxDone()       { return (U->SR & SR_TC) != 0; }

void HardwareSerialCM::hwTxIrq(bool on)
{
  uint32_t m = cmIrqSave();
  if (on) U->CR1 |= CR1_TXEIE; else U->CR1 &= ~CR1_TXEIE;
  cmIrqRestore(m);
}

void HardwareSerialCM::irqHandler()
{
  uint32_t sr = U->SR;
  if (sr & SR_RXNE) rxPush((uint8_t)(U->DR >> 16));                   /* RDR = bits 16-24 */
  if (sr & (SR_PE | SR_FE | SR_ORE)) USART_ClearStatus(U, (en_usart_status_t)(sr & (SR_PE | SR_FE | SR_ORE)));

  if ((U->CR1 & CR1_TXEIE) && (sr & SR_TXE)) {
    uint8_t c;
    if (txPop(&c)) U->DR = c;
    else U->CR1 &= ~CR1_TXEIE;
  }
}

#endif
