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

#include "ata.h"

uint8_t primarySectorData[512];
uint8_t secondarySectorData[512];

typedef enum primaryStatus {
	identify, read, write
} primartStatus;

bool ata_init() {
	// Identify primary drive
	outb(ATA_PRIMARY_DRIVE_HEAD_RW, 0xA0);
	outb(ATA_PRIMARY_SECTOR_COUNT_RW, 0);
	outb(ATA_PRIMARY_SECTOR_NUMBER_RW, 0);
	outb(ATA_PRIMARY_CYLINDER_LOW_RW, 0);
	outb(ATA_PRIMARY_CYLINDER_HIGH_RW, 0);
	outb(ATA_PRIMARY_COMMAND_W, 0xEC);
	if (!inb(ATA_PRIMARY_STATUS_R)) {
		println("Primary drive does not exist");
		return true;
	}

	// Check drive activity for 4~5 seconds
	uint64_t driveStartTime = pit_get_time_since_boot();
	while (1) {
		if (driveStartTime + 5 > pit_get_time_since_boot()) {
			if ((inb(ATA_PRIMARY_STATUS_R) & 0x80) == 0) {
				break;
			}
			else if ((inb(ATA_PRIMARY_CYLINDER_LOW_RW) != 0) & (inb(ATA_PRIMARY_CYLINDER_HIGH_RW) != 0)) {
				println("Primary drive not ATA drive, reccomend changing drive configuration");
				return true;
			}
		}
		else {
			println("Primary drive not functioning, Reccomend soft reboot");
			return true;
		}
	}


	// Identify secondary drive
	outb(ATA_SECONDARY_DRIVE_HEAD_RW, 0xA0);
	outb(ATA_SECONDARY_SECTOR_COUNT_RW, 0);
	outb(ATA_SECONDARY_SECTOR_NUMBER_RW, 0);
	outb(ATA_SECONDARY_CYLINDER_LOW_RW, 0);
	outb(ATA_SECONDARY_CYLINDER_HIGH_RW, 0);
	outb(ATA_SECONDARY_COMMAND_W, 0xEC);
	if (!inb(ATA_SECONDARY_STATUS_R)) {
		println("Seondary drive does not exist");
		return true;
	}

	// Check drive activity for 4~5 seconds
	driveStartTime = pit_get_time_since_boot();
	while (1) {
		if (driveStartTime + 5 > pit_get_time_since_boot()) {
			if (inb(ATA_SECONDARY_STATUS_R) & 0x08) {
				break;
			}
			else if ((inb(ATA_SECONDARY_CYLINDER_LOW_RW) != 0) & (inb(ATA_SECONDARY_CYLINDER_HIGH_RW) != 0)) {
				println("Secondary drive not ATA drive, reccomend changing drive configuration");
				break;
			}
		}
		else {
			println("Secondary drive not functioning, Reccomend soft reboot");
		}
	}
	return false;
}

ata_status_t ata_primary_read_sector() {
	if (inb(ATA_PRIMARY_STATUS_R) & 0x08) {
		for (int x = 0; x < 256; x++) {
			primarySectorData[x*2] = inw(ATA_PRIMARY_DATA_RW);
		}
		return normal;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x01) {
		return AMNF;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x02) {
		return TKZNF;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x04) {
		return ABRT;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x08) {
		return MCR;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x10) {
		return IDNF;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x20) {
		return MC;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x40) {
		return UNC;
	}
	else if (inb(ATA_PRIMARY_STATUS_R) & 0x80) {
		return BBK;
	}
	return error;
}

ata_status_t ata_secondary_read_sector() {
	if (inb(ATA_SECONDARY_STATUS_R) & 0x08) {
		for (int x = 0; x < 256; x++) {
			secondarySectorData[x*2] = inw(ATA_SECONDARY_DATA_RW);
		}
		return normal;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x01) {
		return AMNF;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x02) {
		return TKZNF;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x04) {
		return ABRT;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x08) {
		return MCR;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x10) {
		return IDNF;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x20) {
		return MC;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x40) {
		return UNC;
	}
	else if (inb(ATA_SECONDARY_STATUS_R) & 0x80) {
		return BBK;
	}
	return error;
}