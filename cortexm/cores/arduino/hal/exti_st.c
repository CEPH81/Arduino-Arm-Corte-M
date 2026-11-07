/*
 * exti_st.c - attachInterrupt()/detachInterrupt() con EXTI.
 * STM32 (headers CMSIS de ST) y sus clones GD32F4 / AT32 (mismo mapa de
 * registros EXTI y SYSCFG/IOMUX, accedido con una estructura propia).
 * Una línea EXTI por número de bit (0..15): igual que en el hardware, dos
 * pines con el mismo número de bit (PA3 y PB3) no pueden tener interrupción
 * a la vez. LOW/HIGH se tratan como FALLING/RISING (EXTI solo detecta flancos).
 */
#include "Arduino.h"

#if defined(CM_VENDOR_ST) || defined(GD32F4XX) || defined(CM_VENDOR_AT)

#if defined(STM32H5xx)
  #define CM_EXTI_V2 1
#else
  #define CM_EXTI_V2 0
#endif

/* ---- Clones GD32F4 / AT32: EXTI idéntico al de STM32F4 --------------- */
#if !defined(CM_VENDOR_ST)
typedef struct { volatile uint32_t IMR, EMR, RTSR, FTSR, SWIER, PR; } cm_exti_t;
  #if defined(AT32F403A_407)
    #define CM_EXTI        ((cm_exti_t *)0x40010400UL)                 /* EXINT */
    #define CM_EXTICR      ((volatile uint32_t *)0x40010008UL)         /* IOMUX exintc1..4 */
    #define CM_ROUTE_CLK() (*(volatile uint32_t *)(CRM_BASE + 0x18) |= 1UL)      /* IOMUXEN */
  #else
    #define CM_EXTI        ((cm_exti_t *)0x40013C00UL)
    #define CM_EXTICR      ((volatile uint32_t *)0x40013808UL)         /* SYSCFG/SCFG */
    #define CM_ROUTE_CLK() (*(volatile uint32_t *)(0x40023800UL + 0x44) |= (1UL << 14))
  #endif
  #define EXTI_IMR   CM_EXTI->IMR
  #define EXTI_RTSR  CM_EXTI->RTSR
  #define EXTI_FTSR  CM_EXTI->FTSR
  #define EXTI0_IRQ_N      6
  #define EXTI9_5_IRQ_N    23
  #define EXTI15_10_IRQ_N  40
  #if defined(GD32F4XX)
    #define H_EXTI(n)   EXTI##n##_IRQHandler
    #define H_9_5       EXTI5_9_IRQHandler
    #define H_15_10     EXTI10_15_IRQHandler
  #else
    #define H_EXTI(n)   EXINT##n##_IRQHandler
    #define H_9_5       EXINT9_5_IRQHandler
    #define H_15_10     EXINT15_10_IRQHandler
  #endif
#endif

typedef struct {
  voidFuncPtrParam cb;
  void *param;
} exti_slot_t;

static exti_slot_t exti_slots[16];

#if CM_EXTI_V2
_Static_assert(EXTI15_IRQn == EXTI0_IRQn + 15, "vectores EXTI no contiguos");
#endif

/* Nombres de registros según familia */
#if !defined(CM_VENDOR_ST)
  /* ya definidos arriba */
#elif defined(STM32F4xx)
  #define EXTI_IMR   EXTI->IMR
  #define EXTI_RTSR  EXTI->RTSR
  #define EXTI_FTSR  EXTI->FTSR
#else
  #define EXTI_IMR   EXTI->IMR1
  #define EXTI_RTSR  EXTI->RTSR1
  #define EXTI_FTSR  EXTI->FTSR1
#endif

static IRQn_Type exti_irqn(uint32_t line)
{
#if !defined(CM_VENDOR_ST)
  if (line <= 4) return (IRQn_Type)(EXTI0_IRQ_N + (int)line);
  return (IRQn_Type)((line <= 9) ? EXTI9_5_IRQ_N : EXTI15_10_IRQ_N);
#elif CM_EXTI_V2
  return (IRQn_Type)(EXTI0_IRQn + (int)line);   /* H5: un vector por línea */
#else
  switch (line) {
    case 0: return EXTI0_IRQn;
    case 1: return EXTI1_IRQn;
    case 2: return EXTI2_IRQn;
    case 3: return EXTI3_IRQn;
    case 4: return EXTI4_IRQn;
    default: return (line <= 9) ? EXTI9_5_IRQn : EXTI15_10_IRQn;
  }
#endif
}

static void exti_route(uint32_t line, uint32_t port)
{
#if !defined(CM_VENDOR_ST)
  CM_ROUTE_CLK();
  uint32_t sh = (line & 3u) * 4u;
  CM_EXTICR[line >> 2] = (CM_EXTICR[line >> 2] & ~(0xFUL << sh)) | (port << sh);
#elif CM_EXTI_V2
  uint32_t sh = (line & 3u) * 8u;
  EXTI->EXTICR[line >> 2] = (EXTI->EXTICR[line >> 2] & ~(0xFFUL << sh)) | (port << sh);
#else
  #if defined(SYSCFG_BASE)
  cmPeriphClockEnable(SYSCFG_BASE);
  #endif
  uint32_t sh = (line & 3u) * 4u;
  SYSCFG->EXTICR[line >> 2] = (SYSCFG->EXTICR[line >> 2] & ~(0xFUL << sh)) | (port << sh);
#endif
}

