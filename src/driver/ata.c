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

ata_sector_data_t primarySectorData;
ata_sector_data_t secondarySectorData;

bool primary_lock = false;
bool secondary_lock = false;

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
			return false;
		}
	}
	return false;
}

// PRIMARY ATA DRIVE BUS
ata_sector_data_t ata_primary_read(uint64_t lba) {
	primary_lock = true;
	ata_primary_read_sector_request(lba);
	while (1) {
		if (!primary_lock) {
			return primarySectorData;
		}
	}
}
bool ata_primary_read_sector_request(uint64_t lba) {
	outb(ATA_PRIMARY_DRIVE_HEAD_RW, 0x40);														// Select master drive
	outb(ATA_PRIMARY_SECTOR_COUNT_RW, 0);														// Sector count high byte
	outb(ATA_PRIMARY_SECTOR_NUMBER_RW, (uint8_t)(lba >> 24));									// lba byte 4
	outb(ATA_PRIMARY_CYLINDER_LOW_RW, (uint8_t)(lba >> 32));									// lba byte 5
	outb(ATA_PRIMARY_CYLINDER_HIGH_RW, (uint8_t)(lba >> 40));									// lba byte 6
	outb(ATA_PRIMARY_SECTOR_COUNT_RW, 1);														// Sector count low byte
	outb(ATA_PRIMARY_SECTOR_NUMBER_RW, (uint8_t)(lba));											// lba byte 1
	outb(ATA_PRIMARY_CYLINDER_LOW_RW, (uint8_t)(lba >> 8));										// lba byte 2
	outb(ATA_PRIMARY_CYLINDER_HIGH_RW, (uint8_t)lba >> 16);										// lba byte 3
	outb(ATA_PRIMARY_COMMAND_W, ATA_READ_SECTOR);
	return false;
}
ata_error_t ata_primary_read_sector() {
	if (inb(ATA_PRIMARY_STATUS_R) & 0x08) {
		for (int x = 0; x < 256; x++) {
			primarySectorData.data[x*2] = inw(ATA_PRIMARY_DATA_RW);
		}
		primary_lock = false;
		return normal;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x01) {
		primary_lock = false;
		return AMNF;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x02) {
		primary_lock = false;
		return TKZNF;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x04) {
		primary_lock = false;
		return ABRT;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x08) {
		primary_lock = false;
		return MCR;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x10) {
		primary_lock = false;
		return IDNF;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x20) {
		primary_lock = false;
		return MC;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x40) {
		primary_lock = false;
		return UNC;
	}
	else if (inb(ATA_PRIMARY_ERROR_R) & 0x80) {
		primary_lock = false;
		return BBK;
	}
	primary_lock = false;
	return error;
}


bool ata_primary_write_sector(uint64_t lba, ata_sector_data_t data) {
	return false;
}

// SECONDARY ATA DRIVE BUS
ata_sector_data_t ata_secondary_read(uint64_t lba) {
	secondary_lock = true;
	ata_secondary_read_sector_request(lba);
	while (1) {
		if (!secondary_lock) {
			return secondarySectorData;
		}
	}
}
bool ata_secondary_read_sector_request(uint64_t lba) {
	outb(ATA_SECONDARY_DRIVE_HEAD_RW, 0x40);													// Select master drive
	outb(ATA_SECONDARY_SECTOR_COUNT_RW, 0);														// Sector count high byte
	outb(ATA_SECONDARY_SECTOR_NUMBER_RW, (uint8_t)(lba >> 24));									// lba byte 4
	outb(ATA_SECONDARY_CYLINDER_LOW_RW, (uint8_t)(lba >> 32));									// lba byte 5
	outb(ATA_SECONDARY_CYLINDER_HIGH_RW, (uint8_t)(lba >> 40));									// lba byte 6
	outb(ATA_SECONDARY_SECTOR_COUNT_RW, 1);														// Sector count low byte
	outb(ATA_SECONDARY_SECTOR_NUMBER_RW, (uint8_t)(lba));										// lba byte 1
	outb(ATA_SECONDARY_CYLINDER_LOW_RW, (uint8_t)(lba >> 8));									// lba byte 2
	outb(ATA_SECONDARY_CYLINDER_HIGH_RW, (uint8_t)lba >> 16);									// lba byte 3
	outb(ATA_SECONDARY_COMMAND_W, ATA_READ_SECTOR);
	return false;
}
ata_error_t ata_secondary_read_sector() {
	if (inb(ATA_SECONDARY_STATUS_R) & 0x08) {
		for (int x = 0; x < 256; x++) {
			secondarySectorData.data[x*2] = inw(ATA_SECONDARY_DATA_RW);
		}
		secondary_lock = false;
		return normal;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x01) {
		secondary_lock = false;
		return AMNF;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x02) {
		secondary_lock = false;
		return TKZNF;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x04) {
		secondary_lock = false;
		return ABRT;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x08) {
		secondary_lock = false;
		return MCR;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x10) {
		secondary_lock = false;
		return IDNF;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x20) {
		secondary_lock = false;
		return MC;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x40) {
		secondary_lock = false;
		return UNC;
	}
	else if (inb(ATA_SECONDARY_ERROR_R) & 0x80) {
		secondary_lock = false;
		return BBK;
	}
	secondary_lock = false;
	return error;
}

bool ata_secondary_write_sector(uint64_t lba, ata_sector_data_t data) {
	return false;
}