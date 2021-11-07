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


// ATA TYPES
typedef enum ata_error_t {
	normal, error, AMNF, TKZNF, ABRT, MCR, IDNF, MC, UNC, BBK
} ata_error_t ;

typedef struct ata_sector_data_t {
	uint8_t data[512];
} ata_sector_data_t;

// IO TYPES

// PCI TYPES
typedef struct pci_device_config {
	uint64_t baseAddress;
	uint16_t PCISegGroup;
	uint8_t startBus;
	uint8_t endBus;
	uint32_t reserved;
} __attribute__ ((packed)) pci_device_config;

// PIT TYPES

// PRINT TYPES

// VFS TYPES

// STRING TYPES


// ACPI TYPES
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
	acpi_sdt_header_t* tablePointer[];
} __attribute__ ((packed)) acpi_rsdt_t;

typedef struct acpi_xsdt_t {
	acpi_sdt_header_t acpi_sdt_header;
	acpi_sdt_header_t* tablePointer[];
} __attribute__ ((packed)) acpi_xsdt_t;

typedef struct acpi_MCFG_t {
	acpi_sdt_header_t header;
	uint64_t reserved;
	struct acpi_MCFG_configuration_space {
		uint64_t* base_address;
		uint16_t group_segment_number;
		uint8_t start_pci_number;
		uint8_t end_pci_number;
	} configuration_space[];
} __attribute ((packed)) acpi_MCFG_t;

typedef struct acpi_master_table_t {
	bool OK;
	acpi_MCFG_t MCFG;
} acpi_master_table_t;


// INTERRUPT TYPES
typedef struct IDT_t {
	union {
		uint64_t data;
		struct {
			uint16_t offset1;
			uint16_t selector;
			uint8_t ignore;
			uint8_t flags; 
			uint16_t offset2;
		} __attribute__ ((packed));
	};
} __attribute__ ((packed)) IDT_t;

typedef struct IDTR_t {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed)) IDTR_t;

typedef struct IDT_interruptFrame_t IDT_interruptFrame_t;

typedef struct IDT_pageFault_error_t {
	union {
		uint32_t data;
		struct {
			uint32_t P:1;																		// Present				When set, the page fault was caused by a page-protection violation. When not set, it was caused by a non-present page.
			uint32_t W:1;																		// Write				When set, the page fault was caused by a write access. When not set, it was caused by a read access.
			uint32_t U:1;																		// User					When set, the page fault was caused while CPL = 3. This does not necessarily mean that the page fault was a privilege violation.
			uint32_t R:1;																		// Reserved write		When set, one or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1.
			uint32_t reserved:28;
		} __attribute__ ((packed));
	};
} __attribute__ ((packed)) IDT_pageFault_error_t;

typedef struct IDT_selector_error_t {
	union {
		uint32_t data;
		struct {
			uint32_t E:1;																		// External				When set, the exception originated externally to the processor.
			uint32_t TBL:2;																		/* Table selector		0b00	The Selector Index references a descriptor in the GDT.
																														0b01	The Selector Index references a descriptor in the IDT.
																														0b10	The Selector Index references a descriptor in the LDT.
																														0b11	The Selector Index references a descriptor in the IDT. */
			uint32_t index:13;																	// Selector index		The index in the GDT, IDT or LDT. 
			uint32_t reserved:16;
		} __attribute__ ((packed));
	};
} __attribute__ ((packed)) IDT_selector_error_t;



// KERNEL_INIT TYPES
struct multiboot_tag_header {
	multiboot_uint32_t total_size;
	multiboot_uint32_t reserved;
};



// SCHEDULER TYPES
typedef uintptr_t scheduler_context_t;



// MEMORY TYPES
typedef struct memory_pageDirectoryCR3_t {
	union {
		uint32_t data;
		struct {
			uint32_t ignore1:3;
			uint32_t pageWriteThrough:1;
			uint32_t pageCacheDisable:1;
			uint32_t ignore2:7;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageDirectoryCR3_t;

typedef struct memory_pageDirectory_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t RW:1;                                          // Read/Write
			uint32_t US:1;                                          // 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;                            // 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;							// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t ignore1:1;
			uint32_t size:1;                                        // 0: 4MB pages, 1: 4KiB pages
			uint32_t ignore2:4;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageDirectory_t;

typedef struct memory_pageTable_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t RW:1;                                         // Read/Write
			uint32_t US:1;                                          // 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;                            // 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;							// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t dirty:1;
			uint32_t PAT:1;                                         // PAT, not enabled so must stay zero
			uint32_t global:1;                                      // 0: Invalidate TLB on CR3 flush		1: Do not invalidate
			uint32_t ignore:3;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageTable_t;

typedef struct memory_pageDirectory_owner_t {
	memory_pageDirectory_t* page;
	bool free;
	uintptr_t pid;
} memory_pageDirectory_owner_t;

typedef struct memory_malloc_node_t {
	uintptr_t size;																				// Size for the total structure
	struct memory_malloc_node_t* prevNode;												// Included prev node pointer to effeciently finding the previous node in free()
	struct memory_malloc_node_t* nextNode;												// Included next node pointer to effeciently find the next node or free space
	uintptr_t data[];
} memory_malloc_node_t;

typedef struct memory_page_descriptor_t {
	uintptr_t references;
	scheduler_context_t pid;
	memory_pageTable_t* pageTable;
} memory_page_descriptor_t;



// PANIC TYPES

