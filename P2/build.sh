#!/bin/bash

# Build Script for Process 2
# Exit immediately if a command exits with a non-zero status
set -e

# Run from script directory so paths work from anywhere
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"
mkdir -p bin # Ensure bin directory exists for output files

echo
echo "Building Process 2..."

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
    LDFLAGS="-T linker.ld --defsym=MEM_ADDR=0x00200000"
    RUN_CMD="qemu-system-arm -M versatilepb -nographic -kernel bin/main.elf"
    ;;
  beaglebone)
    CFLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -DPLATFORM_BEAGLEBONE"
    LDFLAGS="-T linker.ld --defsym=MEM_ADDR=0x82200000"
    RUN_CMD=""  # none, since we will run on real hardware
    ;;
  *)
    echo "Unknown target: $TARGET"
    exit 1
    ;;
esac

# Remove previous compiled objects and binaries
echo "  Cleaning up previous build files..."
rm -f bin/*.o bin/main.elf bin/main.bin

echo "  Assembling root.s..."
$AS -o bin/root.o root.s

echo "  Compiling uart driver..."
$CC -c $CFLAGS -o bin/uart.o ./../drivers/uart.c

echo "  Compiling library..."
$CC -c $CFLAGS -o bin/stdio.o ./../lib/stdio.c

echo "  Compiling stdlib..."
$CC -c $CFLAGS -o bin/stdlib.o ./../lib/stdlib.c

echo "  Compiling main.c..."
$CC -c $CFLAGS -o bin/main.o main.c

echo "  Linking object files..."
$LD $LDFLAGS -o bin/main.elf bin/root.o bin/uart.o bin/stdio.o bin/stdlib.o bin/main.o

echo "  Converting ELF to binary..."
$OBJCOPY -O binary bin/main.elf bin/main.bin

if [ "$TARGET" = "versatilepb" ]; then
  echo "  Build complete for VerstatilePB. Run with: $RUN_CMD"
else
  echo "  Build complete for BeagleBone."
fi