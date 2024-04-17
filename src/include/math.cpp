#include "include/math.hpp"

uint64_t math::pow(uint64_t base, uint64_t power) {
    uint64_t number = 1;
    for (uint64_t x = 0; x < power; x++) {
        number *= base;
    }
    return number;
}