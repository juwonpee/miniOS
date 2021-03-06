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

acpi_master_table_t acpi_init(struct multiboot_tag_old_acpi* multiboot_acpi) {
	acpi_master_table_t master_table;

    // Get RSDP
	acpi_rsdp_descriptor_t* rsdp = (acpi_rsdp_descriptor_t*)&(multiboot_acpi->rsdp);


	// Check & verify rsdp
	if (!strcmp_notnull((char*)rsdp->signature, "RSD PTR ", 8)) {
		println("Error: Unable to find RSDP table, Requires ACPI compliant bootloader/bios");
		master_table.OK = false;
		return master_table;
	}

	// RSDP version
	if (rsdp -> revision == 0) {
		println("ACPI Version 1");
	}
	else {
		println("ACPI Version 2");
	}

	// Get RSDT
	acpi_rsdt_t* rsdt = (acpi_rsdt_t*)((uintptr_t)(rsdp -> rsdtAddress));

	// Check & verify rsdt
	if (!strcmp_notnull((char*)rsdt->acpi_sdt_header.signature, "RSDT", 4)) {
		println("Error: Unable to find RSDT table, Requires ACPI compliant bootloader/bios");
		master_table.OK = false;
		return master_table;
	}

	// Get ACPI tables
	int entries = (rsdt -> acpi_sdt_header.length - sizeof(rsdt->acpi_sdt_header)) / 4;
	for (int i = 0; i < entries; i++) {
		acpi_sdt_header_t* header = rsdt->tablePointer[i];

		// Print all ACPI tables
		char tempString[64];
		printf(itoa((uintptr_t)header->length, tempString, 16));
		printf(" ");
		for (uintptr_t j = 0; j < 4; j++) {
			printChar(header->signature[j]);
		}
		printf("\n");

		if (strcmp_notnull((char*)header->signature, "MCFG", 4)) {
			// Get header and MCFG data
			memcpy(&master_table.MCFG, header, header->length);
		}
		else if (strcmp_notnull((char*)header->signature, "APIC" ,4)) {
			// Get header and MADT entry data
			memcpy(&master_table.MADT, header, header->length);
		}
	}
	// for (int i = 0; i < entries; i++) {
	// 	acpi_sdt_header_t* h = (acpi_sdt_header_t*) xsdt -> tablePointer;
	// 	println((char*)&h -> signature);
	// }

	master_table.OK = true;
	return master_table;
}