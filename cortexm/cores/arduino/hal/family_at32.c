/*
 * family_at32.c - Relojes de GPIO y periféricos para Artery AT32F43x y
 * AT32F403A, usando el driver CRM oficial (system/Artery/<serie>/Driver).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_AT)

static inline void clk_on(crm_periph_clock_type p)
{
  uint32_t m = cmIrqSave();
  crm_periph_clock_enable(p, TRUE);
  cmIrqRestore(m);
  __DSB();
}

void cmGpioClockEnable(uint32_t portIndex)
{
  clk_on((crm_periph_clock_type)(CRM_GPIOA_PERIPH_CLOCK + portIndex));   /* bits consecutivos */
}

void cmPeriphClockEnable(uintptr_t base)
{
  switch (base) {
    case USART1_BASE: clk_on(CRM_USART1_PERIPH_CLOCK); break;
    case USART2_BASE: clk_on(CRM_USART2_PERIPH_CLOCK); break;
    case USART3_BASE: clk_on(CRM_USART3_PERIPH_CLOCK); break;
#if defined(AT32F435_437)
    case SCFG_BASE:   clk_on(CRM_SCFG_PERIPH_CLOCK); break;
#else
    case IOMUX_BASE:  clk_on(CRM_IOMUX_PERIPH_CLOCK); break;
#endif
    default: break;
  }
}

static uint32_t apb_div(uint32_t code)
{
  return (code & 4u) ? (SystemCoreClock >> ((code & 3u) + 1u)) : SystemCoreClock;
}

uint32_t cmPeriphClock(uintptr_t base)
{
  bool apb2 = (base >= APB2PERIPH_BASE && base < APB2PERIPH_BASE + 0x10000UL);
  return apb_div(apb2 ? CRM->cfg_bit.apb2div : CRM->cfg_bit.apb1div);
}

#endif
