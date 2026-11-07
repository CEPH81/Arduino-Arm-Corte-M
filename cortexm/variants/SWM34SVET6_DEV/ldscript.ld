/* SWM341xE: 512 KB flash en 0x00000000, 64 KB SRAM */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 508K   /* el resto: EEPROM emulada */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
