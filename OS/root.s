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
    // Set stack pointer for IRQs
    msr CPSR, #0xD2 @ IRQ mode (0b10010) + IRQ/FIQ disabled
    ldr sp, =_stack_top

    // Set CPU to System mode
    msr CPSR, #0xDF @ IRQ mode (0b11111) + IRQ/FIQ disabled

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
    bl log_registers
    bl log_pcb
    b hang

swi_handler:
    bl log_registers
    bl log_pcb
    b hang

prefetch_handler:
    bl log_registers
    bl log_pcb
    b hang

data_handler:
    bl log_registers
    bl log_pcb
    b hang

irq_handler:
    // Save context
    ldr r0, =pcb
    ldr r1, =current_process
    ldr r2, [r1]
    mov r3, #76     @ sizeof(PCB) = 18 * 4 + 4
    mul r4, r3, r2  @ sizeof(PCB) * current_process
    add r5, r0, r4  @ &pcb[current_process]

    str lr, [r5, #56]   @ Save LR (R14)
    subs r6, lr, #4     @ Save PC (R15)
    str r6, [r5, #60]   @ PC = LR_irq - 4
    mrs r7, SPSR        @ Save SPSR
    str r7, [r5, #64]
    dsb

    // Update process state
    mov r0, r2
    mov r1, #3  @ PROCESS_WAITING
    bl update_process_state

    bl schedule
    bl timer_irq_handler

    // Update process state
    ldr r1, =current_process
    ldr r0, [r1]
    mov r1, #2  @ PROCESS_RUNNING
    bl update_process_state

    // Restore context
    ldr r0, =pcb
    ldr r1, =current_process
    ldr r2, [r1]
    mov r3, #76     @ sizeof(PCB) = 18 * 4 + 4
    mul r4, r3, r2  @ sizeof(PCB) * current_process
    add r5, r0, r4  @ &pcb[current_process]

    ldr r6, [r5, #64]   @ Restore SPSR
    msr SPSR, r6
    ldr r7, [r5, #60]   @ Restore LR (R14)
    adds lr, r7, #4     @ LR_irq = saved_PC + 4
    ldm r5, {r0-r12}    @ Restore R0-R12
    subs pc, lr, #4     @ Running the other process

    b hang

fiq_handler:
    bl log_registers
    bl log_pcb
    b hang

// Reserve a contiguous region for the process stack
.section .bss
.align 4            @ Align to 16 bits (2^4)
_stack_bottom:
    .skip 0x2000    @ 8KB stack space
_stack_top:
