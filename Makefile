CC = i386-elf-gcc
AS = i386-elf-as
CC_INCLUDE = src/
CCFLAGS = -O0 -ffreestanding -Wall -Wextra -g
LDFLAGS = -lgcc -nostdlib -g

SRC_DIR = src/
ASM_DIR = src/asm/
OBJECT_DIR = build/object_files/
OUTPUT = build/isodir/boot/miniOS.bin
MODULES_DIR = $(wildcard $(SRC_DIR)*.c)
MODULES = $(notdir $(MODULES_DIR))
OBJECTS = $(wildcard $(OBJECT_DIR)*.o)



make bootloader:
	$(AS) $(ASM_DIR)bootloader.s -o $(OBJECT_DIR)bootloader.o

make kernel: bootloader $(MODULES)
	$(CC) -T src/linker.ld -o $(OUTPUT) -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc
	grub-file --is-x86-multiboot $(OUTPUT)
	grub-mkrescue -o build/miniOS.iso build/isodir

%.c: 
	$(CC) -I$(CC_INCLUDE) -c $(SRC_DIR)$@ -o $(OBJECT_DIR)$(addsuffix .o,$(basename $@)) $(CCFLAGS)
	
make run:
	qemu-system-i386 \
		-M q35 -m 1G\
		-cdrom build/miniOS.iso \
		-nographic 

make run_debug:	
	qemu-system-i386 \
		-M q35 -m 1G\
		-cdrom build/miniOS.iso \
		-nographic \
		-S -s

make run_no_grub:	
	qemu-system-i386 \
		-M q35 -m 1G\
		-kernel build/isodir/boot/miniOS.bin \
		-nographic 

make all: bootloader kernel check_multiboot
