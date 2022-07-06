AS = i686-elf-as
CC = i686-elf-gcc
CC_INCLUDE = -Isrc/ -Isrc/include/ -Isrc/kernel -Isrc/driver
CCFLAGS = -O0 -ggdb -ffreestanding -Wall
LDFLAGS = -lgcc -nostdlib

SRC_DIR = src

BOOTLOADER_SOURCE = $(wildcard $(SRC_DIR)/bootloader/*.S)
BOOTLOADER_OBJ = $(BOOTLOADER_SOURCE:.S=.o)
C_SOURCES = $(wildcard $(SRC_DIR)/driver/*.c $(SRC_DIR)/include/*.c $(SRC_DIR)/kernel/*.c)
C_OBJ = $(C_SOURCES:.c=.o)
OBJECTS = $(BOOTLOADER_OBJ) $(C_OBJ)
OUTPUT = build/isodir/boot/miniOS.bin
OUTPUT_IMAGE = build/miniOS.iso

QEMU = qemu-system-i386 -cpu pentium

all: image
	make clean

binary: bootloader $(C_OBJ)
	$(CC) -T src/linker.ld -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

bootloader:
	$(CC) $(CC_INCLUDE) -g -c $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

clean:
	find . -name "*.o" | xargs -r rm 

image: binary
	make check_multiboot
	grub-mkrescue --modules="normal part_msdos ext2 multiboot multiboot2" -o $(OUTPUT_IMAGE) build/isodir

# special compiler command for interrupt.c, 80387 instructions not allowed in interrupt functions
$(SRC_DIR)/kernel/interrupt.o: $(SRC_DIR)/kernel/interrupt.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS) -mgeneral-regs-only
%.o: %.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS)

check_multiboot: 
	grub-file --is-x86-multiboot2 $(OUTPUT)

run:
	$(QEMU) \
		-m 128M -M q35 -cpu pentium3 -m 1G\
		-hda $(OUTPUT_IMAGE)\
		-nographic 

debug:
	$(QEMU) \
		-m 128M -M q35 -cpu pentium3 -m 1G\
		-hda $(OUTPUT_IMAGE)\
		-nographic \
		-S -s
