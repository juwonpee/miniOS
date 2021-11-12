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


char readChar() {
    return inb(COMport);
}

void printChar(char character) {
    while (1) {
		if (inb(COMport + 5) & 0x20) {
			outb(COMport, character);
			return;
		}
	}
}

__attribute__ ((fastcall)) void print(char* format, ...) {
	char* traverse;
	uintptr_t i;
	char* s;
	
	// Module 1: Init arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0'; traverse++) {
		while (*traverse != '%') {
			printChar(*traverse);
			traverse++;
		}

		traverse++;

		// Module 2: Fetching and executing arguments
		switch(*traverse) {
			case 'c': i = va_arg(arg, int);														// Char argument
				printChar(arg);
				break;
			case 'd': i = va_arg(arg, int);														// Integer argument
				if (i < 10) {

				}
		}
	}
}

__attribute__ ((fastcall)) void println(char* string) {
    print(string);
    print("\n");
}

__attribute__ ((fastcall)) void printsn(char* string) {
    print("\33[2K\r");
    print(string);
}
