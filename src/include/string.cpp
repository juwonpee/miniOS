#include "include/string.hpp"

char* stringUtils::itoa(int64_t num, char* buffer, uint8_t base) {
    char* returnBuffer = buffer;
    int length = 0;

    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num *= -1;
    }

    while (true) {
        const int asciiNumber = 48;
        const int asciiCharacter = 65;
        
        // modulo with base number to get that digit
        uint64_t digit = num % base;
        num /= base;

        // convert digit to character
        if (digit < 10) {
            *buffer = (char)(asciiNumber + digit);
        }
        else if (digit >= 10) {
            *buffer = (char)(asciiCharacter + digit - 10);
        }
        
        // increment to next character
        buffer++;
        length++;

        if (num == 0) break;
    }

    // add negative sign
    if (isNegative) {
        *buffer = '-';
        buffer++;
        length++;
    }

    // null terminate buffer
    *buffer = '\0';

    // reverse buffer
    for (int x = 0; x < length/2; x++) {
        char A = returnBuffer[x];
        char B = returnBuffer[length-1-x];

        returnBuffer[x] = B;
        returnBuffer[length-1-x] = A;
    }


    return returnBuffer;
}