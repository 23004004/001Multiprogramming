# Multiprogramming

#### By @23004004 - Carlos Alvarez and @GabGP - Gabriel Garcia

This project implements a basic multiprogramming operating system capable of performing context switches between multiple processes using a Round-Robin scheduler. The system is designed to run on ARM-based architectures, specifically supporting **VersatilePB** and **BeagleBone Black**.

## Features

- **Round-Robin Scheduling**: Alternates execution between processes to ensure fair CPU distribution.
- **PCB Context Management**: Each process maintains its own Process Control Block (PCB) including register states (R0–R12), SP, LR, PC, and SPSR.
- **Banked Register Handling**: Safely accesses System-mode stacks from IRQ mode to ensure process isolation.
- **Millisecond Timer Support**: Configurable timer initialization for context switching.

## How to Run?

### To build

- `make bbb` BeagleBone Black
- `make qemu` VersatilePB
- `make qemu-debug` VersatilePB

Note: This program is meant to be ran on Linux and requires gcc-arm-none-aebi , gdb-and multiarch and optionally qemu.

### To run on BeagleBone Black

While connected to a terminal like CoolTerm via UART at 1152000 bauds, run:

- `loady 0x82000000`
- Send the file at ./build/bin/os.bin
- `go 0x82000000`

## Debugging

In another terminal:

- run `gdb-multiarch build/bin/os.elf`
- target remote localhost:3333

Helpful debugging options

- layout regs
- break [function name or number]
- continue
