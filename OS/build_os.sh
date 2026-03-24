#!/bin/bash

# Build Script for OS
# Exit immediately if a command exits with a non-zero status
set -e

# Run from script directory so paths work from anywhere
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"
mkdir -p bin # Ensure bin directory exists for output files

TARGET="${TARGET:-versatilepb}"   # default target
echo "  Selected TARGET=${TARGET}"

# Define commands and flags
CC="arm-none-eabi-gcc"
AS="arm-none-eabi-as"
LD="arm-none-eabi-ld"
OBJCOPY="arm-none-eabi-objcopy"

case "$TARGET" in
  versatilepb)
    CFLAGS="-DPLATFORM_VERSATILEPB"
    LDFLAGS="-T linker.ld --defsym=MEM_ADDR=0x00000000 --defsym=P1_ADDR=0x00100000 --defsym=P2_ADDR=0x00200000"
    RUN_CMD="qemu-system-arm -M versatilepb -nographic -kernel bin/os.elf"
    ;;
  beaglebone)
    CFLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -DPLATFORM_BEAGLEBONE"
    LDFLAGS="-T linker.ld --defsym=MEM_ADDR=0x82000000 --defsym=P1_ADDR=0x82100000 --defsym=P2_ADDR=0x82200000"
    RUN_CMD=""  # none, since we will run on real hardware
    ;;
  *)
    echo "Unknown target: $TARGET"
    exit 1
    ;;
esac

# Remove previous compiled objects and binaries
echo "  Cleaning up previous build files..."
rm -f bin/*.o bin/os.elf bin/os.bin

# Compilar primer P1 y P2 para que ya esten en memoria
TARGET=$TARGET ../P1/build_process_1.sh
TARGET=$TARGET ../P2/build_process_2.sh

echo ""
echo "Building OS..."

echo "  Assembling root.s..."
$AS -o bin/root.o root.s

echo "  Assembling processes.s (embedding P1 and P2 binaries)..."
$AS -o bin/processes.o processes.s

echo "  Compiling kernel..."
$CC -c $CFLAGS -o bin/kernel.o kernel.c

echo "  Compiling uart driver..."
$CC -c $CFLAGS -o bin/uart.o ../drivers/uart.c

echo "  Compiling library..."
$CC -c $CFLAGS -o bin/stdio.o ../lib/stdio.c

echo "  Compiling stdlib..."
$CC -c $CFLAGS -o bin/stdlib.o ../lib/stdlib.c

echo "  Compiling debugger..."
$CC -c $CFLAGS -o bin/logs.o ../debugger/logs.c

echo "  Linking object files..."
$LD $LDFLAGS -o bin/os.elf bin/root.o bin/processes.o bin/kernel.o bin/uart.o bin/stdio.o bin/stdlib.o bin/logs.o

echo "  Converting ELF to binary..."
$OBJCOPY -O binary bin/os.elf bin/os.bin

echo "  Disassembling for debugging..."
arm-none-eabi-objdump -d bin/os.elf > bin/disasm.txt

echo "  Cleaning up compilation files..."
rm -f bin/*.o

if [ "$TARGET" = "versatilepb" ]; then
  echo "  Build complete for VerstatilePB. Run with: $RUN_CMD"
else
  echo "  Build complete for BeagleBone."
fi