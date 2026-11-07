/* SWM181xB: 64 KB flash en 0x00000000, 16 KB SRAM */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 64K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 16K
}
_Min_Heap_Size  = 0x200;
_Min_Stack_Size = 0x600;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
