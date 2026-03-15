ENTRY(_start)

SECTIONS
{
    . = 0x00200000; /* Starting address in RAM */

    .text :
    {
        *(.text*)
    }

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        *(.data*)
    }

    .bss :
    {
        __bss_start = .;
        *(.bss*)
        __bss_end = .;
    }
}
