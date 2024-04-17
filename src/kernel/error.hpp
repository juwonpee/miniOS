#include "include/stdint-gcc.hpp"

class error {
    public:
    static void panic(const char* reason);
    static void warning(const char* reason);
};