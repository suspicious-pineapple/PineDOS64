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


//stolen from osdev.wiki, dont tell anyone
struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__ ((packed));



struct XSDT {
  struct ACPISDTHeader h;
  uint64_t PointerToOtherSDT[(h.Length - sizeof(h)) / 8];
};


typedef struct sysinfo {
    struct XSDP xsdp;
    struct XSDT xsdt;
    
} sysinfo_t;


extern sysinfo_t system;






