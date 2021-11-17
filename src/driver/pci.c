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

void enumerate_function(uint64_t deviceAddress, uint64_t function) {
	uintptr_t functionAddress = deviceAddress + (function << 12);

	// Map memory to pci devices
	malloc_direct_map((void*)functionAddress, (void*)functionAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)functionAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;

	printf("Vendor ID: %x, Device ID: %x\n", device_header->vendor_id, device_header->device_id);

	return;

}

void enumerate_device(uint64_t busAddress, uint64_t device) {
	uintptr_t deviceAddress = busAddress + (device << 15);

	// Map memory to pci devices
	malloc_direct_map((void*)deviceAddress, (void*)deviceAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)deviceAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
	else {
		for (uintptr_t function = 0; device < 8; device++) {
			enumerate_function(deviceAddress, function);
		}
	}
}

void enumerate_bus(uint64_t baseAddress, uintptr_t bus) {
	uintptr_t busAddress = baseAddress + (bus << 20);

	// Map memory to pci devices
	malloc_direct_map((void*)busAddress, (void*)busAddress);
	pci_device_header_t* device_header = (pci_device_header_t*)busAddress;

	// Check if device is null
	if (device_header->device_id == 0 || device_header->device_id == 0xFFFF) return;
	else {
		for (uintptr_t device = 0; device < 32; device++) {
			enumerate_device(busAddress, device);
		}
	}
}

bool enumerate_pci(acpi_MCFG_t mcfg) {
	uintptr_t entries = (mcfg.header.length - sizeof(acpi_sdt_header_t)) / sizeof(pci_configuration_space_t);

	// Discover PCI devices
	printf("PCI devices:\n");
	for (uintptr_t i = 0; i < entries; i++) {
		pci_configuration_space_t device = mcfg.configurationSpace[i];
		for (uintptr_t bus = (uintptr_t)device.baseAddress; bus < device.endBus; bus++) {
			enumerate_bus(device.baseAddress, bus);
		}
	}
	return false;
}

bool pci_init(acpi_MCFG_t mcfg) {
	if (enumerate_pci(mcfg)) {
		return true;
	}
	return false;
}