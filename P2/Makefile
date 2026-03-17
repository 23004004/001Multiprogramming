TARGET ?= versatilepb
BUILD_SH := ./build.sh

qemu:
	@echo "\n=== Process 2 ==="
	TARGET=versatilepb $(BUILD_SH)

bbb:
	@echo "\n=== Process 2 ==="
	TARGET=beaglebone $(BUILD_SH)

.PHONY: qemu bbb