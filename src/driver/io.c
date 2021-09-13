/*
   Copyright [2021] [juwonpee]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "io.h"

char serial_input_buffer[256];
uint8_t serial_input_buffer_index;

void serialInit() {
    outb(COMport + 1, 0x01);    // Enable data available interrupt
    outb(COMport + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COMport + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    outb(COMport + 1, 0x00);    //                  (hi byte)
    outb(COMport + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COMport + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COMport + 4, 0x0B);    // IRQs enabled, RTS/DSR set

    outb(COMport + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COMport + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
    
    if (inb(COMport) != 0xAE) { // If not the same packet as recieved.
        panic();
    }
    outb(COMport + 4, 0x0F);    // Return port to normal state

    println("");
    println("Serial port Initialized");
    return;
}

char serialInByte() {
    // if (inb(COMport + 5) & 1) {
    //     return inb(COMport);
    // }
    // return 0;
    return inb(COMport);
}

void serialOutByte(char character) {
    while (1) {
        if ((inb(COMport + 5) & 0x20) == 0) {                           // If output buffer is empty;
            outb(COMport, character);
            return;
        }
        else {
            io_wait();
        }
    }
}

void serial_interrupt_read() {
    char tempChar = inb(COMport);
	char tempString[64];
    switch (tempChar) {
        case 0x0D:                                                   // Change carrige return to new line
            serial_input_buffer[serial_input_buffer_index] = '\0';
			serial_input_buffer[serial_input_buffer_index + 1] = '\n';
			printChar(serial_input_buffer[serial_input_buffer_index]);
			printChar(serial_input_buffer[serial_input_buffer_index + 1]);
			break;
        case 0x7F:													// Delete to backspace
            serial_input_buffer[serial_input_buffer_index] = 0x08;
			printChar(serial_input_buffer[serial_input_buffer_index]);
			break;
		default:
    		serial_input_buffer[serial_input_buffer_index] = tempChar;
			printChar(serial_input_buffer[serial_input_buffer_index]);
    }

    serial_input_buffer_index++;
}

char* serial_get_buffer(char* buffer) {
    (*buffer) = (*serial_input_buffer);
    if (serial_input_buffer_index == 255) {
        buffer[255] = '\0';
    }
    else {
        buffer[serial_input_buffer_index] = '\0';
    }
}

uint8_t inb(uint16_t port) {
    uint8_t returnVal;
    asm volatile (
        "inb %1, %0" 
        : "=a" (returnVal)
        : "Nd" (port)
    );
    return returnVal;
}

void outb(uint16_t port, uint8_t value) {
    asm volatile (
        "outb %0, %1"
        : 
        : "a" (value), "Nd" (port)
    );
}

uint16_t inw(uint16_t port) {
    uint16_t returnVal;
    asm volatile (
        "inw %1, %0"
        : "=a" (returnVal)
        : "Nd" (port)
    );
}

void outw(uint16_t port, uint16_t value) {
    asm volatile (
        "outw %0, %1"
        : 
        : "a" (value), "Nd" (port)
    );
}

void io_wait() {
    outb(0x80, 0);
}