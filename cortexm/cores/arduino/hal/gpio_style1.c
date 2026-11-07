/*
 * gpio_style1.c - GPIO "MODER/OTYPER/OSPEEDR/PUPDR/AFR".
 * STM32 F4/L4/G4/WB/H5, GD32F4xx y AT32F43x tienen el mismo mapa y los
 * mismos códigos de campo; solo cambia cómo se habilita el reloj
 * (cmGpioClockEnable, en hal/family_*.c).
 */
#include "Arduino.h"

#if CM_GPIO_STYLE == 1

/* Velocidad/fuerza de salida para pinMode(OUTPUT): AT32 usa 'odrvr' con
 * 1 = fuerza alta; ST/GD 2 = alta velocidad. */
#if defined(CM_VENDOR_AT)
  #define OUT_SPEED  1u
  #define AF_SPEED   1u
#else
  #define OUT_SPEED  2u
  #define AF_SPEED   3u
#endif

/* Escritura atómica de un campo de 'width' bits de un registro */
static inline void field_write(volatile uint32_t *reg, uint32_t pos, uint32_t width, uint32_t val)
{
  uint32_t mask = ((1UL << width) - 1UL) << pos;
  uint32_t m = cmIrqSave();
  *reg = (*reg & ~mask) | ((val << pos) & mask);
  cmIrqRestore(m);
}

static void pin_config(uint8_t pin, uint32_t mode, uint32_t otype, uint32_t pull, uint32_t speed)
{
  cm_gpio_regs_t *g = cmPinRegs(pin);
  uint32_t n = cmPinBit(pin);
  cmGpioClockEnable(cmPinPortIndex(pin));
  field_write(&g->OSPEEDR, n * 2u, 2u, speed);
  field_write(&g->OTYPER,  n,      1u, otype);
  field_write(&g->PUPDR,   n * 2u, 2u, pull);
  field_write(&g->MODER,   n * 2u, 2u, mode);
}

void pinMode(pin_size_t pin, PinMode mode)
{
  if (!cmPinValid(pin)) return;
  switch (mode) {
    case OUTPUT:           pin_config(pin, 1u, 0u, 0u, OUT_SPEED); break;
    case OUTPUT_OPENDRAIN: pin_config(pin, 1u, 1u, 0u, OUT_SPEED); break;
    case INPUT_PULLUP:     pin_config(pin, 0u, 0u, 1u, 0u); break;
    case INPUT_PULLDOWN:   pin_config(pin, 0u, 0u, 2u, 0u); break;
    case INPUT:
    default:               pin_config(pin, 0u, 0u, 0u, 0u); break;
  }
}

void cmPinFunction(uint8_t pin, uint8_t af, bool openDrain, uint8_t pull)
{
  if (!cmPinValid(pin)) return;
  cm_gpio_regs_t *g = cmPinRegs(pin);
  uint32_t n = cmPinBit(pin);
  cmGpioClockEnable(cmPinPortIndex(pin));
  field_write(&g->AFR[n >> 3], (n & 7u) * 4u, 4u, af);
  pin_config(pin, 2u, openDrain ? 1u : 0u, pull, AF_SPEED);
}

void cmPinAnalog(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  pin_config(pin, 3u, 0u, 0u, 0u);
#if defined(GPIO_ASCR_ASC0)
  /* STM32L4: conectar el interruptor analógico al ADC */
  ((GPIO_TypeDef *)cmPinRegs(pin))->ASCR |= cmPinMask(pin);
#endif
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
