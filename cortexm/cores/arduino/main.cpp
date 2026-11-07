/*
 * main.cpp - Punto de entrada del núcleo Arduino-Arm-Cortex-M.
 * SystemInit() (CMSIS del fabricante) ya corrió desde el startup.
 */
#include "Arduino.h"

/* La variante define SystemClock_Config() (PLL, cristal, latencia de flash).
 * Si no lo hace, el chip sigue con el reloj que dejó SystemInit(). */
extern "C" __attribute__((weak)) void SystemClock_Config(void) { }
__attribute__((weak)) void initVariant(void) { }

void init(void)
{
  SystemClock_Config();       /* PLL / cristal / latencia de flash de la placa */
  SystemCoreClockUpdate();    /* SystemCoreClock refleja el reloj real */
  cmTimeInit();               /* SysTick 1 ms + DWT */
}

int main(void)
{
  init();
  initVariant();
  setup();
  for (;;) {
    loop();
    if (arduino::serialEventRun) arduino::serialEventRun();
  }
  return 0;
}
