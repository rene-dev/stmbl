MEMORY
{
   FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 32K
   RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 10K
}

ENTRY(main)

SECTIONS
{
   .  = 0x0;         /* From 0x00000000 */
   .text :
   {
      *(vectors)    /* Vector table */
      *(.text)      /* Program code */
      *(.rodata)    /* Read only data */
   } >FLASH

   .  = 0x20000000;  /* From 0x20000000 */
   .data :
   {
      *(.data)      /* Data memory */
   } >RAM AT > FLASH

   .bss :
   {
      *(.bss)       /* Zero-filled run time allocate data memory */
   } >RAM AT > FLASH
}
