/*
 * exti_swm.c - attachInterrupt()/detachInterrupt() para Synwit SWM181/SWM341.
 * Cada puerto tiene su interrupción; el núcleo guarda 16 callbacks (uno por
 * número de bit, como EXTI en STM32): PA3 y PB3 no pueden usarse a la vez.
 * Soporta los 5 modos de Arduino, incluidos LOW y HIGH por nivel.
 *  - SWM341: vectores fijos GPIOA_Handler ... GPION_Handler.
 *  - SWM181: IRQ16+puerto del multiplexor (IRQ16_Handler ... IRQ20_Handler).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_SWM)

typedef struct { voidFuncPtrParam cb; void *param; } exti_slot_t;
static exti_slot_t slots[16];

#if defined(CHIP_SWM341)
static const IRQn_Type port_irq[] = { GPIOA_IRQn, GPIOB_IRQn, GPIOC_IRQn, GPIOD_IRQn, GPIOE_IRQn, GPIOM_IRQn, GPION_IRQn };
#endif

void attachInterruptParam(pin_size_t pin, voidFuncPtrParam cb, PinStatus mode, void *param)
{
  if (!cmPinValid(pin) || cb == NULL) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t bit = cmPinMask(pin), line = cmPinBit(pin), port = cmPinPortIndex(pin);
  uint32_t m = cmIrqSave();
  slots[line].cb = cb;
  slots[line].param = param;
  g->INTEN &= ~bit;
  if (mode == LOW || mode == HIGH) g->INTLVLTRG |= bit; else g->INTLVLTRG &= ~bit;
  if (mode == CHANGE) g->INTBE |= bit; else g->INTBE &= ~bit;
  if (mode == RISING || mode == HIGH) g->INTRISEEN |= bit; else g->INTRISEEN &= ~bit;
  g->INTCLR = bit;
  g->INTEN |= bit;
  cmIrqRestore(m);
#if defined(CHIP_SWM341)
  NVIC_SetPriority(port_irq[port], 6);
  NVIC_EnableIRQ(port_irq[port]);
#else
  IRQ_Connect(IRQ16_31_GPIOA + port, IRQ16_IRQ + port, 2);
#endif
}

static void call_plain(void *p) { ((voidFuncPtr)p)(); }

void attachInterrupt(pin_size_t pin, voidFuncPtr cb, PinStatus mode)
{
  attachInterruptParam(pin, call_plain, mode, (void *)cb);
}

void detachInterrupt(pin_size_t pin)
{
  if (!cmPinValid(pin)) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t bit = cmPinMask(pin);
  uint32_t m = cmIrqSave();
  g->INTEN &= ~bit;
  g->INTCLR = bit;
  slots[cmPinBit(pin)].cb = NULL;
  cmIrqRestore(m);
}

static void dispatch(uint32_t port)
{
  GPIO_TypeDef *g = cm_swm_gpio[port];
  uint32_t st = g->INTSTAT & 0xFFFFu;
  g->INTCLR = st;
  while (st) {
    uint32_t line = 31u - (uint32_t)__CLZ(st);
    st &= ~(1UL << line);
    if (slots[line].cb) slots[line].cb(slots[line].param);
  }
}

#if defined(CHIP_SWM341)
void GPIOA_Handler(void) { dispatch(0); }
void GPIOB_Handler(void) { dispatch(1); }
void GPIOC_Handler(void) { dispatch(2); }
void GPIOD_Handler(void) { dispatch(3); }
void GPIOE_Handler(void) { dispatch(4); }
void GPIOM_Handler(void) { dispatch(5); }
void GPION_Handler(void) { dispatch(6); }
#else
void IRQ16_Handler(void) { dispatch(0); }
void IRQ17_Handler(void) { dispatch(1); }
void IRQ18_Handler(void) { dispatch(2); }
void IRQ19_Handler(void) { dispatch(3); }
void IRQ20_Handler(void) { dispatch(4); }
#endif

#endif
