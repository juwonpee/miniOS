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


typedef struct acpi_rsdp_descriptor_t {
	uint8_t signature[8];
	uint8_t checksum;
	uint8_t OEMID[6];
	uint8_t revision;
	uint32_t rsdtAddress;
	uint32_t length;
	uint64_t xsdpAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} acpi_rsdp_descriptor_t;

typedef struct acpi_sdt_header {
	uint8_t signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t OEMID[6];
	uint8_t OEMTableID[8];
	uint32_t OENRevision;
	uint32_t creatorID;
	uint32_t creatorRevision;
} acpi_sdt_header;

typedef struct 
