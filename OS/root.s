.section .text
.syntax unified
.code 32
.globl _start

_start:
b reset_handler

// ARM Vector Table
.align 5                 @ Align to 32 bits (2^5)
vector_table:
    b reset_handler      @ 0x00: Reset
    b undefined_handler  @ 0x04: Undefined Instruction
    b swi_handler        @ 0x08: Software Interrupt (SWI)
    b prefetch_handler   @ 0x0C: Prefetch Abort
    b data_handler       @ 0x10: Data Abort
    b .                  @ 0x14: Reserved
    b irq_handler        @ 0x18: IRQ (Interrupt Request)
    b fiq_handler        @ 0x1C: FIQ (Fast Interrupt Request)

reset_handler:
    // Set CPU to System mode
    mrs r0, CPSR
    mov r1, #0x1F   @ System mode (0b11111)
    orr r0, r0, r1
    msr CPSR, r0    @ Write to CPSR to switch mode

    // Set the initial stack pointer for the OS
    ldr sp, =_stack_top

    // Clear the .bss section
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0
clear_bss:
    cmp r0, r1          @ While the current address is less than the end of .bss
    it lt
    strlt r2, [r0], #4  @ Clear current location and go to the next (increment by 4 bytes)
    blt clear_bss

    // Place a memory barrier
    dsb @ Data Synchronization Barrier
    isb @ Instruction Synchronization Barrier

    // Set up the ARM vector table via VBAR (Vector Base Address Register)
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0  @ Write the address of the vector table to VBAR

    // Jump to the C main function
    bl kernel_init
    
    // If main returns, loop forever
hang:
    b hang

undefined_handler:
    bl print_registers
    bl print_pcb
    b hang

swi_handler:
    bl print_registers
    bl print_pcb
    b hang

prefetch_handler:
    bl print_registers
    bl print_pcb
    b hang

data_handler:
    bl print_registers
    bl print_pcb
    b hang

irq_handler:
    bl timer_irq_handler
    bl context_switch
    b hang

fiq_handler:
    bl print_registers
    bl print_pcb
    b hang

.globl enable_irq
enable_irq:
    mrs r0, CPSR
    mov r1, #0xFFFFFF7F
    and r0, r0, r1
    msr CPSR, r0    @ Enable IRQs by clearing the I bit in CPSR
    bx lr

.globl disable_irq
disable_irq:
    mrs r0, CPSR
    mov r1, #0x80
    orr r0, r0, r1
    msr CPSR, r0    @ Disable IRQs by setting the I bit in CPSR
    bx lr

// Reserve a contiguous region for the process stack
.section .bss
.align 4            @ Align to 16 bits (2^4)
_stack_bottom:
    .skip 0x2000    @ 8KB stack space
_stack_top:
