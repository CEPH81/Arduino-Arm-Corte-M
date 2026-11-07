/* Air105: el programa empieza en 0x01001000 (el cargador de la ROM guarda
 * su cabecera en el primer sector de la flash, 0x01000000). 640 KB SRAM. */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x01001000, LENGTH = 4M - 8K   /* el resto: EEPROM emulada */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 640K
}
_Min_Heap_Size  = 0x1000;
_Min_Stack_Size = 0x1000;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
