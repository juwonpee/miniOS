AS = i686-elf-as
CC = i686-elf-g++
CC_INCLUDE = -Isrc/ -Isrc/include/ -Isrc/kernel -Isrc/driver
CCFLAGS = -O0 -ggdb -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -mgeneral-regs-only
LDFLAGS = -lgcc -nostdlib

SRC_DIR = src

BOOTLOADER_SOURCE = $(wildcard $(SRC_DIR)/bootloader/*.S)
BOOTLOADER_OBJ = $(BOOTLOADER_SOURCE:.S=.o)
C_SOURCES = $(wildcard $(SRC_DIR)/driver/*.cpp $(SRC_DIR)/include/*.cpp $(SRC_DIR)/kernel/*.cpp)
C_OBJ = $(C_SOURCES:.cpp=.o)
OBJECTS = $(BOOTLOADER_OBJ) $(C_OBJ)
OUTPUT = build/isodir/boot/miniOS.bin
OUTPUT_IMAGE = build/miniOS.iso

QEMU = qemu-system-i386
QEMU_ARGS = -M q35 -cpu pentium3 -m 1G # Use Q35 Type to default to ahci drive interface

all: image
	make clean
	
run: all
	$(QEMU) $(QEMU_ARGS) \
		-hda $(OUTPUT_IMAGE)\
		-nographic 

debug: all
	$(QEMU) $(QEMU_ARGS) \
		-hda $(OUTPUT_IMAGE)\
		-nographic \
		-S -s


binary: bootloader $(C_OBJ)
	$(CC) -T src/linker.ld -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

bootloader:
	$(CC) $(CC_INCLUDE) -g -c $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

clean:
	find . -name "*.o" | xargs -r rm 

image: binary
	make check_multiboot
	grub-mkrescue --modules="normal part_msdos ext2 multiboot multiboot2" -o $(OUTPUT_IMAGE) build/isodir

%.o: %.cpp
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS)

check_multiboot: 
	grub-file --is-x86-multiboot2 $(OUTPUT)
