/* AT32F437ZMT7: 4032 KB flash, 384 KB SRAM (configuración por defecto de EOPB0) */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 4028K   /* el resto: EEPROM emulada */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 384K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
