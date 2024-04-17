#include "driver/serial.hpp"
#include "driver/io.hpp"
#include "kernel/error.hpp"
#include "include/stdarg.hpp"
#include "include/string.hpp"

bool serial::serialInit() {
    io::outb(COMport + 1, 0x01);    // Enable data available interrupt
    io::outb(COMport + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    io::outb(COMport + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    io::outb(COMport + 1, 0x00);    //                  (hi byte)
    io::outb(COMport + 3, 0x03);    // 8 bits, no parity, one stop bit
    io::outb(COMport + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    io::outb(COMport + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    io::outb(COMport + 4, 0x1E);    // Set in loopback mode, test the serial chip
    io::outb(COMport + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if (io::inb(COMport) != 0xAE) { // If not the same packet as recieved.
        error::panic((char*)"IO Error: Did not recieve the same packet during initialization\n");
    }
    io::outb(COMport + 4, 0x0F);    // Return port to normal state

    printf((char*)"\n");
    printf((char*)"Serial port Initialized\n");
    return true;
}


void serial::printChar(char character) {
    while (true) {
        if (io::inb(COMport + 5) & 0x20) {  // If port is available
            io::outb(COMport, character);
            return;
        }
    }
}

void serial::printf(const char* format, ...) {
    const char* traverse;
    int64_t index;
    char* str;

    // init arguments
    va_list arg;
    va_start(arg, format);

    for (traverse = format; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {
			if (*traverse == '\0') {
				return;
			}
			printChar(*traverse);
			traverse++;
		}

		traverse++;

		// Module 2: Fetching and executing arguments
		char tempString[256];
		switch(*traverse) {
			case 'c': index = va_arg(arg, intptr_t);													// Char argument
				printChar(index);
				break;
			case 'd': index = va_arg(arg, intptr_t);												// Integer argument
				printf(stringUtils::itoa(index, tempString, 10));
				break;
			case 'x': index = va_arg(arg, intptr_t);												// Hexadecimal argument
				printf(stringUtils::itoa(index, tempString, 16));
				break;
			case 'o': index = va_arg(arg, intptr_t);												// Octal argument
				printf(stringUtils::itoa(index, tempString, 8));
				break;
			case 's': str = va_arg(arg, char*);													// String argument
				if (str != nullptr) printf(str);
				break;
		}
    }


}


