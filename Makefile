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


all: bootloader kernel clear_objects image check_multiboot

bootloader:
	$(AS) $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

kernel: $(C_OBJ)
	$(CC) -T src/linker.ld -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

clear_objects:
	find . -name "*.o" | xargs rm 

image:
	grub-mkrescue -o build/miniOS.iso build/isodir

%.o: %.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS)


check_multiboot:
	grub-file --is-x86-multiboot $(OUTPUT)

	

run:
	qemu-system-x86_64 \
		-M q35 -m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic 

run_debug:	
	qemu-system-x86_64 \
		-M q35 -m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic \
		-S -s

run_no_grub:	
	qemu-system-x86_64 \
		-M q35 -m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic 

