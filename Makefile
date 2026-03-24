TARGET ?= versatilepb
BUILD_SH := ./build/build_os.sh
BUILD_P1_SH := ./build/build_process_1.sh
BUILD_P2_SH := ./build/build_process_2.sh

qemu:
	@echo "\n Building project for QEMU..."
	@echo "\n=== OS (includes P1 and P2) ==="
	TARGET=versatilepb $(BUILD_SH)

bbb:
	@echo "\n Building project for BeagleBone..."
	@echo "\n=== OS (includes P1 and P2) ==="
	TARGET=beaglebone $(BUILD_SH)

.PHONY: qemu bbb