/* STM32H503 genérica */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
}
_Min_Heap_Size  = 0x400;
_Min_Stack_Size = 0x800;
REGION_ALIAS("VECTORS", FLASH);
INCLUDE cortexm_sections.ld
