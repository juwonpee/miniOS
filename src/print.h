#pragma once

#include "types.h"
#include "io.h"

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

int serialInit(uint16_t _COMport);

char serialInByte();

void printByte(char character);

void print(char* string);

void println(char* string);

