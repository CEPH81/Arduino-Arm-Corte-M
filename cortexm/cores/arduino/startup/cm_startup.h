/*
 * cm_startup.h - Definiciones para las tablas de vectores generadas.
 */
#ifndef CM_STARTUP_H
#define CM_STARTUP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*cm_vector_t)(void);

extern uint32_t _estack;
void Reset_Handler(void);
void Default_Handler(void);

/* Todo manejador no definido por el usuario o el núcleo cae en
 * Default_Handler. El alias debe apuntar a un símbolo de la misma unidad de
 * compilación, por eso cada tabla tiene su propio cm_unused_irq(). */
#define CM_WEAK_ALIAS  __attribute__((weak, alias("cm_unused_irq")))
__attribute__((used)) static void cm_unused_irq(void) { Default_Handler(); }

/* La tabla va en .isr_vector (el linker script la pone al inicio de FLASH) */
#define CM_VECTOR_TABLE(n) \
  __attribute__((section(".isr_vector"), used)) const cm_vector_t cm_vector_table[n]

extern const cm_vector_t cm_vector_table[];

#ifdef __cplusplus
}
#endif

#endif
