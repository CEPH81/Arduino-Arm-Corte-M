/*
 * vendor_air105.c - Compila los drivers de MegaHunt que usa el núcleo
 * (relojes y UART).
 */
#if defined(AIR105)
  #include "air105_sysctrl.c"
  #include "air105_uart.c"
  #include "air105_qspi.c"
  #include "air105_cache.c"
#endif
