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
#include "io.h"
#include "print.h"
#include "pit.h"
#include "interrupt.h"

#define ATA_PRIMARY_IRQ						15
#define ATA_SECONDARY_IRQ					14

// ATA Registers
#define ATA_PRIMARY_DATA_RW					0x1F0
#define ATA_PRIMARY_ERROR_R					0x1F1
#define ATA_PRIMARY_FEATURES_W				0x1F1
#define ATA_PRIMARY_SECTOR_COUNT_RW			0x1F2
#define ATA_PRIMARY_SECTOR_NUMBER_RW		0x1F3
#define ATA_PRIMARY_CYLINDER_LOW_RW			0x1F4
#define ATA_PRIMARY_CYLINDER_HIGH_RW		0x1F5
#define ATA_PRIMARY_DRIVE_HEAD_RW			0x1F6
#define ATA_PRIMARY_STATUS_R				0x1F7
#define ATA_PRIMARY_COMMAND_W				0x1F7
#define ATA_PRIMARY_ALTERNATE_STATUS_R		0x3F6
#define ATA_PRIMARY_DEVICE_CONTROL_W		0x3F6
#define ATA_PRIMARY_DRIVE_ADDRESS_R			0x3F7

#define ATA_SECONDARY_DATA_RW				0x170
#define ATA_SECONDARY_ERROR_R				0x171
#define ATA_SECONDARY_FEATURES_W			0x171
#define ATA_SECONDARY_SECTOR_COUNT_RW		0x172
#define ATA_SECONDARY_SECTOR_NUMBER_RW		0x173
#define ATA_SECONDARY_CYLINDER_LOW_RW		0x174
#define ATA_SECONDARY_CYLINDER_HIGH_RW		0x175
#define ATA_SECONDARY_DRIVE_HEAD_RW			0x176
#define ATA_SECONDARY_STATUS_R				0x177
#define ATA_SECONDARY_COMMAND_W				0x177
#define ATA_SECONDARY_ALTERNATE_STATUS_R	0x376
#define ATA_SECONDARY_DEVICE_CONTROL_W		0x376
#define ATA_SECONDARY_DRIVE_ADDRESS_R		0x377

// ATA Commands
#define ATA_READ_SECTOR   					0x20
#define ATA_READ_SECTOR_WITHOUT_RETRY  		0x21
#define ATA_READ_LONG						0x22
#define ATA_READ_LONG_WITHOUT_RETRY			0x23
#define ATA_READ_SECTOR_EXT					0x24
#define ATA_READ_MULTIPLE_EXT				0x29
#define ATA_READ_STREAM						0x2B
#define ATA_READ_LOG_EXT					0x2F
#define ATA_WRITE_SECTOR					0x30
#define ATA_WRITE_SECTOR_WITHOUT_RETRY		0x31
#define ATA_WRITE_LONG						0x32
#define ATA_WRITE_LONG_WITHOUT_RETRY		0x33
#define ATA_WRITE_SECTOR_EXT				0x34
#define ATA_CFA_WRITE_SECTOR_WITHOUT_ERASE	0x38
#define ATA_WRITE_MULTIPLE_EXT				0x39
#define ATA_WRITE_STREAM_EXT				0x3B
#define ATA_WRITE_VERIFY					0x3C
#define ATA_WRITE_LOG_EXT					0x3F
#define ATA_FORMAT_TRACK					0x50
#define ATA_TRUSTED_RECIEVE					0x5C
#define ATA_TRUSTED_SEND					0x5E
#define ATA_CFA_TRANSLATE_SECTOR			0x87
#define ATA_DOWNLOAD_MICROCODE				0x92
#define ATA_IDENTIFY_PACKET_DEVICE			0xA1
#define ATA_SMART							0xB0
#define ATA_DEVICE_CONFIGURATION_OVERLAY	0xB1
#define ATA_READ_MULTIPLE					0xC4
#define ATA_WRITE_MULTIPLE					0xC5
#define ATA_CFA_WRITE_MULTIPLE_WITHOUT_ERASE 0xCD
#define ATA_WRITE_MULTIPLE_FUA_EXT			0xCE
#define ATA_READ_BUFFER						0xE4
#define ATA_WRITE_BUFFER					0xE8
#define ATA_IDENTIFY_DEVICE					0xEC
#define ATA_SECURITY_SET_PASSWORD			0xF1
#define ATA_SECURITY_UNLOCK					0xF2
#define ATA_SECURITY_ERASE_UNIT				0xF4
#define ATA_SECURITY_DISABLE_PASSWORD		0xF6


typedef enum ata_error_t {
	normal, error, AMNF, TKZNF, ABRT, MCR, IDNF, MC, UNC, BBK
} ata_error_t ;

typedef struct ata_sector_data_t {
	uint8_t data[512];
} ata_sector_data_t;

bool ata_init();

// Primary
ata_sector_data_t ata_primary_read(uint64_t lba);
bool ata_primary_read_sector_request(uint64_t lba);
ata_error_t ata_primary_read_sector();

bool ata_primary_write(uint64_t lba, ata_sector_data_t data);
bool ata_primary_write_request(uint64_t lba);
bool ata_primary_write_sector(uint64_t lba, ata_sector_data_t data);

// Secondary
ata_sector_data_t ata_secondary_read(uint64_t lba);
bool ata_secondary_read_sector_request(uint64_t lba);
ata_error_t ata_secondary_read_sector();

bool ata_secondary_write(uint64_t lba, ata_sector_data_t data);
bool ata_secondary_write_request(uint64_t lba);
bool ata_secondary_write_sector(uint64_t lba, ata_sector_data_t data);