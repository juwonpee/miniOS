CC = i686-elf-gcc
AS = i686-elf-as
CC_INCLUDE = -Isrc/ -Isrc/include/ -Isrc/kernel -Isrc/driver
CCFLAGS = -Og -ggdb -ffreestanding -Wall -Wextra -lgcc -nostdlib
LDFLAGS = -

SRC_DIR = src
OBJ_DIR = build/object_files

BOOTLOADER_SOURCE = $(wildcard $(SRC_DIR)/bootloader/*.s)
BOOTLOADER_OBJ = $(OBJ_DIR)/bootloader.o
C_SOURCES = $(wildcard $(SRC_DIR)/driver/*.c $(SRC_DIR)/include/*.c $(SRC_DIR)/kernel/*.c)
C_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS = $(wildcard $(OBJ_DIR)/*.o)
OUTPUT = build/isodir/boot/miniOS.bin



kernel:
	$(CC) $(CC_INCLUDE) -T src/linker.ld -o $(OUTPUT) $(C_SOURCES) $(CCFLAGS)
	grub-mkrescue -o build/miniOS.iso build/isodir

bootloader:
	$(AS) $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

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

all: bootloader kernel check_multiboot
