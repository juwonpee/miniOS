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

#define PCI_CONFIG_ADDRESS 			0xCF8
#define PCI_CONFIG_DATA				0xCFC
#define PCI_FORWARDING_REGISTER		0xCFA

extern char* class_codes[];

bool pci_init(acpi_MCFG_t mcfg);

char* pci_get_device_class(uint8_t class);

char* pci_get_device_vendor_name(uint16_t vendorID);

char* pci_get_device_device_name(uint16_t vendorID, uint16_t deviceID);
