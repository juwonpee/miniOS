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

all: binary check_multiboot image clear

binary: kernel clear

bootloader:
	$(CC) $(CC_INCLUDE) -g -c $(BOOTLOADER_SOURCE) -o $(BOOTLOADER_OBJ)

kernel: bootloader $(C_OBJ)
	$(CC) -T src/linker.ld -o $(OUTPUT) $(OBJECTS) $(LDFLAGS)

clear:
	find . -name "*.o" | xargs -r rm 

image: check_multiboot
	grub-mkrescue --modules="normal part_msdos ext2 multiboot multiboot2" -o $(OUTPUT_IMAGE) build/isodir
# #	dd if=/dev/zero of=$(OUTPUT_IMAGE) bs=512 count=100000
# 	fdisk $(OUTPUT_IMAGE)
# 	losetup /dev/loop0 $(OUTPUT_IMAGE)
# 	losetup /dev/loop1 $(OUTPUT_IMAGE) -o 1048576
# 	mke2fs /dev/loop0
# 	mkfs.vfat -F 32 /dev/loop1
# 	mmd -i /dev/loop1 ::boot
# 	mcopy -i /dev/loop1 build/isodir/boot/miniOS.bin ::boot/miniOS.bin
# 	mmd -i /dev/loop1 ::boot/grub
# 	mcopy -i /dev/loop1 build/isodir/boot/grub/grub.cfg ::boot/grub/grub.cfg
# 	grub-install --target=i386-pc --force --root-directory=/mnt --no-floppy --modules="normal part_msdos ext2 multiboot multiboot2" /dev/loop0
# 	losetup -d /dev/loop0
# 	losetup -d /dev/loop1

fs:
	dd if=/dev/zero of=build/fs.hdd bs=1M count=50
	fdisk build/fs.hdd
	mkfs.vfat build/fs.hdd
	mkdir tempMount
	mount build/fs.hdd tempMount
	cp -r build/test_files tempMount
	umount tempMount
	rm -r tempMount
#ontcw

%.o: %.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS)
# special compiler command for interrupt.c, 80387 instructions not allowed in interrupt functions
$(SRC_DIR)/kernel/interrupt.o: $(SRC_DIR)/kernel/interrupt.c
	$(CC) $(CC_INCLUDE) -c $< -o $@ $(CCFLAGS) -mgeneral-regs-only


check_multiboot:
	grub-file --is-x86-multiboot2 $(OUTPUT)

run:
	$(QEMU) \
		-m 256M\
		-hda $(OUTPUT_IMAGE) \
		-hdb build/fs.hdd \
		-nographic 

run_debug:	
	$(QEMU) \
		-m 256M\
		-hda $(OUTPUT_IMAGE) \
		-hdb build/fs.hdd \
		-nographic \
		-S -s

run_no_grub:	
	$(QEMU) \
		-m 256M\
		-kernel $(OUTPUT) \
		-hdb build/fs.hdd \
		-nographic 

run_no_grub_debug:	
	$(QEMU) \
		-m 256M\
		-kernel $(OUTPUT) \
		-hdb build/fs.hdd \
		-nographic \
		-S -s
