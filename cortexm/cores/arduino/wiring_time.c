/*
 * wiring_time.c - millis/micros/delay comunes a cualquier Cortex-M.
 *  - SysTick a 1 kHz para millis().
 *  - micros() combina el contador de ms con SysTick->VAL (sin carreras).
 *  - delayMicroseconds() usa el contador de ciclos DWT->CYCCNT (M3/M4/M7/M33),
 *    con respaldo a SysTick en núcleos sin DWT (M0/M0+).
 */
#include "Arduino.h"

volatile uint32_t cm_ms_ticks = 0;
static uint32_t cm_cycles_per_us = 1;

void SysTick_Handler(void)
{
  cm_ms_ticks++;
}

void cmTimeInit(void)
{
  cm_cycles_per_us = SystemCoreClock / 1000000UL;
  if (cm_cycles_per_us == 0) cm_cycles_per_us = 1;
  SysTick_Config(SystemCoreClock / 1000UL);
#if defined(DWT) && (__CORTEX_M >= 3)
  #if defined(DCB)
  DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
  #else
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  #endif
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#endif
}

unsigned long millis(void)
{
  return cm_ms_ticks;
}

unsigned long micros(void)
{
  uint32_t ms, val, load = SysTick->LOAD + 1UL;
  do {
    ms  = cm_ms_ticks;
    val = SysTick->VAL;
    /* SysTick se desbordó y su IRQ aún no corrió (interrupciones
     * deshabilitadas o contexto de mayor prioridad): releer VAL, que ya
     * es posterior al desborde, y contar ese milisegundo. */
    if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk) {
      val = SysTick->VAL;
      ms++;
      break;
    }
  } while (ms != cm_ms_ticks);
  return ms * 1000UL + ((load - 1UL - val) * 1000UL) / load;
}

void delay(unsigned long ms)
{
  if (ms == 0) return;
  uint32_t start = micros();
  while (ms > 0) {
    yield();
    while (ms > 0 && (micros() - start) >= 1000UL) {
      ms--;
      start += 1000UL;
    }
  }
}

void delayMicroseconds(unsigned int us)
{
#if defined(DWT) && (__CORTEX_M >= 3)
  uint32_t start = DWT->CYCCNT;
  uint32_t ticks = (uint32_t)us * cm_cycles_per_us;
  while ((DWT->CYCCNT - start) < ticks) { }
#else
  uint32_t start = micros();
  while ((micros() - start) < us) { }
#endif
}

void yield(void) __attribute__((weak));
void yield(void) { }
