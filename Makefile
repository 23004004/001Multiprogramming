TARGET ?= versatilepb
BUILD_SH := ./build.sh
BUILD_P1_SH := ./P1/build.sh
BUILD_P2_SH := ./P2/build.sh

qemu:
	@echo "\n=== Process 1 ==="
	TARGET=versatilepb $(BUILD_P1_SH)

	@echo "\n=== Process 2 ==="
	TARGET=versatilepb $(BUILD_P2_SH)

	@echo "\n=== OS ==="
	TARGET=versatilepb $(BUILD_SH)

bbb:
	@echo "\n=== Process 1 ==="
	TARGET=beaglebone $(BUILD_P1_SH)

	@echo "\n=== Process 2 ==="
	TARGET=beaglebone $(BUILD_P2_SH)

	@echo "\n=== OS ==="
	TARGET=beaglebone $(BUILD_SH)

.PHONY: qemu bbb