#pragma once
#include <stdint.h>

struct XSDP {
 char signature[8];
 uint8_t checksum;
 char oem_id[6];

 uint8_t revision;
 uint32_t rsdt_addr;      // deprecated since version 2.0

 uint32_t length;
 uint64_t xsdt_addr;

 uint8_t extended_checksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

typedef struct sysinfo {
    struct XSDP xsdp;

} sysinfo_t;









