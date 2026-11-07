/*
 * Tabla de vectores generada por tools/gen_vectors.py
 * Origen: Synwit startup_SWM181.s
 * No editar a mano.
 */
#if defined(CHIP_SWM181)

#include "../cm_startup.h"

void NMI_Handler(void) CM_WEAK_ALIAS;
void HardFault_Handler(void) CM_WEAK_ALIAS;
void SVC_Handler(void) CM_WEAK_ALIAS;
void PendSV_Handler(void) CM_WEAK_ALIAS;
void SysTick_Handler(void) CM_WEAK_ALIAS;
void IRQ0_Handler(void) CM_WEAK_ALIAS;
void IRQ1_Handler(void) CM_WEAK_ALIAS;
void IRQ2_Handler(void) CM_WEAK_ALIAS;
void IRQ3_Handler(void) CM_WEAK_ALIAS;
void IRQ4_Handler(void) CM_WEAK_ALIAS;
void IRQ5_Handler(void) CM_WEAK_ALIAS;
void IRQ6_Handler(void) CM_WEAK_ALIAS;
void IRQ7_Handler(void) CM_WEAK_ALIAS;
void IRQ8_Handler(void) CM_WEAK_ALIAS;
void IRQ9_Handler(void) CM_WEAK_ALIAS;
void IRQ10_Handler(void) CM_WEAK_ALIAS;
void IRQ11_Handler(void) CM_WEAK_ALIAS;
void IRQ12_Handler(void) CM_WEAK_ALIAS;
void IRQ13_Handler(void) CM_WEAK_ALIAS;
void IRQ14_Handler(void) CM_WEAK_ALIAS;
void IRQ15_Handler(void) CM_WEAK_ALIAS;
void IRQ16_Handler(void) CM_WEAK_ALIAS;
void IRQ17_Handler(void) CM_WEAK_ALIAS;
void IRQ18_Handler(void) CM_WEAK_ALIAS;
void IRQ19_Handler(void) CM_WEAK_ALIAS;
void IRQ20_Handler(void) CM_WEAK_ALIAS;
void IRQ21_Handler(void) CM_WEAK_ALIAS;
void IRQ22_Handler(void) CM_WEAK_ALIAS;
void IRQ23_Handler(void) CM_WEAK_ALIAS;
void IRQ24_Handler(void) CM_WEAK_ALIAS;
void IRQ25_Handler(void) CM_WEAK_ALIAS;
void IRQ26_Handler(void) CM_WEAK_ALIAS;
void IRQ27_Handler(void) CM_WEAK_ALIAS;
void IRQ28_Handler(void) CM_WEAK_ALIAS;
void IRQ29_Handler(void) CM_WEAK_ALIAS;
void IRQ30_Handler(void) CM_WEAK_ALIAS;
void IRQ31_Handler(void) CM_WEAK_ALIAS;

CM_VECTOR_TABLE(48) = {
  (cm_vector_t)&_estack,
  Reset_Handler,                   /*   1 */
  NMI_Handler,                     /*   2 */
  HardFault_Handler,               /*   3 */
  0,                               /*   4 */
  0,                               /*   5 */
  0,                               /*   6 */
  0,                               /*   7 */
  0,                               /*   8 */
  0,                               /*   9 */
  0,                               /*  10 */
  SVC_Handler,                     /*  11 */
  0,                               /*  12 */
  0,                               /*  13 */
  PendSV_Handler,                  /*  14 */
  SysTick_Handler,                 /*  15 */
  IRQ0_Handler,                    /*  16  IRQ 0 */
  IRQ1_Handler,                    /*  17  IRQ 1 */
  IRQ2_Handler,                    /*  18  IRQ 2 */
  IRQ3_Handler,                    /*  19  IRQ 3 */
  IRQ4_Handler,                    /*  20  IRQ 4 */
  IRQ5_Handler,                    /*  21  IRQ 5 */
  IRQ6_Handler,                    /*  22  IRQ 6 */
  IRQ7_Handler,                    /*  23  IRQ 7 */
  IRQ8_Handler,                    /*  24  IRQ 8 */
  IRQ9_Handler,                    /*  25  IRQ 9 */
  IRQ10_Handler,                   /*  26  IRQ 10 */
  IRQ11_Handler,                   /*  27  IRQ 11 */
  IRQ12_Handler,                   /*  28  IRQ 12 */
  IRQ13_Handler,                   /*  29  IRQ 13 */
  IRQ14_Handler,                   /*  30  IRQ 14 */
  IRQ15_Handler,                   /*  31  IRQ 15 */
  IRQ16_Handler,                   /*  32  IRQ 16 */
  IRQ17_Handler,                   /*  33  IRQ 17 */
  IRQ18_Handler,                   /*  34  IRQ 18 */
  IRQ19_Handler,                   /*  35  IRQ 19 */
  IRQ20_Handler,                   /*  36  IRQ 20 */
  IRQ21_Handler,                   /*  37  IRQ 21 */
  IRQ22_Handler,                   /*  38  IRQ 22 */
  IRQ23_Handler,                   /*  39  IRQ 23 */
  IRQ24_Handler,                   /*  40  IRQ 24 */
  IRQ25_Handler,                   /*  41  IRQ 25 */
  IRQ26_Handler,                   /*  42  IRQ 26 */
  IRQ27_Handler,                   /*  43  IRQ 27 */
  IRQ28_Handler,                   /*  44  IRQ 28 */
  IRQ29_Handler,                   /*  45  IRQ 29 */
  IRQ30_Handler,                   /*  46  IRQ 30 */
  IRQ31_Handler,                   /*  47  IRQ 31 */
};

#endif
