MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } >FLASH

    _data_load = LOADADDR(.data);

PROVIDE(_data_load_start = LOADADDR(.data));

   .data : AT(ADDR(.text) + SIZEOF(.text))     /* atau gunakan LOADADDR nanti */
   {
       . = ALIGN(4);
       _data_start = .;        /* awal data di RAM */
       *(.data)
       *(.data*)
       . = ALIGN(4);
       _data_end = .;          /* akhir data di RAM */
    } >RAM AT>FLASH

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } >RAM

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _estack = .;
        . = . + 0x2000; /* 8KB stack */
        _stack_top = .;
    } >RAM

    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.gnu.debuglink)
        *(.note.gnu.build-id)
    }
}

