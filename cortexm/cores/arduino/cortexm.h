/*
 * cortexm.h - API interna común del núcleo Arduino-Arm-Cortex-M.
 *
 * Numeración de pines: el número Arduino de un pin es  puerto*16 + bit.
 *   PA0 = 0, PA15 = 15, PB0 = 16 ... PH15 = 127.
 * La dirección del puerto y la máscara salen de un desplazamiento y una
 * suma, sin tablas: digitalWrite() termina en un solo store a BSRR/POSR.
 * Las placas con rótulos D0..Dn los definen como alias en su variant.h.
 */
#ifndef CORTEXM_H
#define CORTEXM_H

#include <stdint.h>
#include <stdbool.h>
#include "cortexm_device.h"
#include "hal/cm_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Nombres de pin -------------------------------------------- */
#define CM_PIN(port, bit)   ((uint8_t)(((port) << 4) | (bit)))
#define CM_PIN_NONE         0xFFu

#define CM_PORT_DEF(L, n) \
  P##L##0 = CM_PIN(n,0),  P##L##1 = CM_PIN(n,1),  P##L##2 = CM_PIN(n,2),  P##L##3 = CM_PIN(n,3),  \
  P##L##4 = CM_PIN(n,4),  P##L##5 = CM_PIN(n,5),  P##L##6 = CM_PIN(n,6),  P##L##7 = CM_PIN(n,7),  \
  P##L##8 = CM_PIN(n,8),  P##L##9 = CM_PIN(n,9),  P##L##10 = CM_PIN(n,10), P##L##11 = CM_PIN(n,11), \
  P##L##12 = CM_PIN(n,12), P##L##13 = CM_PIN(n,13), P##L##14 = CM_PIN(n,14), P##L##15 = CM_PIN(n,15)

enum {
  CM_PORT_DEF(A, 0), CM_PORT_DEF(B, 1), CM_PORT_DEF(C, 2), CM_PORT_DEF(D, 3), CM_PORT_DEF(E, 4),
#if defined(CHIP_SWM341)
  CM_PORT_DEF(M, 5), CM_PORT_DEF(N, 6)      /* SWM341: puertos A-E, M y N */
#else
  CM_PORT_DEF(F, 5), CM_PORT_DEF(G, 6), CM_PORT_DEF(H, 7)
#endif
};

/* La variante define CM_VALID_PORTS: bit n = el puerto n existe en el chip.
 * Evita fallos de bus al tocar un puerto inexistente (p.ej. GPIOD en L432). */
#define cmPinValid(p)       ((p) < 128u && ((CM_VALID_PORTS >> ((p) >> 4)) & 1u))

/* Versiones "Fast": sin comprobaciones; con constantes son 1-2 instrucciones */
#define digitalWriteFast(p, v)  do { if (v) cm_gpio_set(p); else cm_gpio_clr(p); } while (0)
#define digitalReadFast(p)      cm_gpio_read(p)
#define digitalToggleFast(p)    cm_gpio_toggle(p)

void digitalToggle(uint8_t pin);

/* ---------- Servicios que implementa cada familia --------------------- */
void     cmGpioClockEnable(uint32_t portIndex);
/* Pin en función alternativa. 'fn' es el código de la familia:
 *   estilo 1: número AF (0..15) · estilo 2: 0 = salida AF push-pull, 1 = entrada
 *   HC32: FSEL (0..63)          · Synwit: número de función del PORT          */
void     cmPinFunction(uint8_t pin, uint8_t fn, bool openDrain, uint8_t pull /*0 nada,1 up,2 down*/);
#define  cmPinAF cmPinFunction
void     cmPinAnalog(uint8_t pin);
/* Frecuencia en Hz del reloj que alimenta al periférico en 'base' */
uint32_t cmPeriphClock(uintptr_t base);
/* Activa el reloj del periférico en 'base' (UART, SPI, I2C, TIM...) */
void     cmPeriphClockEnable(uintptr_t base);

/* Sección crítica que respeta el estado previo de las interrupciones */
static inline uint32_t cmIrqSave(void) { uint32_t m = __get_PRIMASK(); __disable_irq(); return m; }
static inline void cmIrqRestore(uint32_t m) { __set_PRIMASK(m); }

/* Reloj del sistema: cada variante o familia la implementa (débil por defecto) */
void SystemClock_Config(void);

/* Tick de 1 ms */
extern volatile uint32_t cm_ms_ticks;

#ifdef __cplusplus
}
#endif

#endif /* CORTEXM_H */
