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



#include "print.h"

uint16_t COMport;
char charBuffer[256];
uint8_t charBuffer_index;

void serialInit(uint16_t _COMport) {
    COMport = _COMport;
    outb(COMport + 1, 0x00);    // Disable all interrupts
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
    if (inb(COMport + 5) & 1) {
        return inb(COMport);
    }
    return 0;
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

void serial_interval_read() {
    char tempChar = serialInByte();
    if (tempChar != 0) {
        charBuffer[charBuffer_index] = tempChar;
        charBuffer_index++;
    }
}

char* read(char* buffer) {
    buffer = charBuffer;
    if (charBuffer_index == 255) {
        buffer[255] = '\0';
    }
    else {
        buffer[charBuffer_index] = '\0';
    }
    charBuffer_index = 0;
}

void print(char* string) {
    int x = 0;
    while (string[x] != '\0') {
        if (inb(COMport + 5) & 0x20) {
            outb(COMport, string[x]);
        }
        outb(COMport + 5, string[x]);
        x++;
    }
}

void println(char* string) {
    print(string);
    print("\n");
}

