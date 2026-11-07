/*
 * vendor_at32.c - Compila solo los drivers de Artery que usa el núcleo
 * (reloj, flash y alimentación) y solo cuando la placa es AT32.
 */
#if defined(AT32F435_437)
  #include "at32f435_437_crm.c"
  #include "at32f435_437_flash.c"
  #include "at32f435_437_pwc.c"
#elif defined(AT32F403A_407)
  #include "at32f403a_407_crm.c"
  #include "at32f403a_407_flash.c"
  #include "at32f403a_407_pwc.c"
#endif
