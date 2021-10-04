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

typedef struct acpi_sdt_header_t {
	uint8_t signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t OEMID[6];
	uint8_t OEMTableID[8];
	uint32_t OEMRevision;
	uint32_t creatorID;
	uint32_t creatorRevision;
} __attribute__ ((packed)) acpi_sdt_header_t;

typedef struct acpi_rsdt_t {
	acpi_sdt_header_t acpi_sdt_header;
	uint32_t tablePointer[];
} __attribute__ ((packed)) acpi_rsdt_t;

typedef struct acpi_xsdt_t {
	acpi_sdt_header_t acpi_sdt_header;
	uint64_t tablePointer[];
} __attribute__ ((packed)) acpi_xsdt_t;


bool acpi_init(struct multiboot_tag_old_acpi multiboot_acpi);