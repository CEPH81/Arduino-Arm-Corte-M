/*
 * gpio_style2.c - GPIO "CRL/CRH" (STM32F1, AT32F403A): 4 bits por pin
 * (MODE[1:0] + CNF[1:0]); el pull-up/down lo elige ODR en modo entrada.
 */
#include "Arduino.h"

#if CM_GPIO_STYLE == 2

#define CFG_IN_FLOAT   0x4u
#define CFG_IN_PULL    0x8u
#define CFG_OUT_PP     0x3u     /* salida 50 MHz push-pull */
#define CFG_OUT_OD     0x7u
#define CFG_AF_PP      0xBu
#define CFG_AF_OD      0xFu
#define CFG_ANALOG     0x0u

static void pin_cfg(uint8_t pin, uint32_t cfg)
{
  cm_gpio_regs_t *g = cmPinRegs(pin);
  uint32_t n = cmPinBit(pin);
  volatile uint32_t *reg = (n < 8u) ? &g->CRL : &g->CRH;
  uint32_t sh = (n & 7u) * 4u;
  cmGpioClockEnable(cmPinPortIndex(pin));
  uint32_t m = cmIrqSave();
  *reg = (*reg & ~(0xFUL << sh)) | (cfg << sh);
  cmIrqRestore(m);
}

void pinMode(pin_size_t pin, PinMode mode)
{
  if (!cmPinValid(pin)) return;
  switch (mode) {
    case OUTPUT:           pin_cfg(pin, CFG_OUT_PP); break;
    case OUTPUT_OPENDRAIN: pin_cfg(pin, CFG_OUT_OD); break;
    case INPUT_PULLUP:     cm_gpio_set(pin); pin_cfg(pin, CFG_IN_PULL); break;
    case INPUT_PULLDOWN:   cm_gpio_clr(pin); pin_cfg(pin, CFG_IN_PULL); break;
    case INPUT:
    default:               pin_cfg(pin, CFG_IN_FLOAT); break;
  }
}

/* fn: 0 = salida de función alternativa, 1 = entrada de periférico.
 * (En F1 la entrada de un periférico es un pin de entrada normal.) */
void cmPinFunction(uint8_t pin, uint8_t fn, bool openDrain, uint8_t pull)
{
  if (!cmPinValid(pin)) return;
  if (fn == 1u) {
    if (pull == 1u)      { cm_gpio_set(pin); pin_cfg(pin, CFG_IN_PULL); }
    else if (pull == 2u) { cm_gpio_clr(pin); pin_cfg(pin, CFG_IN_PULL); }
    else                 pin_cfg(pin, CFG_IN_FLOAT);
  } else {
    pin_cfg(pin, openDrain ? CFG_AF_OD : CFG_AF_PP);
  }
}

void cmPinAnalog(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  pin_cfg(pin, CFG_ANALOG);
}

void digitalWrite(pin_size_t pin, PinStatus val)
{
  if (!cmPinValid(pin)) return;
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
  cm_gpio_toggle(pin);
}

#endif
