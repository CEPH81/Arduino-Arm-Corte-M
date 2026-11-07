/*
 * family_swm.c - GPIO y servicios de familia para Synwit SWM181 / SWM341.
 * Las direcciones de los puertos no son regulares, así que se usan dos
 * tablas en flash (GPIO y PORT) indexadas por el número de puerto.
 * La configuración de pines usa los drivers oficiales (GPIO_Init/PORT_Init).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_SWM)

#if defined(CHIP_SWM341)
GPIO_TypeDef *const cm_swm_gpio[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOM, GPION };
static PORT_TypeDef *const swm_port[] = { PORTA, PORTB, PORTC, PORTD, PORTE, PORTM, PORTN };
#else
GPIO_TypeDef *const cm_swm_gpio[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE };
static PORT_TypeDef *const swm_port[] = { PORTA, PORTB, PORTC, PORTD, PORTE };
#endif

/* GPIO_Init() ya habilita el reloj del puerto */
void cmGpioClockEnable(uint32_t portIndex) { (void)portIndex; }
/* UART_Init() habilita el reloj del UART y calcula el baudrate con SystemCoreClock */
void cmPeriphClockEnable(uintptr_t base) { (void)base; }
uint32_t cmPeriphClock(uintptr_t base) { (void)base; return SystemCoreClock; }

void pinMode(pin_size_t pin, PinMode mode)
{
  if (!cmPinValid(pin)) return;
  GPIO_TypeDef *g = cmPinRegs(pin);
  uint32_t n = cmPinBit(pin);
  uint32_t m = cmIrqSave();
  switch (mode) {
    case OUTPUT:           GPIO_Init(g, n, 1, 0, 0, 0); break;
    case OUTPUT_OPENDRAIN: GPIO_Init(g, n, 1, 0, 0, 1); break;
    case INPUT_PULLUP:     GPIO_Init(g, n, 0, 1, 0, 0); break;
    case INPUT_PULLDOWN:   GPIO_Init(g, n, 0, 0, 1, 0); break;
    case INPUT:
    default:               GPIO_Init(g, n, 0, 0, 0, 0); break;
  }
  cmIrqRestore(m);
}

/* fn = código de función del PORT (p.ej. PORTM_PIN1_UART0_TX o FUNMUX_UART0_TXD) */
void cmPinFunction(uint8_t pin, uint8_t fn, bool openDrain, uint8_t pull)
{
  if (!cmPinValid(pin)) return;
  PORT_TypeDef *p = swm_port[cmPinPortIndex(pin)];
  uint32_t bit = cmPinMask(pin);
  uint32_t m = cmIrqSave();
  PORT_Init(p, cmPinBit(pin), fn, 1);
  if (pull == 1u) p->PULLU |= bit; else p->PULLU &= ~bit;
  if (pull == 2u) p->PULLD |= bit; else p->PULLD &= ~bit;
  if (openDrain) p->OPEND |= bit; else p->OPEND &= ~bit;
  cmIrqRestore(m);
}

void cmPinAnalog(uint8_t pin)
{
  if (!cmPinValid(pin)) return;
  /* Función analógica: la elige el driver del ADC; aquí se apaga la entrada digital */
  PORT_Init(swm_port[cmPinPortIndex(pin)], cmPinBit(pin), 0, 0);
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
