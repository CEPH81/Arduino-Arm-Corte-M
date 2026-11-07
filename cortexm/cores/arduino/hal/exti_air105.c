/*
 * exti_air105.c - attachInterrupt()/detachInterrupt() para el Air105.
 * Cada puerto (A-F) tiene su interrupción (EXTI0_IRQn ... EXTI5_IRQn) y
 * registros INTP_TYPE (2 bits por pata: 1 subida, 2 bajada, 3 ambos) e
 * INTP_STA (escribir 1 borra). Solo hay flancos: LOW se trata como FALLING
 * y HIGH como RISING. El núcleo guarda 16 callbacks (uno por número de bit,
 * como EXTI en STM32): PA3 y PB3 no pueden usarse a la vez.
 */
#include "Arduino.h"

#if defined(CM_VENDOR_MH)

typedef struct { voidFuncPtrParam cb; void *param; } exti_slot_t;
static exti_slot_t slots[16];

static const IRQn_Type port_irq[] = { EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, EXTI5_IRQn };

void attachInterruptParam(pin_size_t pin, voidFuncPtrParam cb, PinStatus mode, void *param)
{
  if (!cmPinValid(pin) || cb == NULL) return;
  uint32_t port = cmPinPortIndex(pin), line = cmPinBit(pin), sh = line * 2u;
  uint32_t t = (mode == CHANGE) ? 3u : (mode == RISING || mode == HIGH) ? 1u : 2u;
  uint32_t m = cmIrqSave();
  slots[line].cb = cb;
  slots[line].param = param;
  GPIO->INTP_TYPE_STA[port].INTP_TYPE = (GPIO->INTP_TYPE_STA[port].INTP_TYPE & ~(3UL << sh)) | (t << sh);
  GPIO->INTP_TYPE_STA[port].INTP_STA = 1UL << line;
  cmIrqRestore(m);
  NVIC_SetPriority(port_irq[port], 6);
  NVIC_EnableIRQ(port_irq[port]);
}

static void call_plain(void *p) { ((voidFuncPtr)p)(); }

void attachInterrupt(pin_size_t pin, voidFuncPtr cb, PinStatus mode)
{
  attachInterruptParam(pin, call_plain, mode, (void *)cb);
}

void detachInterrupt(pin_size_t pin)
{
  if (!cmPinValid(pin)) return;
  uint32_t port = cmPinPortIndex(pin), line = cmPinBit(pin);
  uint32_t m = cmIrqSave();
  GPIO->INTP_TYPE_STA[port].INTP_TYPE &= ~(3UL << (line * 2u));
  GPIO->INTP_TYPE_STA[port].INTP_STA = 1UL << line;
  slots[line].cb = NULL;
  cmIrqRestore(m);
}

static void dispatch(uint32_t port)
{
  uint32_t st = GPIO->INTP_TYPE_STA[port].INTP_STA & 0xFFFFu;
  GPIO->INTP_TYPE_STA[port].INTP_STA = st;
  while (st) {
    uint32_t line = 31u - (uint32_t)__CLZ(st);
    st &= ~(1UL << line);
    if (slots[line].cb) slots[line].cb(slots[line].param);
  }
}

void EXTI0_IRQHandler(void) { dispatch(0); }
void EXTI1_IRQHandler(void) { dispatch(1); }
void EXTI2_IRQHandler(void) { dispatch(2); }
void EXTI3_IRQHandler(void) { dispatch(3); }
void EXTI4_IRQHandler(void) { dispatch(4); }
void EXTI5_IRQHandler(void) { dispatch(5); }

#endif
