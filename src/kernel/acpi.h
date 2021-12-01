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

#include "types.h"
#include "multiboot2.h"
#include "print.h"
#include "memory.h"
#include "apic.h"
#include "pci.h"


typedef struct acpi_rsdp_descriptor_t {
	uint8_t signature[8];
	uint8_t checksum;
	uint8_t OEMID[6];
	uint8_t revision;
	uint32_t rsdtAddress;
} __attribute__ ((packed)) acpi_rsdp_descriptor_t;

typedef struct acpi_rsdp2_descriptor_t {
	acpi_rsdp_descriptor_t base;

	uint32_t length;
	uint64_t xsdpAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} __attribute ((packed)) acpi_rsdp2_descriptor_t;



typedef struct acpi_rsdt_t {
	acpi_sdt_header_t acpi_sdt_header;
	acpi_sdt_header_t* tablePointer[];
} __attribute__ ((packed)) acpi_rsdt_t;

typedef struct acpi_xsdt_t {
	acpi_sdt_header_t acpi_sdt_header;
	acpi_sdt_header_t* tablePointer[];
} __attribute__ ((packed)) acpi_xsdt_t;

// Supports up to 64 MCFG entries for now due to how the memory manager currently works (should be plenty enough)
// TODO (Low prio): Fix this workaround


typedef struct acpi_MADT_t {
	acpi_sdt_header_t header;
	apic_descriptor_t descriptor;
} __attribute ((packed)) acpi_MADT_t;

typedef struct acpi_master_table_t {
	bool OK;
	acpi_MCFG_t MCFG;
	acpi_MADT_t MADT;
} acpi_master_table_t;

acpi_master_table_t acpi_init(struct multiboot_tag_old_acpi* multiboot_acpi);