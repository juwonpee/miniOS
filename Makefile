make clean:
	rm -r build/*
	mkdir build/object_files


make bootloader:
	i386-elf-as src/bootloader/bootloader.s -o build/object_files/bootloader.o

make kernel:
	# i386-elf-gcc -static -w \
	# 	-I src/include/ \
	# 	-I src/print/ \
	# 	-I src/newlib-cygwin \
	# 	-m32 -c src/kernel.c -o build/object_files/kernel.o -ffreestanding -O2 -Wall -nostdlib -Wextra

	i386-elf-gcc -c src/test_kernel.c -o build/object_files/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i386-elf-gcc -m32 -T src/linker.ld -o build/miniOS.bin -ffreestanding -O2 -nostdlib build/object_files/bootloader.o build/object_files/kernel.o -lgcc

make check_multiboot:
	grub-file --is-x86-multiboot build/miniOS.bin
	echo $?

make run:
	qemu-system-x86_64 \
		-M q35 -m 256 \
		-kernel build/miniOS.bin \
		-vnc :0