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
#include "acpi.h"
#include "memory.h"
#include "ahci.h"

#define PCI_CONFIG_ADDRESS 			0xCF8
#define PCI_CONFIG_DATA				0xCFC
#define PCI_FORWARDING_REGISTER		0xCFA





typedef struct pci_bar_t {
	pci_device_header_t header;
	uint32_t bar[6];
	uint32_t cardbus_cis_ptr;
	uint32_t subsystem_vendor_id;
	uint16_t subsystem_id;
	uint32_t expansion_rom_base_address;
	uint8_t capabilities_pointer;
	uint8_t reserved1;
	uint16_t reserved2;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint8_t min_grant;
	uint8_t max_latency;
} __attribute__ ((packed)) pci_bar_t;

extern char* class_codes[];

bool pci_init(acpi_MCFG_t mcfg, pci_device_header_t* ahci_device_address);

char* pci_get_device_class(uint8_t class);

char* pci_get_device_vendor_name(uint16_t vendorID);

char* pci_get_device_device_name(uint16_t vendorID, uint16_t deviceID);
