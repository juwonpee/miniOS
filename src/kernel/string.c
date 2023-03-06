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



#include "string.h"


char* itoa(uint64_t num, char* buffer, int base) {
    int index = 0;
    int digits = 0;
    uint64_t tempNum = num;
    if (num == 0) {
        buffer[index] = 48;
        buffer[index  + 1] = '\0';
        return buffer;
    }

    switch (base) {
        case 10:
            // find number of digits
            while (1) {
                if (tempNum / 10 >= 1) {
                    digits++;
                    tempNum = tempNum / 10;
                }
                else {
                    break;
                }
            }
            index = digits + 1;

            buffer[index] = '\0';

            while (1) {
                index--;
                if (num / 10 >= 1) {
                    buffer[index] = 48 + num % 10;
                    num = num / 10;   
                }
                else {
                    buffer[index] = 48 + num;
                    break;
                }
            }

        case 16:
            // find number of digits
            while (1) {
                if (tempNum / 16 >= 1) {
                    digits++;
                    tempNum = tempNum / 16;
                }
                else {
                    break;
                }
            }
            index = digits + 1;

            buffer[index] = '\0';

            while (1) {
                index--;
                if (num / 16 >= 1) {
                    if (num % 16 >= 10) {
                        buffer[index] = 65 - 10 + num % 16;
                    }
                    else {
                        buffer[index] = 48 + num % 16;
                    }
                    num = num / 16;   
                }
                else {
                    if (num >= 10) {
                        buffer[index] = 65 - 10 + num;
                    }
                    else {
                        buffer[index] = 48 + num;
                    }
                    break;
                }
            }
    }
    
    return buffer;
}

uintptr_t strlen(char* string) {
    uintptr_t length = 1;
    for (uintptr_t index = 0; string[index] != '\0'; index++) {
        length++;
    }
    return length;
}

bool strcmp(char* string1, char* string2) {
    uintptr_t length = 0;
    while (string1[length] != '\0') {
        length++;
    }
    
    for (uintptr_t i = 0; i < length; i++) {
        if (string1[i] != string2[i]) {
            return true;
        }
    }
    return false;
}

bool strcmp_notnull(char* string1, char* string2, uintptr_t length) {
    for (uintptr_t i = 0; i < length; i++) {
        if (string1[i] != string2[i]) {
            return false;
        }
    }
    return true;
}