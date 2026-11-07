/* AT32F403ACGU7: 1024 KB flash, 96 KB SRAM (por defecto; 224 KB cambiando EOPB0) */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1022K   /* el resto: EEPROM emulada */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 96K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
