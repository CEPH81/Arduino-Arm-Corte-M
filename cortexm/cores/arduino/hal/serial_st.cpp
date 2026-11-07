/*
 * serial_st.cpp - Acceso al hardware de USART "estilo ST".
 *   Estilo 1 (SR/DR/BRR/CR1..3): STM32F4, GD32F4, AT32F403A, AT32F43x.
 *            Los clones tienen el mismo mapa y los mismos bits, así que se
 *            usa una estructura propia en lugar del header de cada marca.
 *   Estilo 2 (ISR/ICR/RDR/TDR):  STM32 L4/G4/WB/H5 (FIFO desactivado).
 */
#include "Arduino.h"

#if CM_USART_STYLE == 1 || CM_USART_STYLE == 2

/* Bits comunes de CR1 en ambos estilos */
#define CR1_RE      (1UL << 2)
#define CR1_TE      (1UL << 3)
#define CR1_RXNEIE  (1UL << 5)
#define CR1_TXEIE   (1UL << 7)
#define CR1_PS      (1UL << 9)
#define CR1_PCE     (1UL << 10)
#define CR2_STOP2   (2UL << 12)

#if CM_USART_STYLE == 1
typedef struct { volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR; } usart_t;
#define CR1_UE      (1UL << 13)
#define CR1_M       (1UL << 12)
#define ST_RXNE     (1UL << 5)
#define ST_TC       (1UL << 6)
#define ST_TXE      (1UL << 7)
#define ST_ERR      0x0FUL                     /* PE FE NE ORE */
#define U           ((usart_t *)_base)
#define U_STATUS    (U->SR)
#define U_RD()      (U->DR)
#define U_WR(c)     (U->DR = (c))
#define U_CLR_ERR() ((void)U->DR)               /* leer SR y luego DR los limpia */
#else
#define CR1_UE      USART_CR1_UE
#define U           ((USART_TypeDef *)_base)
#define ST_RXNE     USART_ISR_RXNE
#define ST_TC       USART_ISR_TC
#define ST_TXE      USART_ISR_TXE
#define ST_ERR      (USART_ISR_ORE | USART_ISR_FE | USART_ISR_NE | USART_ISR_PE)
#define U_STATUS    (U->ISR)
#define U_RD()      (U->RDR)
#define U_WR(c)     (U->TDR = (c))
#define U_CLR_ERR() (U->ICR = USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_NECF | USART_ICR_PECF)
#endif

void HardwareSerialCM::hwBegin(unsigned long baud, uint16_t config)
{
  cmPeriphClockEnable(_base);
  cmPinFunction(_txPin, _txFn, false, 0);
#if CM_GPIO_STYLE == 2
  cmPinFunction(_rxPin, 1, false, 1);           /* F1: RX = entrada con pull-up */
#else
  cmPinFunction(_rxPin, _rxFn, false, 1);       /* pull-up: línea en reposo alta */
#endif

  U->CR1 = 0;
  U->CR2 = 0;
  U->CR3 = 0;

  /* Arduino da bits de datos SIN paridad; el USART cuenta la paridad
   * dentro de la longitud de palabra. */
  uint32_t data   = (config & SERIAL_DATA_MASK) == SERIAL_DATA_7 ? 7u : 8u;
  uint32_t parity = config & SERIAL_PARITY_MASK;
  bool hasParity  = (parity == SERIAL_PARITY_EVEN || parity == SERIAL_PARITY_ODD);
  uint32_t word   = data + (hasParity ? 1u : 0u);
  uint32_t cr1    = CR1_TE | CR1_RE | CR1_RXNEIE;
  if (hasParity) cr1 |= CR1_PCE | ((parity == SERIAL_PARITY_ODD) ? CR1_PS : 0u);
#if CM_USART_STYLE == 2
  if (word == 9u) cr1 |= USART_CR1_M0;
  else if (word == 7u) cr1 |= USART_CR1_M1;
#else
  if (word == 9u) cr1 |= CR1_M;                /* estilo 1 no tiene 7 bits */
#endif
  if ((config & SERIAL_STOP_BIT_MASK) == SERIAL_STOP_BIT_2) U->CR2 = CR2_STOP2;

  /* Sobremuestreo x16 en ambos estilos: BRR = PCLK / baud (redondeado) */
  uint32_t pclk = cmPeriphClock(_base);
  U->BRR = (pclk + baud / 2u) / baud;

  U->CR1 = cr1;
  U->CR1 = cr1 | CR1_UE;

  NVIC_SetPriority((IRQn_Type)_irq, 5);
  NVIC_ClearPendingIRQ((IRQn_Type)_irq);
  NVIC_EnableIRQ((IRQn_Type)_irq);
}

void HardwareSerialCM::hwEnd()
{
  NVIC_DisableIRQ((IRQn_Type)_irq);
  U->CR1 = 0;
  pinMode(_txPin, INPUT);
  pinMode(_rxPin, INPUT);
}

bool HardwareSerialCM::hwTxReady()   { return (U_STATUS & ST_TXE) != 0; }
void HardwareSerialCM::hwPut(uint8_t c) { U_WR(c); }
bool HardwareSerialCM::hwTxDone()    { return (U_STATUS & ST_TC) != 0; }

void HardwareSerialCM::hwTxIrq(bool on)
{
  uint32_t m = cmIrqSave();
  if (on) U->CR1 |= CR1_TXEIE; else U->CR1 &= ~CR1_TXEIE;
  cmIrqRestore(m);
}

void HardwareSerialCM::irqHandler()
{
  uint32_t sr = U_STATUS;

#if CM_USART_STYLE == 1
  /* Leer SR y luego DR ya limpia los errores: no leer DR dos veces */
  if (sr & ST_RXNE) rxPush((uint8_t)U_RD());
  else if (sr & ST_ERR) U_CLR_ERR();
#else
  if (sr & ST_RXNE) rxPush((uint8_t)U_RD());
  if (sr & ST_ERR)  U_CLR_ERR();
#endif

  if ((U->CR1 & CR1_TXEIE) && (sr & ST_TXE)) {
    uint8_t c;
    if (txPop(&c)) U_WR(c);
    else U->CR1 &= ~CR1_TXEIE;
  }
}

#endif
