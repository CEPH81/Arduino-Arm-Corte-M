/*
 * system_air105.c - Reloj del Air105 para el núcleo Arduino-Arm-Cortex-M.
 * La ROM arranca el programa con el oscilador de 12 MHz y la caché de la
 * flash QSPI ya configurados. SystemClock_Config() elige la frecuencia del
 * PLL según el menú "Reloj de CPU" (AIR105_PLL), como los ejemplos de MegaHunt:
 * CPU = PLL; el hardware limita HCLK a 102 MHz (HCLK = CPU / 2 por encima) y
 * PCLK = HCLK / 2 (UART, SPI, I2C y temporizadores la leen con
 * SYSCTRL_GetClocksFreq()).
 */
#include "cortexm_device.h"

uint32_t SystemCoreClock = 204000000UL;

void SystemInit(void) { }

void SystemCoreClockUpdate(void)
{
  SYSCTRL_ClocksTypeDef c;
  SYSCTRL_GetClocksFreq(&c);
  SystemCoreClock = c.CPU_Frequency;
}

void SystemClock_Config(void)
{
#if defined(AIR105_PLL)
  SYSCTRL_PLLConfig(AIR105_PLL);
  SYSCTRL_PLLDivConfig(SYSCTRL_PLL_Div_None);
#endif
}
