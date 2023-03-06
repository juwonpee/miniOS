#pragma once

// PCI TYPES
typedef struct pci_configuration_space_t {
	uint64_t baseAddress;
	uint16_t group_segment_number;
	uint8_t startBus;
	uint8_t endBus;
	uint32_t reserved;
} __attribute__ ((packed)) pci_configuration_space_t;

typedef struct pci_device_header_t {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t program_interface;
	uint8_t subClass;
	uint8_t class;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t BIST;
} __attribute__ ((packed)) pci_device_header_t;