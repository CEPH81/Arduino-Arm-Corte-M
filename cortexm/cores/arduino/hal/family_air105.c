/*
 * family_air105.c - GPIO y servicios de familia para MegaHunt Air105 (MH1903S).
 *
 * GPIO: puertos A-F cada 0x10 (IODR, BSRR, OEN, PUE). OEN = 1 es entrada.
 * Función de cada pata: GPIO->ALT[puerto], 2 bits por pata
 *   0 = función 0 (UART0 en PA0/PA1...), 1 = GPIO, 2 = función 2, 3 = función 3
 * (tabla de funciones: manual del Air105, capítulo GPIO).
 * Sin salida de drenaje abierto en hardware: se emula con OEN (como el
 * driver de MegaHunt): HIGH suelta la pata, LOW la lleva a 0.
 * Relojes: todos los periféricos APB comparten PCLK.
 */
#include "Arduino.h"

#if defined(CM_VENDOR_MH)

#define ALT_GPIO 1u

static uint16_t od_mask[GPIO_GROUP_NUM];       /* patas en drenaje abierto */

static void set_alt(uint8_t pin, uint32_t fn)
{
  uint32_t port = cmPinPortIndex(pin), sh = cmPinBit(pin) * 2u;
  uint32_t m = cmIrqSave();
  GPIO->ALT[port] = (GPIO->ALT[port] & ~(3UL << sh)) | ((fn & 3u) << sh);
  cmIrqRestore(m);
}

void cmGpioClockEnable(uint32_t portIndex)
{
  (void)portIndex;
  SYSCTRL->CG_CTRL1 |= SYSCTRL_APBPeriph_GPIO;
}

void cmPeriphClockEnable(uintptr_t base)
{
  uint32_t bit = 0;
  switch (base) {
    case UART0_BASE: bit = SYSCTRL_APBPeriph_UART0; break;
    case UART1_BASE: bit = SYSCTRL_APBPeriph_UART1; break;
    case UART2_BASE: bit = SYSCTRL_APBPeriph_UART2; break;
    case UART3_BASE: bit = SYSCTRL_APBPeriph_UART3; break;
    default: return;
  }
  SYSCTRL->CG_CTRL1 |= bit;
}

uint32_t cmPeriphClock(uintptr_t base)
{
  (void)base;
  SYSCTRL_ClocksTypeDef c;
  SYSCTRL_GetClocksFreq(&c);
  return c.PCLK_Frequency;
}

void pinMode(pin_size_t pin, PinMode mode)
{
  if (!cmPinValid(pin)) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t bit = cmPinMask(pin), port = cmPinPortIndex(pin);
  cmGpioClockEnable(port);
  uint32_t m = cmIrqSave();
  od_mask[port] &= (uint16_t)~bit;
  switch (mode) {
    case OUTPUT:
      g->PUE &= ~bit;
      g->OEN &= ~bit;
      break;
    case OUTPUT_OPENDRAIN:                 /* empieza suelta (HIGH) */
      g->BSRR = bit << 16;
      g->PUE &= ~bit;
      g->OEN |= bit;
      od_mask[port] |= (uint16_t)bit;
      break;
    case INPUT_PULLUP:
      g->OEN |= bit;
      g->PUE |= bit;
      break;
    case INPUT_PULLDOWN:                   /* no hay pull-down interno */
    case INPUT:
    default:
      g->OEN |= bit;
      g->PUE &= ~bit;
      break;
  }
  cmIrqRestore(m);
  set_alt(pin, ALT_GPIO);
}

/* fn = valor de ALT (0, 2 o 3). pull: 1 = pull-up (no hay pull-down). */
void cmPinFunction(uint8_t pin, uint8_t fn, bool openDrain, uint8_t pull)
{
  (void)openDrain;
  if (!cmPinValid(pin)) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t bit = cmPinMask(pin);
  cmGpioClockEnable(cmPinPortIndex(pin));
  uint32_t m = cmIrqSave();
  if (pull == 1u) g->PUE |= bit; else g->PUE &= ~bit;
  cmIrqRestore(m);
  set_alt(pin, fn);
}

void cmPinAnalog(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t bit = cmPinMask(pin);
  uint32_t m = cmIrqSave();
  g->OEN |= bit;
  g->PUE &= ~bit;
  cmIrqRestore(m);
}

void digitalWrite(pin_size_t pin, PinStatus val)
{
  if (!cmPinValid(pin)) return;
  uint32_t port = cmPinPortIndex(pin), bit = cmPinMask(pin);
  if (od_mask[port] & bit) {
    GPIO_TypeDef *g = cmPinRegs(pin);
    uint32_t m = cmIrqSave();
    if (val != LOW) g->OEN |= bit; else g->OEN &= ~bit;
    cmIrqRestore(m);
    return;
  }
  if (val != LOW) cm_gpio_set(pin); else cm_gpio_clr(pin);
}

PinStatus digitalRead(pin_size_t pin)
{
  if (!cmPinValid(pin)) return LOW;
  return cm_gpio_read(pin) ? HIGH : LOW;
}

void digitalToggle(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  uint32_t port = cmPinPortIndex(pin), bit = cmPinMask(pin);
  if (od_mask[port] & bit) {
    GPIO_TypeDef *g = cmPinRegs(pin);
    uint32_t m = cmIrqSave();
    g->OEN ^= bit;
    cmIrqRestore(m);
    return;
  }
  cm_gpio_toggle(pin);
}

#endif
