ENTRY(_start)

MEMORY
{
    ram : ORIGIN = MEM_ADDR, LENGTH = 8K
}

SECTIONS
{
    . = ORIGIN(ram); /* Starting address in RAM */

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

    _stack_bottom = __bss_end;
    _stack_top = ORIGIN(ram) + LENGTH(ram);
}
