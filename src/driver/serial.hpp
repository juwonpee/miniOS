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

#include "include/stdint-gcc.hpp"

#define COM1                0x3F8
#define COM2                0x2F8
#define COM3                0x3E8
#define COM4                0x2E8
#define COMport             COM1

class serial {
    public:
    static bool serialInit();
	static void printf(const char* format, ...);

	private:
	static void printChar(char);

};