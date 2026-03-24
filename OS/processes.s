// Process 1 image — placed at MEM_ADDR + 0x100000
.section .p1_image, "a"
.global p1_image_start
p1_image_start:
    .incbin "../build/bin/process_1.bin"
p1_image_end:

// Process 2 image — placed at MEM_ADDR + 0x200000
.section .p2_image, "a"
.global p2_image_start
p2_image_start:
    .incbin "../build/bin/process_2.bin"
p2_image_end:
