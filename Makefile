make clean:
	rm -r build/*
	mkdir build/object_files

make all: bootloader kernel check_multiboot
	echo Make all
make bootloader:
	echo Making bootloader
	nasm -i src/bootloader/ src/bootloader/multiboot.asm -f elf -o build/object_files/multiboot.o	

make kernel:
	echo Making kernel
	gcc -Isrc/include/** -m32 -c src/init.c -o build/object_files/init.o -ffreestanding -O2 -Wall -nostdlib -Wextra
	gcc -m32 -T src/linker.ld -o build/miniOS.bin -ffreestanding -O2 -nostdlib build/object_files/multiboot.o build/object_files/init.o lgcc

make check_multiboot:
	grub-file --is-x86-multiboot build/miniOS.bin
	echo $?