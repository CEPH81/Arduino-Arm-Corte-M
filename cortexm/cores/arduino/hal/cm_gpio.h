/*
 * cm_gpio.h - Acceso rápido a GPIO según el estilo de la familia.
 *
 * Número de pin Arduino = puerto*16 + bit (PA0 = 0, PB0 = 16, ...).
 * Para cada estilo se definen, como funciones inline:
 *   cm_gpio_set(pin)  cm_gpio_clr(pin)  cm_gpio_read(pin)  cm_gpio_toggle(pin)
 * Con un número de pin constante compilan a 1-3 instrucciones.
 */
#ifndef CM_GPIO_H
#define CM_GPIO_H

#include <stdint.h>
#include "../cortexm_device.h"

#define cmPinMask(p)        (1UL << ((p) & 15u))
#define cmPinBit(p)         ((uint32_t)((p) & 15u))
#define cmPinPortIndex(p)   ((uint32_t)((p) >> 4))

#define CM_INLINE static inline __attribute__((always_inline))

/* ===================== Estilo 1: MODER ... BSRR (F4) ===================== */
#if CM_GPIO_STYLE == 1
typedef struct {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} cm_gpio_regs_t;

#define cmGpioRegs(port)  ((cm_gpio_regs_t *)(CM_GPIO_BASE0 + (uint32_t)(port) * CM_GPIO_STRIDE))
#define cmPinRegs(p)      cmGpioRegs(cmPinPortIndex(p))

CM_INLINE void cm_gpio_set(uint32_t p)  { cmPinRegs(p)->BSRR = cmPinMask(p); }
CM_INLINE void cm_gpio_clr(uint32_t p)  { cmPinRegs(p)->BSRR = cmPinMask(p) << 16; }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmPinRegs(p)->IDR >> cmPinBit(p)) & 1u; }
CM_INLINE void cm_gpio_toggle(uint32_t p) {
  cm_gpio_regs_t *g = cmPinRegs(p);
  uint32_t m = cmPinMask(p);
  g->BSRR = (g->ODR & m) ? (m << 16) : m;     /* BSRR: atómico frente a IRQ */
}
#define CM_PORT_OUT_REG(port)  (&cmGpioRegs(port)->ODR)
#define CM_PORT_IN_REG(port)   (&cmGpioRegs(port)->IDR)

/* ===================== Estilo 2: CRL/CRH (F1) ============================ */
#elif CM_GPIO_STYLE == 2
typedef struct {
  volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
} cm_gpio_regs_t;

#define cmGpioRegs(port)  ((cm_gpio_regs_t *)(CM_GPIO_BASE0 + (uint32_t)(port) * CM_GPIO_STRIDE))
#define cmPinRegs(p)      cmGpioRegs(cmPinPortIndex(p))

CM_INLINE void cm_gpio_set(uint32_t p)  { cmPinRegs(p)->BSRR = cmPinMask(p); }
CM_INLINE void cm_gpio_clr(uint32_t p)  { cmPinRegs(p)->BRR  = cmPinMask(p); }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmPinRegs(p)->IDR >> cmPinBit(p)) & 1u; }
CM_INLINE void cm_gpio_toggle(uint32_t p) {
  cm_gpio_regs_t *g = cmPinRegs(p);
  uint32_t m = cmPinMask(p);
  g->BSRR = (g->ODR & m) ? (m << 16) : m;
}
#define CM_PORT_OUT_REG(port)  (&cmGpioRegs(port)->ODR)
#define CM_PORT_IN_REG(port)   (&cmGpioRegs(port)->IDR)

/* ===================== Estilo 3: HC32 PORT =============================== */
#elif CM_GPIO_STYLE == 3
/* Registros de 16 bits por puerto, separados 0x10: PIDR, -, PODR, POER,
 * POSR (set), PORR (reset), POTR (toggle). Puertos: A B C D E H.
 * El puerto H usa el índice 7 en la numeración Arduino (PH0 = 112) y el
 * índice 5 en el hardware. */
#define CM_HC32_PORT_BASE   0x40053800UL
#define cmHc32Port(port)    ((uint32_t)(port) == 7u ? 5u : (uint32_t)(port))
#define cmHc32Reg(p, off)   (*(volatile uint16_t *)(CM_HC32_PORT_BASE + cmHc32Port(cmPinPortIndex(p)) * 0x10u + (off)))

