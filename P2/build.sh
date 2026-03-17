#!/bin/bash

# Build Script for Process 2
# Exit immediately if a command exits with a non-zero status
set -e

# Run from script directory so paths work from anywhere
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo
echo "Building Process 2..."

TARGET="${TARGET:-versatilepb}"   # default target
echo "  Selected TARGET=${TARGET}"

# Define commands and flags
CC="arm-none-eabi-gcc"
AS="arm-none-eabi-as"
LD="arm-none-eabi-ld"
LDFLAGS="-T linker.ld"
OBJCOPY="arm-none-eabi-objcopy"

case "$TARGET" in
  versatilepb)
    CFLAGS="-DPLATFORM_VERSATILEPB"
    RUN_CMD="qemu-system-arm -M versatilepb -nographic -kernel main.elf"
    ;;
  beaglebone)
    CFLAGS="-mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -DPLATFORM_BEAGLEBONE"
    RUN_CMD=""  # none - deploy to real hardware
    ;;
  *)
    echo "Unknown target: $TARGET"
    exit 1
    ;;
esac

# Remove previous compiled objects and binaries
echo "  Cleaning up previous build files..."
rm -f *.o ./../lib/*.o main.elf main.bin

echo "  Assembling root.s..."
$AS -o root.o root.s

echo "  Compiling os..."
$CC -c $CFLAGS -o ./../lib/os.o ./../lib/os.c

echo "  Compiling library..."
$CC -c $CFLAGS -o ./../lib/stdio.o ./../lib/stdio.c

echo "  Compiling main.c..."
$CC -c $CFLAGS -o main.o main.c

echo "  Linking object files..."
$LD $LDFLAGS -o main.elf root.o ./../lib/os.o ./../lib/stdio.o main.o

echo "  Converting ELF to binary..."
$OBJCOPY -O binary main.elf main.bin

if [ "$TARGET" = "versatilepb" ]; then
  echo "  Build complete for VerstatilePB. Run with: $RUN_CMD"
else
  echo "  Build complete for BeagleBone."
fi
