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



#pragma once

#include <stdarg.h>

#include "types.h"
#include "driver/io.h"
#include "kernel/panic.h"





char readChar();

void printChar(char character);

__attribute__ ((fastcall)) void printf(char* format, ...);

__attribute__ ((fastcall)) void println(char* string);

__attribute__ ((fastcall)) void printsn(char* string);