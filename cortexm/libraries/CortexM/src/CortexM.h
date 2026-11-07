/* CortexM.h - Utilidades del núcleo Arduino Arm Cortex-M (CEPH81) */
#pragma once
#include <Arduino.h>

/* Nombre del chip para mensajes */
static inline const char *cortexmChip(void)
{
#if defined(STM32F407xx)
  return "STM32F407";
#elif defined(STM32L432xx)
  return "STM32L432";
#elif defined(STM32G431xx)
  return "STM32G431";
#elif defined(STM32WB55xx)
  return "STM32WB55";
#elif defined(STM32H503xx)
  return "STM32H503";
#elif defined(GD32F427)
  return "GD32F427";
#elif defined(AT32F403A_407)
  return "AT32F403A";
#elif defined(AT32F435_437)
  return "AT32F437";
#elif defined(HC32F460)
  return "HC32F460";
#elif defined(CHIP_SWM181)
  return "SWM181";
#elif defined(CHIP_SWM341)
  return "SWM341";
#else
  return "Cortex-M";
#endif
}
