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
    outb(COMport, character);
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

