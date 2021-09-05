CC = i686-elf-gcc
AS = i686-elf-as
CC_INCLUDE = -Isrc/ -Isrc/include/ -Isrc/kernel -Isrc/driver
CCFLAGS = -Og -ggdb -ffreestanding -Wall -Wextra -g
LDFLAGS = -lgcc -nostdlib

SRC_DIR = src

BOOTLOADER_SOURCE = $(wildcard $(SRC_DIR)/bootloader/*.s)
BOOTLOADER_OBJ = $(BOOTLOADER_SOURCE:.s=.o)
C_SOURCES = $(wildcard $(SRC_DIR)/driver/*.c $(SRC_DIR)/include/*.c $(SRC_DIR)/kernel/*.c)
C_OBJ = $(C_SOURCES:.c=.o)
OBJECTS = $(BOOTLOADER_OBJ) $(C_OBJ)
OUTPUT = build/isodir/boot/miniOS.bin

QEMU = qemu-system-i386 -cpu pentium

all: kernel clear image check_multiboot

binary: kernel clear

bootloader:
	$(AS) $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

kernel: bootloader $(C_OBJ)
	$(CC) -T src/linker.ld -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

clear:
	find . -name "*.o" | xargs -r rm 

image:
	grub-mkrescue -o build/miniOS.iso build/isodir


%.o: %.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS)
# special compiler command for interrupts, 80387 instructions not allowed in interrupt functions
$(SRC_DIR)/kernel/interrupt.o: $(SRC_DIR)/kernel/interrupt.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS) -mgeneral-regs-only


check_multiboot:
	grub-file --is-x86-multiboot $(OUTPUT)

	

run:
	$(QEMU) \
		-m 1G\
		-hda build/miniOS.iso \
		-nographic 

run_debug:	
	$(QEMU) \
		-m 1G\
		-hda build/miniOS.iso \
		-nographic \
		-S -s

run_no_grub:	
	$(QEMU) \
		-m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic 

run_no_grub_debug:	
	$(QEMU) \
		-m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic \
		-S -s
