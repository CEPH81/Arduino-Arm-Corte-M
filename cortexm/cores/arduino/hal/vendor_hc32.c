/*
 * vendor_hc32.c - Compila los drivers DDL de HDSC que usa el núcleo
 * (reloj, flash, SRAM, GPIO, USART, PWC, ADC, Timer A) solo cuando la placa es HC32F460.
 * Las interrupciones NO usan hc32f460_interrupts.c (define SysTick y todos
 * los IRQ como funciones fuertes); el INTC se programa en family_hc32.c.
 */
#if defined(HC32F460)
  #include "hc32f460_clk.c"
  #include "hc32f460_efm.c"
  #include "hc32f460_sram.c"
  #include "hc32f460_gpio.c"
  #include "hc32f460_usart.c"
  #include "hc32f460_pwc.c"
  #include "hc32f460_adc.c"
  #include "hc32f460_timera.c"
#endif
