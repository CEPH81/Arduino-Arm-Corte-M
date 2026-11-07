/*
 * cm_startup.c - Arranque común para todas las familias Cortex-M.
 *
 * Orden: FPU -> .data -> .bss -> SystemInit() del fabricante -> VTOR ->
 * constructores C++ -> main(). SystemInit se llama DESPUÉS de inicializar
 * .data/.bss, así su escritura de SystemCoreClock no se pierde (algunos
 * startups de fabricante lo hacen al revés).
 */
#include "cm_startup.h"
#include "cortexm_device.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
extern void __libc_init_array(void);
extern int main(void);

__attribute__((weak)) void SystemInit(void) { }

__attribute__((noreturn, used, optimize("no-tree-loop-distribute-patterns")))
void Reset_Handler(void)
{
#if defined(__ARM_FP)
  /* Acceso completo a CP10/CP11 antes de cualquier instrucción de coma flotante */
  SCB->CPACR |= (0xFUL << 20);
  __DSB();
  __ISB();
#endif

  uint32_t *src = &_sidata, *dst = &_sdata;
  while (dst < &_edata) *dst++ = *src++;
  for (dst = &_sbss; dst < &_ebss; ) *dst++ = 0;

  SystemInit();

#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1) || (__CORTEX_M >= 3)
  /* Necesario si el programa no empieza en 0x08000000 (bootloader) */
  SCB->VTOR = (uint32_t)cm_vector_table;
  __DSB();
#endif

  __libc_init_array();
  main();
  for (;;) { }
}

/* Excepción o interrupción sin manejador: con depurador se detiene aquí. */
void Default_Handler(void)
{
#if (__CORTEX_M >= 3)
  if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) __BKPT(0);
#endif
  for (;;) { }
}
