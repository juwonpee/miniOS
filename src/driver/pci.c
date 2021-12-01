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

#include "pci.h"

char tempString[64];


void enumerate_function(uint64_t deviceAddress, uint64_t function, pci_device_header_t* ahci_device_address) {
	uintptr_t offset = function << 12;
	uintptr_t functionAddress = deviceAddress + offset;

	// Map memory to pci devices
	if (!memory_kernel_check_exists((void*)functionAddress)) memory_direct_map((void*)functionAddress, (void*)functionAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)functionAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;

	printf("%s %s %s\n", pci_get_device_vendor_name(device_header->vendor_id), pci_get_device_device_name(device_header->vendor_id, device_header->device_id), pci_get_device_class(device_header->class));
	if (device_header->class == 0x1 && device_header->subClass == 0x6 && device_header->program_interface == 0x1) {
		//ahci_init(device_header);
		*ahci_device_address = *device_header;
	}

	return;

}

void enumerate_device(uint64_t busAddress, uint64_t device, pci_device_header_t* ahci_device_address) {
	uintptr_t offset = device << 15;
	uintptr_t deviceAddress = busAddress + offset;

	// Map memory to pci devices
	if (!memory_kernel_check_exists((void*)deviceAddress)) memory_direct_map((void*)deviceAddress, (void*)deviceAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)deviceAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
	else {
		for (uintptr_t function = 0; function < 8; function++) {
			enumerate_function(deviceAddress, function, ahci_device_address);
		}
	}
}

void enumerate_bus(uint64_t baseAddress, uintptr_t bus, pci_device_header_t* ahci_device_address) {
	uintptr_t offset = bus << 20;
	uintptr_t busAddress = baseAddress + offset;

	// Map memory to pci devices
	if (!memory_kernel_check_exists((void*)busAddress)) memory_direct_map((void*)busAddress, (void*)busAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)busAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
	else {
		for (uintptr_t device = 0; device < 32; device++) {
			enumerate_device(busAddress, device, ahci_device_address);
		}
	}
}

bool enumerate_pci(acpi_MCFG_t mcfg, pci_device_header_t* ahci_device_address) {
	uintptr_t entries = (mcfg.header.length - sizeof(acpi_sdt_header_t)) / sizeof(pci_configuration_space_t);

	// Discover PCI devices
	printf("PCI devices:\n");
	for (uintptr_t i = 0; i < entries; i++) {
		pci_configuration_space_t device = mcfg.configurationSpace[i];
		for (uintptr_t bus = (uintptr_t)device.startBus; bus < device.endBus; bus++) {
			enumerate_bus(device.baseAddress, bus, ahci_device_address);
		}
	}
	return false;
}

bool pci_init(acpi_MCFG_t mcfg, pci_device_header_t* ahci_device_address) {
	if (enumerate_pci(mcfg, ahci_device_address)) {
		return true;
	}
	return false;
}

char* pci_get_device_class(uint8_t class) {
	char* class_codes[] = {
		"Unclassified",
		"Mass Storage Controller",
		"Network Controller",
		"Display Controller",
		"Multimedia Controller",
		"Memory Controller",
		"Bridge",
		"Simple Communication Controller",
		"Base System Peripheral",
		"Input Device Controller",
		"Docking Station",
		"Processor",
		"Serial Bus Controller",
		"Wireless Controller",
		"Intelligent Controller",
		"Satellite Communication Controller",
		"Encryption Controller",
		"Signal Processing Controller",
		"Processing Accelerator",
		"Non-Essential Instrumentation",
		"0x3F (Reserved)"
	};

	return class_codes[class];
}

char* pci_get_device_vendor_name(uint16_t vendorID) {
	switch (vendorID) {
		case 0x8086:
			return "Intel Corp.";
		case 0x1234:
			return "QEMU";
		default:
			return itoa(vendorID, tempString, 16);
	}
}

char* pci_get_device_device_name(uint16_t vendorID, uint16_t deviceID) {
	switch (vendorID) {
		case 0x8086:
			switch (deviceID) {
				case 0x29C0:
					return "82G33/G31/P35/P31 Express DRAM Controller";
				case 0x10D3:
					return "82574L Gigabit Network Connection";
				case 0x2918:
					return "82801IB (ICH9) LPC Interface Controller";
				case 0x2922:
					return "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI mode]";
				case 0x2930:
					return "82801I (ICH9 Family) SMBus Controller";
				default:
					return itoa(deviceID, tempString, 16);
			}
		case 0x1234:
			switch (deviceID) {
				case 0x1111:
					return "Virtual Video Controller";
				default:
					return itoa(deviceID, tempString, 16);
			}
		default:
			return itoa(deviceID, tempString, 16);
	}
}
