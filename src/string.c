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