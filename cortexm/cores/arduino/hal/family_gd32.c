/*
 * family_gd32.c - Relojes de GPIO y periféricos para GD32F4xx.
 * El RCU de GD32F4 tiene el mismo mapa que el RCC de STM32F4
 * (AHB1EN 0x30, APB1EN 0x40, APB2EN 0x44, prescalers APB en CFG0).
 */
#include "Arduino.h"

#if defined(GD32F4XX)

_Static_assert(GPIOB - GPIOA == CM_GPIO_STRIDE, "separacion GPIO inesperada");

#define RCU_REG(off)  (*(volatile uint32_t *)(RCU_BASE + (off)))

static inline void set_and_sync(uint32_t off, uint32_t bits)
{
  if ((RCU_REG(off) & bits) == bits) return;
  uint32_t m = cmIrqSave();
  RCU_REG(off) |= bits;
  cmIrqRestore(m);
  (void)RCU_REG(off);
  __DSB();
}

void cmGpioClockEnable(uint32_t portIndex)
{
  set_and_sync(0x30, 1UL << portIndex);            /* RCU_AHB1EN: PAEN = bit 0 */
}

void cmPeriphClockEnable(uintptr_t base)
{
  switch (base) {
    case USART0: set_and_sync(0x44, 1UL << 4);  break;   /* APB2 */
    case USART5: set_and_sync(0x44, 1UL << 5);  break;   /* APB2 */
    case USART1: set_and_sync(0x40, 1UL << 17); break;   /* APB1 */
    case USART2: set_and_sync(0x40, 1UL << 18); break;
    case UART3:  set_and_sync(0x40, 1UL << 19); break;
    case UART4:  set_and_sync(0x40, 1UL << 20); break;
    case SYSCFG_BASE: set_and_sync(0x44, 1UL << 14); break;
    default: break;
  }
}

static uint32_t apb_div(uint32_t code)
{
  return (code & 4u) ? (SystemCoreClock >> ((code & 3u) + 1u)) : SystemCoreClock;
}

uint32_t cmPeriphClock(uintptr_t base)
{
  uint32_t cfg0 = RCU_REG(0x08);
  bool apb2 = (base >= APB2_BUS_BASE && base < APB2_BUS_BASE + 0x10000UL);
  return apb_div(apb2 ? (cfg0 >> 13) & 7u : (cfg0 >> 10) & 7u);
}

#endif
