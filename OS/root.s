.section .text
.syntax unified
.code 32
.globl _start

// ARM Vector Table
.align 5                 @ Align to 32 bytes (2^5)
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
    mov r0, #0x1F       @ System mode (0b11111)
    msr CPSR.M, r0      @ Write to CPSR to switch mode

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
    dmb @ Data Memory Barrier
    isb @ Instruction Synchronization Barrier

    // Set up the ARM vector table via VBAR (Vector Base Address Register)
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0  @ Write the address of the vector table to VBAR

    // Jump to the C main function
    bl main
    
    // If main returns, loop forever
hang:
    b hang

undefined_handler:
    b hang

swi_handler:
    b hang

prefetch_handler:
    b hang

data_handler:
    b hang

irq_handler:
    push {r0-r12, lr}       @ Save all registers 
    bl timer_irq_handler    @ Call C handler 
    pop {r0-r12, lr}        @ Restore registers 
    subs pc, lr, #4         @ Return from interrupt 
    b hang

fiq_handler:
    b hang

.globl enable_irq
enable_irq:
    mov r0, #0
    msr CPSR.I, r0  @ Enable IRQs by clearing the I bit in CPSR
    bx lr

// Stack space allocation
.section .bss
.align 4            @ Align to 16 bytes (2^4)
_stack_bottom:
    .skip 0x2000    @ 8KB stack space
_stack_top:
