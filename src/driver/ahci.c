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

#include "ahci.h"


int check_type(ahci_hba_port* port) {
	uint32_t ssts = port->ssts;
 
	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

void probe_ports(ahci_hba_memory* abar) {
	uint32_t pi = abar->pi;
	for (uintptr_t i = 0; i < 32; i++) {
		if (pi & 1) {
			int dt = check_type((ahci_hba_port*)&abar->ports);
			if (dt == AHCI_DEV_SATA) {
				printf("SATA drive found at port %x\n", i);
			}
			else if (dt == AHCI_DEV_SATAPI) {
				printf("SATAPI drive found at port %x\n", i);
			}
			else if (dt == AHCI_DEV_SEMB) {
				printf("SEMB drive found at port %x\n", i);
			}
			else if (dt == AHCI_DEV_PM) {
				printf("PM drive found at port %x\n", i);
			}
			else {
				printf("No drive found at port %x\n", i);
			}
		}
	}
}

bool ahci_init(pci_device_header_t* baseAddress) {
    printf("Initializing AHCI Controller... \n");
	pci_bar_t* temp1 = (pci_bar_t*)baseAddress;
	ahci_hba_memory* abar = (ahci_hba_memory*)temp1->bar[5];
	if (!memory_kernel_check_exists(abar)) memory_direct_map(abar, abar);
	probe_ports(abar);


	return false;
}