CM_INLINE void cm_gpio_set(uint32_t p)  { cmHc32Reg(p, 0x08) = (uint16_t)cmPinMask(p); }
CM_INLINE void cm_gpio_clr(uint32_t p)  { cmHc32Reg(p, 0x0A) = (uint16_t)cmPinMask(p); }
CM_INLINE void cm_gpio_toggle(uint32_t p) { cmHc32Reg(p, 0x0C) = (uint16_t)cmPinMask(p); }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmHc32Reg(p, 0x00) >> cmPinBit(p)) & 1u; }
#define CM_PORT_OUT_REG(port)  ((volatile uint16_t *)(CM_HC32_PORT_BASE + cmHc32Port(port) * 0x10u + 0x04u))
#define CM_PORT_IN_REG(port)   ((volatile uint16_t *)(CM_HC32_PORT_BASE + cmHc32Port(port) * 0x10u))

/* ===================== Estilo 4: Synwit ================================== */
#elif CM_GPIO_STYLE == 4
/* Las direcciones de los puertos no son regulares: tabla en flash. */
extern GPIO_TypeDef *const cm_swm_gpio[];
#define cmPinRegs(p)  (cm_swm_gpio[cmPinPortIndex(p)])

#if defined(CHIP_SWM341)
/* DATAPINn: escribir 0/1 cambia solo ese pin (atómico). IDR = entrada. */
CM_INLINE void cm_gpio_set(uint32_t p) { (&cmPinRegs(p)->DATAPIN0)[cmPinBit(p)] = 1; }
CM_INLINE void cm_gpio_clr(uint32_t p) { (&cmPinRegs(p)->DATAPIN0)[cmPinBit(p)] = 0; }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmPinRegs(p)->IDR >> cmPinBit(p)) & 1u; }
CM_INLINE void cm_gpio_toggle(uint32_t p) {
  volatile uint32_t *d = &(&cmPinRegs(p)->DATAPIN0)[cmPinBit(p)];
  *d = !(cmPinRegs(p)->ODR >> cmPinBit(p) & 1u);
}
#define CM_PORT_OUT_REG(port)  (&cm_swm_gpio[port]->ODR)
#define CM_PORT_IN_REG(port)   (&cm_swm_gpio[port]->IDR)
#else
/* SWM181 (Cortex-M0): DATA es lectura/escritura; se protege la RMW. */
CM_INLINE void cm_swm_rmw(uint32_t p, uint32_t set) {
  uint32_t pm = __get_PRIMASK();
  __disable_irq();
  if (set) cmPinRegs(p)->DATA |= cmPinMask(p); else cmPinRegs(p)->DATA &= ~cmPinMask(p);
  __set_PRIMASK(pm);
}
CM_INLINE void cm_gpio_set(uint32_t p) { cm_swm_rmw(p, 1); }
CM_INLINE void cm_gpio_clr(uint32_t p) { cm_swm_rmw(p, 0); }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmPinRegs(p)->DATA >> cmPinBit(p)) & 1u; }
CM_INLINE void cm_gpio_toggle(uint32_t p) {
  uint32_t pm = __get_PRIMASK();
  __disable_irq();
  cmPinRegs(p)->DATA ^= cmPinMask(p);
  __set_PRIMASK(pm);
}
#define CM_PORT_OUT_REG(port)  (&cm_swm_gpio[port]->DATA)
#define CM_PORT_IN_REG(port)   (&cm_swm_gpio[port]->DATA)
#endif

/* ===================== Estilo 5: MegaHunt / Air105 ======================= */
#elif CM_GPIO_STYLE == 5
/* Puertos A-F cada 0x10: IODR (bits 0-15 salida, 16-31 entrada),
 * BSRR (0-15 pone a 1, 16-31 pone a 0, atómico), OEN (1 = entrada), PUE. */
#define cmGpioRegs(port)  ((GPIO_TypeDef *)(GPIO_BASE + (uint32_t)(port) * 0x10u))
#define cmPinRegs(p)      cmGpioRegs(cmPinPortIndex(p))

CM_INLINE void cm_gpio_set(uint32_t p)  { cmPinRegs(p)->BSRR = cmPinMask(p); }
CM_INLINE void cm_gpio_clr(uint32_t p)  { cmPinRegs(p)->BSRR = cmPinMask(p) << 16; }
CM_INLINE uint32_t cm_gpio_read(uint32_t p) { return (cmPinRegs(p)->IODR >> (16u + cmPinBit(p))) & 1u; }
CM_INLINE void cm_gpio_toggle(uint32_t p) {
  GPIO_TypeDef *g = cmPinRegs(p);
  uint32_t m = cmPinMask(p);
  g->BSRR = (g->IODR & m) ? (m << 16) : m;
}
#define CM_PORT_OUT_REG(port)  (&cmGpioRegs(port)->IODR)
#define CM_PORT_IN_REG(port)   (&cmGpioRegs(port)->IODR)

#else
#error "CM_GPIO_STYLE desconocido"
#endif

#endif /* CM_GPIO_H */
