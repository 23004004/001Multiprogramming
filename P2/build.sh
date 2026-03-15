#!/bin/bash

# Build and Run Script for Interrupt Handler Lab
# BeagleBone Black Bare-Metal Application

# Exit immediately if a command exits with a non-zero status
set -e

# Run from script directory so paths work from anywhere
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Remove previous compiled objects and binaries
echo "Cleaning up previous build files..."
rm -f *.o ./../lib/*.o main.elf main.bin

echo "Assembling startup.s..."
arm-none-eabi-as -o root.o root.s

# Esto es nuevo para compilarlo y saber de donde sacarlo, esto es mas build
echo "Compiling os..."
arm-none-eabi-gcc -c -o ./../lib/os.o ./../lib/os.c

# Esto tambien nuevo para compilarlo y saber de donde sacarlo, esto es mas build
echo "Compiling library..."
arm-none-eabi-gcc -c -o ./../lib/stdio.o ./../lib/stdio.c

#Esto de aqui para abajo ya estaba

echo "Compiling main.c..."
arm-none-eabi-gcc -c -o main.o main.c

echo "Linking object files..."
arm-none-eabi-ld -T linker.ld -o main.elf root.o ./../lib/os.o ./../lib/stdio.o main.o

echo "Converting ELF to binary..."
arm-none-eabi-objcopy -O binary main.elf main.bin

echo "Running QEMU..."
qemu-system-arm -M versatilepb -nographic -kernel main.elf
