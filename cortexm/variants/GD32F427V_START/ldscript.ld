/* GD32F427VKT6: 3072 KB flash, 192 KB SRAM contigua (+64 KB TCM en 0x10000000) */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2816K   /* el resto: EEPROM emulada */
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 192K
  TCM   (rw)  : ORIGIN = 0x10000000, LENGTH = 64K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
