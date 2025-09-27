/* ---------- Memory Regions ---------- */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Initial stack pointer di top of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

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

    /* Kode program & read-only data */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;               /* akhir .text di FLASH */
    } > FLASH

    /* Simbol untuk proses copy data dari FLASH ke RAM */
    _data_load_start = LOADADDR(.data);

    /* Data section di RAM, isi di-*load* dari FLASH */
    .data : AT ( _etext )
    {
        . = ALIGN(4);
        _data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _data_end = .;
    } > RAM

    /* Variabel tak terinisialisasi di RAM */
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

    /* Stack opsional */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _stack_start = .;
        . = . + 0x2000;     /* reserve 8 KB stack */
        _stack_end = .;
    } > RAM

    /* Hapus section yang tidak dipakai */
    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.gnu.debuglink)
        *(.note.gnu.build-id)
    }
}
