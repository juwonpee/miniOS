#pragma once

#include "include/stdint-gcc.hpp"


class string {
    public:
    string();
};

class stringUtils {
    public:
    // returns the length of the number string
    // Supports any arbitrary bases till hexadecimal
    static char* itoa(int64_t num, char* buffer, uint8_t base);
};