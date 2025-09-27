/* ---------- Memory Regions ---------- */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* simbol penting untuk startup.s */
_estack = ORIGIN(RAM) + LENGTH(RAM);   /* initial SP di top of RAM */

/* ---------- Sections ---------- */
SECTIONS
{
    /* Vector Table */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Code & read-only data */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Symbol untuk copy data dari FLASH ke RAM */
    PROVIDE(_data_load_start = LOADADDR(.data));

    /* Data section di RAM dengan isi dari FLASH */
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        . = ALIGN(4);
        _data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _data_end = .;
    } > RAM AT > FLASH

    /* BSS section di RAM */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM

    /* Optional: reserved stack space (tidak mempengaruhi _estack) */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _stack_start = .;
        . = . + 0x2000;      /* reserve 8 KB */
        _stack_end = .;
    } > RAM

    /* Buang section yang tidak dipakai */
    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.gnu.debuglink)
        *(.note.gnu.build-id)
    }
}
