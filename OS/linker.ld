ENTRY(_start)

MEMORY
{
    ram : ORIGIN = MEM_ADDR, LENGTH = 64K
}

SECTIONS
{
    . = ORIGIN(ram);

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > ram

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > ram

    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > ram

    _stack_bottom = __bss_end__;
    _stack_top = ORIGIN(ram) + LENGTH(ram);

    .p1_image P1_ADDR :
    {
        *(.p1_image)
    }

    .p2_image P2_ADDR :
    {
        *(.p2_image)
    }
}
