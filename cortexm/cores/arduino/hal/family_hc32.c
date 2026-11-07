/*
 * family_hc32.c - Servicios de familia para HDSC HC32F460.
 *  - Los puertos GPIO no tienen reloj que habilitar.
 *  - Los periféricos se habilitan en PWC_FCGx (bit a 0 = reloj activo).
 *  - Cada IRQ del NVIC (IRQ000..IRQ127) recibe su fuente de evento a través
 *    del INTC: INTC->SELn = número de evento (INT_USART1_RI, ...).
 *  - Palabras ICG en 0x400: todas a 0xFFFFFFFF (perro guardián apagado,
 *    HRC a 16 MHz). Si quedaran con código, el chip arrancaría mal.
 */
#include "Arduino.h"

#if defined(HC32F460)

__attribute__((section(".icg_sec"), used))
const uint32_t cm_hc32_icg[8] = {
  0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
  0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL,
};

void cmGpioClockEnable(uint32_t portIndex) { (void)portIndex; }

void cmPeriphClockEnable(uintptr_t base)
{
  switch (base) {
    case (uintptr_t)M4_USART1: PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART1, Enable); break;
    case (uintptr_t)M4_USART2: PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART2, Enable); break;
    case (uintptr_t)M4_USART3: PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART3, Enable); break;
    case (uintptr_t)M4_USART4: PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART4, Enable); break;
    default: break;
  }
}

/* USART, SPI, I2C y timers van en PCLK1 */
uint32_t cmPeriphClock(uintptr_t base)
{
  (void)base;
  stc_clk_freq_t f;
  CLK_GetClockFreq(&f);
  return f.pclk1Freq;
}

/* Conecta la fuente de evento 'src' a la IRQ 'irqn' del NVIC */
void cmHc32IrqRoute(uint32_t irqn, uint32_t src, uint32_t prio)
{
  (&M4_INTC->SEL0)[irqn] = src;
  NVIC_SetPriority((IRQn_Type)irqn, prio);
  NVIC_ClearPendingIRQ((IRQn_Type)irqn);
  NVIC_EnableIRQ((IRQn_Type)irqn);
}

#endif