static void exti_clear_pending(uint32_t mask)
{
#if !defined(CM_VENDOR_ST)
  CM_EXTI->PR = mask;
#elif CM_EXTI_V2
  EXTI->RPR1 = mask;
  EXTI->FPR1 = mask;
#elif defined(STM32F4xx)
  EXTI->PR = mask;
#else
  EXTI->PR1 = mask;
#endif
}

static uint32_t exti_pending(void)
{
#if !defined(CM_VENDOR_ST)
  return CM_EXTI->PR & EXTI_IMR;
#elif CM_EXTI_V2
  return (EXTI->RPR1 | EXTI->FPR1) & EXTI_IMR;
#elif defined(STM32F4xx)
  return EXTI->PR & EXTI_IMR;
#else
  return EXTI->PR1 & EXTI_IMR;
#endif
}

void attachInterruptParam(pin_size_t pin, voidFuncPtrParam cb, PinStatus mode, void *param)
{
  if (!cmPinValid(pin) || cb == NULL) return;
  uint32_t line = cmPinBit(pin);
  uint32_t mask = 1UL << line;
  bool rise = (mode == RISING || mode == CHANGE || mode == HIGH);
  bool fall = (mode == FALLING || mode == CHANGE || mode == LOW);

  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  EXTI_IMR &= ~mask;
  exti_slots[line].cb = cb;
  exti_slots[line].param = param;
  cmGpioClockEnable(cmPinPortIndex(pin));
  exti_route(line, cmPinPortIndex(pin));
  if (rise) EXTI_RTSR |= mask; else EXTI_RTSR &= ~mask;
  if (fall) EXTI_FTSR |= mask; else EXTI_FTSR &= ~mask;
  exti_clear_pending(mask);
  EXTI_IMR |= mask;
  __set_PRIMASK(primask);

  IRQn_Type irq = exti_irqn(line);
  NVIC_SetPriority(irq, 6);
  NVIC_EnableIRQ(irq);
}

static void call_plain(void *p) { ((voidFuncPtr)p)(); }

void attachInterrupt(pin_size_t pin, voidFuncPtr cb, PinStatus mode)
{
  attachInterruptParam(pin, call_plain, mode, (void *)cb);
}

void detachInterrupt(pin_size_t pin)
{
  if (!cmPinValid(pin)) return;
  uint32_t line = cmPinBit(pin);
  uint32_t mask = 1UL << line;
  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  EXTI_IMR  &= ~mask;
  EXTI_RTSR &= ~mask;
  EXTI_FTSR &= ~mask;
  exti_clear_pending(mask);
  exti_slots[line].cb = NULL;
  __set_PRIMASK(primask);
}

/* Atiende las líneas [first, last] pendientes */
static void exti_dispatch(uint32_t first, uint32_t last)
{
  uint32_t range = ((0xFFFFUL >> (15u - last)) >> first) << first;
  uint32_t pend = exti_pending() & range;
  exti_clear_pending(pend);
  while (pend) {
    uint32_t line = 31u - (uint32_t)__CLZ(pend);
    pend &= ~(1UL << line);
    if (exti_slots[line].cb) exti_slots[line].cb(exti_slots[line].param);
  }
}

#if !defined(CM_VENDOR_ST)
void H_EXTI(0)(void) { exti_dispatch(0, 0); }
void H_EXTI(1)(void) { exti_dispatch(1, 1); }
void H_EXTI(2)(void) { exti_dispatch(2, 2); }
void H_EXTI(3)(void) { exti_dispatch(3, 3); }
void H_EXTI(4)(void) { exti_dispatch(4, 4); }
void H_9_5(void)     { exti_dispatch(5, 9); }
void H_15_10(void)   { exti_dispatch(10, 15); }
#elif CM_EXTI_V2
#define EXTI_H(n) void EXTI##n##_IRQHandler(void) { exti_dispatch(n, n); }
EXTI_H(0) EXTI_H(1) EXTI_H(2)  EXTI_H(3)  EXTI_H(4)  EXTI_H(5)  EXTI_H(6)  EXTI_H(7)
EXTI_H(8) EXTI_H(9) EXTI_H(10) EXTI_H(11) EXTI_H(12) EXTI_H(13) EXTI_H(14) EXTI_H(15)
#else
void EXTI0_IRQHandler(void)     { exti_dispatch(0, 0); }
void EXTI1_IRQHandler(void)     { exti_dispatch(1, 1); }
void EXTI2_IRQHandler(void)     { exti_dispatch(2, 2); }
void EXTI3_IRQHandler(void)     { exti_dispatch(3, 3); }
void EXTI4_IRQHandler(void)     { exti_dispatch(4, 4); }
void EXTI9_5_IRQHandler(void)   { exti_dispatch(5, 9); }
void EXTI15_10_IRQHandler(void) { exti_dispatch(10, 15); }
#endif

#endif
