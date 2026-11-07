/*
 * exti_hc32.c - attachInterrupt()/detachInterrupt() para HC32F460.
 * 16 líneas EIRQ (una por número de bit, como EXTI en STM32): dos pines
 * con el mismo bit (PA3 y PB3) no pueden tener interrupción a la vez.
 * La línea n usa la IRQ 16+n del NVIC (IRQ016..IRQ031), conectada por INTC.
 */
#include "Arduino.h"

#if defined(HC32F460)

extern void cmHc32IrqRoute(uint32_t irqn, uint32_t src, uint32_t prio);

#define EXTI_IRQ_BASE  16u
#define PCR(p)   (*(volatile uint16_t *)(CM_HC32_PORT_BASE + 0x400u + cmHc32Port(cmPinPortIndex(p)) * 0x40u + cmPinBit(p) * 4u))
#define PCR_INTE (1u << 12)

typedef struct { voidFuncPtrParam cb; void *param; } exti_slot_t;
static exti_slot_t slots[16];

void attachInterruptParam(pin_size_t pin, voidFuncPtrParam cb, PinStatus mode, void *param)
{
  if (!cmPinValid(pin) || cb == NULL) return;
  uint32_t line = cmPinBit(pin);
  /* EIRQTRG: 0 = flanco de bajada, 1 = subida, 2 = ambos, 3 = nivel bajo */
  uint32_t trg;
  switch (mode) {
    case RISING:  trg = 1u; break;
    case CHANGE:  trg = 2u; break;
    case LOW:     trg = 3u; break;
    case HIGH:    trg = 1u; break;   /* sin nivel alto en HW: se usa subida */
    case FALLING:
    default:      trg = 0u; break;
  }
  uint32_t m = cmIrqSave();
  slots[line].cb = cb;
  slots[line].param = param;
  (&M4_INTC->EIRQCR0)[line] = trg;
  M4_INTC->EICFR = 1UL << line;
  PORT_Unlock();
  PCR(pin) |= PCR_INTE;
  PORT_Lock();
  cmIrqRestore(m);
  cmHc32IrqRoute(EXTI_IRQ_BASE + line, INT_PORT_EIRQ0 + line, 6);
}

static void call_plain(void *p) { ((voidFuncPtr)p)(); }

void attachInterrupt(pin_size_t pin, voidFuncPtr cb, PinStatus mode)
{
  attachInterruptParam(pin, call_plain, mode, (void *)cb);
}

void detachInterrupt(pin_size_t pin)
{
  if (!cmPinValid(pin)) return;
  uint32_t line = cmPinBit(pin);
  NVIC_DisableIRQ((IRQn_Type)(EXTI_IRQ_BASE + line));
  PORT_Unlock();
  PCR(pin) &= (uint16_t)~PCR_INTE;
  PORT_Lock();
  slots[line].cb = NULL;
}

static inline void dispatch(uint32_t line)
{
  M4_INTC->EICFR = 1UL << line;
  if (slots[line].cb) slots[line].cb(slots[line].param);
}

#define H(n, line) void IRQ0##n##_Handler(void) { dispatch(line); }
H(16, 0)  H(17, 1)  H(18, 2)  H(19, 3)  H(20, 4)  H(21, 5)  H(22, 6)  H(23, 7)
H(24, 8)  H(25, 9)  H(26, 10) H(27, 11) H(28, 12) H(29, 13) H(30, 14) H(31, 15)

#endif
