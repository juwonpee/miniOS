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

#include "acpi.h"

void* MADT;

bool acpi_init(struct multiboot_tag_old_acpi multiboot_acpi) {
    // Check acpi version
	acpi_rsdp_descriptor_t* rsdp = (acpi_rsdp_descriptor_t*)(&multiboot_acpi.rsdp);
	if (rsdp -> revision == 0) {
		println("ACPI Version 1");
	}
	else {
		println("ACPI Version 2");
	}

	// Get RSDT
	acpi_rsdt_t* rsdt = (acpi_xsdt_t*)((uintptr_t)(rsdp -> rsdtAddress));

	// Get ACPI tables
	int entries = (rsdt -> acpi_sdt_header.length - sizeof(rsdt -> acpi_sdt_header)) / 4;
	// for (int i = 0; i < entries; i++) {
	// 	acpi_sdt_header_t* h = (acpi_sdt_header_t*) xsdt -> tablePointer;
	// 	println((char*)&h -> signature);
	// }
	return false;
}