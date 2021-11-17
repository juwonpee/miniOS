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

__attribute__ ((fastcall)) void printf(char* format, ...) {
	// int x = 0;
    // while (format[x] != '\0') {
	// 	printChar(format[x]);
    //     x++;
    // }

	char* traverse;
	int64_t i;
	char* s;
	
	// Module 1: Init arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0'; traverse++) {
		while (*traverse != '%') {
			if (*traverse == '\0') {
				return;
			}
			printChar(*traverse);
			traverse++;
		}

		traverse++;

		// Module 2: Fetching and executing arguments
		char tempString[64];
		switch(*traverse) {
			case 'c': i = va_arg(arg, intptr_t);													// Char argument
				printChar(i);
				break;
			case 'd': i = va_arg(arg, intptr_t);												// Integer argument
				printf(itoa(i, tempString, 10));
				break;
			case 'x': i = va_arg(arg, intptr_t);												// Hexadecimal argument
				printf(itoa(1, tempString, 16));
				break;
			case 'o': i = va_arg(arg, intptr_t);												// Octal argument
				printf(itoa(i, tempString, 8));
				break;
			case 's': s = va_arg(arg, char*);													// String argument
				printf(s);
				break;
		}
	}

	// Close argument list
	va_end(arg);
}

__attribute__ ((fastcall)) void println(char* string) {
    printf(string);
    printf("\n");
}

__attribute__ ((fastcall)) void printsn(char* string) {
    printf("\33[2K\r");
    printf(string);
}
