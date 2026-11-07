/*
 * gpio_hc32.c - GPIO para HC32F460 (registros PCR/PFSR por pin, vía DDL).
 * Nota: el HC32F460 solo tiene pull-up interno; INPUT_PULLDOWN queda como
 * entrada sin resistencia.
 */
#include "Arduino.h"

#if CM_GPIO_STYLE == 3

static inline en_port_t hc_port(uint8_t pin) { return (en_port_t)cmHc32Port(cmPinPortIndex(pin)); }
static inline uint16_t hc_bit(uint8_t pin)   { return (uint16_t)cmPinMask(pin); }

static void hc_init(uint8_t pin, en_pin_mode_t mode, bool od, bool pullup)
{
  stc_port_init_t c;
  memset(&c, 0, sizeof c);
  c.enPinMode  = mode;
  c.enPinOType = od ? Pin_OType_Od : Pin_OType_Cmos;
  c.enPullUp   = pullup ? Enable : Disable;
  c.enPinDrv   = Pin_Drv_H;
  PORT_Init(hc_port(pin), hc_bit(pin), &c);
}

void pinMode(pin_size_t pin, PinMode mode)
{
  if (!cmPinValid(pin)) return;
  PORT_SetFunc(hc_port(pin), hc_bit(pin), Func_Gpio, Disable);
  switch (mode) {
    case OUTPUT:           hc_init(pin, Pin_Mode_Out, false, false); break;
    case OUTPUT_OPENDRAIN: hc_init(pin, Pin_Mode_Out, true,  false); break;
    case INPUT_PULLUP:     hc_init(pin, Pin_Mode_In,  false, true);  break;
    case INPUT_PULLDOWN:
    case INPUT:
    default:               hc_init(pin, Pin_Mode_In,  false, false); break;
  }
}

/* fn = número de función FSEL (Func_Usart1_Tx = 32, ...) */
void cmPinFunction(uint8_t pin, uint8_t fn, bool openDrain, uint8_t pull)
{
  if (!cmPinValid(pin)) return;
  hc_init(pin, Pin_Mode_In, openDrain, pull == 1u);
  PORT_SetFunc(hc_port(pin), hc_bit(pin), (en_port_func_t)fn, Disable);
}

void cmPinAnalog(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  hc_init(pin, Pin_Mode_Ana, false, false);
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
