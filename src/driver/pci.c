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


void enumerate_bus(uint64_t address, uint64_t bus) {

}

void enumerate_device(uint64_t address, uint64_t device) {

}

void enumerate_function(uint64_t address, uint64_t function) {
	
}

bool pci_init(acpi_master_table_t acpi_master_table) {
	acpi_MCFG_t MCFG = acpi_master_table.MCFG;
	//int entries = (MCFG.header.length - sizeof(acpi_MCFG_t)) / sizeof(acpi_device_config)
}