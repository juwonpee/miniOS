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
#include "print.h"

typedef struct apic_entry_0_t {
	uint8_t processor_id;
	uint8_t apic_id;
	uint64_t flags;
} __attribute__ ((packed)) apic_entry_0_t;

typedef struct apic_entry_1_t {
	uint8_t io_apic_id;
	uint8_t reserved;
	uint32_t io_apic_address;
	uint32_t global_system_interrupt_base;
} __attribute__ ((packed)) apic_entry_1_t;

typedef struct apic_entry_2_t {
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t global_system_interrupt;
	uint16_t flags;
} __attribute__ ((packed)) apic_entry_2_t;

typedef struct apic_entry_3_t {
	uint8_t nmi_source;
	uint8_t reserved;
	uint8_t flags;
	uint8_t global_system_interrupt;
} __attribute__ ((packed)) apic_entry_3_t;

typedef struct apic_entry_4_t {
	uint8_t acpi_processor_id;
	uint16_t flags;
	uint8_t lint;
} __attribute__ ((packed)) apic_entry_4_t;

typedef struct apic_entry_5_t {
	uint16_t reserved;
	uint64_t local_apic_address;
} __attribute__ ((packed)) apic_entry_5_t;

typedef struct apic_entry_9_t {
	uint16_t reserved;
	uint32_t processor_local_x2_apic_id;
	uint32_t flags;
	uint32_t acpi_id;
} __attribute__ ((packed)) apic_entry_9_t;

typedef struct apic_entry_t {
	uint8_t entryType;
	uint8_t length;
	uint8_t entry[];
} __attribute__ ((packed)) apic_entry_t;

// Supports up to 256 bytes of APIC entries for now due to how the memory manager currently works (should be plenty enough)
// TODO (Low prio): Fix this workaround
typedef struct apic_descriptor_t {
	uint32_t apic_address;
	uint32_t flags;
	uint8_t entry[256];
} __attribute__ ((packed)) apic_descriptor_t;