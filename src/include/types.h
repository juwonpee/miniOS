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

#include <stdint-gcc.h>
#include <stdbool.h>
#include "multiboot2.h"


// Forward declarations
typedef struct acpi_sdt_header acpi_sdt_header;

// ATA TYPES
typedef enum ata_error_t {
	normal, error, AMNF, TKZNF, ABRT, MCR, IDNF, MC, UNC, BBK
} ata_error_t ;

typedef struct ata_sector_data_t {
	uint8_t data[512];
} ata_sector_data_t;


// PCI TYPES
typedef struct pci_configuration_space_t {
	uint64_t baseAddress;
	uint16_t group_segment_number;
	uint8_t startBus;
	uint8_t endBus;
	uint32_t reserved;
} __attribute__ ((packed)) pci_configuration_space_t;

typedef struct pci_device_header_t {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t program_interface;
	uint8_t subClass;
	uint8_t class;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t BIST;
} __attribute__ ((packed)) pci_device_header_t;


// ACPI TYPES
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


// STRING TYPES


// ACPI TYPES
typedef struct acpi_MCFG_t {
	acpi_sdt_header_t header;
	uint64_t reserved;
	pci_configuration_space_t configurationSpace[64];
} __attribute__ ((packed)) acpi_MCFG_t;


// INTERRUPT TYPES
typedef struct IDT_pageFault_error_t {
	union {
		uint32_t data;
		struct {
			uint32_t P:1;																		// Present				When set, the page fault was caused by a page-protection violation. When not set, it was caused by a non-present page.
			uint32_t W:1;																		// Write				When set, the page fault was caused by a write access. When not set, it was caused by a read access.
			uint32_t U:1;																		// User					When set, the page fault was caused while CPL = 3. This does not necessarily mean that the page fault was a privilege violation.
			uint32_t R:1;																		// Reserved write		When set, one or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1.
			uint32_t reserved:28;
		};
	};
} __attribute__ ((packed)) IDT_pageFault_error_t;



// SCHED TYPES
typedef uint16_t scheduler_pid_t;

