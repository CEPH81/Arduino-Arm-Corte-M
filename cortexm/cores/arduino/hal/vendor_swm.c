/*
 * vendor_swm.c - Compila los drivers de Synwit que usa el núcleo
 * (PORT, GPIO, UART y, en SWM181, el multiplexor de IRQ).
 */
#if defined(CHIP_SWM181)
  #include "SWM181_port.c"
  #include "SWM181_gpio.c"
  #include "SWM181_uart.c"
  #include "SWM181_irqmux.c"
#elif defined(CHIP_SWM341)
  #include "SWM341_port.c"
  #include "SWM341_gpio.c"
  #include "SWM341_uart.c"
  #include "SWM341_adc.c"
  #include "SWM341_pwm.c"
  #include "SWM341_flash.c"      /* Flash_Param_at_xMHz(), usado por SystemInit */
#endif
