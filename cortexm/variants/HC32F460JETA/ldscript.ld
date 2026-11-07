/* HC32F460JETA: 512 KB flash en 0x00000000, 188 KB SRAM contigua
 * (SRAMH 32 KB en 0x1FFF8000 + SRAM1/2/3 desde 0x20000000).
 * 0x000-0x3FF: tabla de vectores · 0x400-0x41F: palabras ICG (todas 0xFF) */
MEMORY
{
  VECT  (rx)  : ORIGIN = 0x00000000, LENGTH = 0x400
  ICG   (rx)  : ORIGIN = 0x00000400, LENGTH = 0x20
  FLASH (rx)  : ORIGIN = 0x00000420, LENGTH = 512K - 0x420
  RAM   (xrw) : ORIGIN = 0x1FFF8000, LENGTH = 188K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", VECT);

SECTIONS
{
  .icg_sec : { KEEP(*(.icg_sec)) } >ICG
}
INCLUDE cortexm_sections.ld
