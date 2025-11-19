#include "system.h"
#include "kernel.h"
#include "drawing.h"
volatile sysinfo_t system;

void init_system(){
    system.xsdp = system.xsdp; 
    //system.xsdt = (struct XSDT*)((system.xsdp->xsdt_addr));
    
    print_string(&kglobals.console, "\r\nParsing XSDP..\r\nOEM ID:");
    char oem_str[7];
    print_string(&kglobals.console, "\r\nACPI ver.:");
    print_hex_byte(&kglobals.console, system.xsdp->revision);
    print_string(&kglobals.console, "\r\n");
    
    memcpy(oem_str, system.xsdp->oem_id,6);
    oem_str[6]=0;
    
    print_string(&kglobals.console, oem_str);
    print_string(&kglobals.console, "\r\n");
    
    uint8_t checksum_xsdp = 0;
    uint8_t checksum_rsdp = 0;
    
    for(uint16_t i = 0; i < sizeof(struct XSDP);i++){
        checksum_xsdp += *(uint8_t*)(i+(uint64_t)system.xsdp);
    }
    for(uint16_t i = 0; i < sizeof(struct RSDP);i++){
        checksum_rsdp += *(uint8_t*)(i+(uint64_t)system.xsdp);
    }
    
    
    
    
    //print_string(&kglobals.console, "\r\n");
    
    //}
    print_string(&kglobals.console, "\r\n________\r\nXSDP hexdump:");
    hex_dump(&kglobals.console, (uint64_t)system.xsdp,sizeof(struct XSDP));
    
    print_string(&kglobals.console, "\r\nChecksums for RSDP/XSDP:\r\n");
    print_hex_byte(&kglobals.console, checksum_rsdp);
    print_string(&kglobals.console, "\r\n");
    print_hex_byte(&kglobals.console, checksum_xsdp);
    print_string(&kglobals.console, "\r\n");

    if(system.xsdp->revision != 0 && checksum_xsdp==0){
        print_string(&kglobals.console, "XSDP supported!\r\n");
        system.acpi_revision = 1;
        system.xsdt = (struct XSDT*) ((system.xsdp->xsdt_addr) | 0xFFFF800000000000);
        
    } else if(system.xsdp->revision==0 && checksum_rsdp==0){
        print_string(&kglobals.console, "Falling back to RSDP\r\nActually, i lied. no acpi 1.0 support");
        render_console(&kglobals.console);

        system.acpi_revision = 0;
        while(1){};
        
    } else {
        print_string(&kglobals.console, "Something's fucked. where tf is the rsdp\r\n");
        render_console(&kglobals.console);
        while(1){};
    }


    uint32_t entries = (system.xsdt->header.length -sizeof(system.xsdt->header))/8;
    char signature[5] = {0,0,0,0,0}; 
    for(uint32_t i = 0; i < entries; i++){
        struct ACPISDTHeader *header = (struct ACPISDTHeader*) (system.xsdt->other_sdt[i] | 0xFFFF800000000000);
        memcpy(signature,header->signature,4);
        print_string(&kglobals.console, "\r\n");
        print_string(&kglobals.console, signature);
    }


    print_string(&kglobals.console, "\r\n");


    print_string(&kglobals.console, "MADT:");
    struct ACPISDTHeader* madt = find_acpi_table('CIPA');



};


struct ACPISDTHeader* find_acpi_table(uint32_t signature){
    print_string(&kglobals.console, "\r\n");
    print_hex64(&kglobals.console, signature);
    
    uint32_t entries = (system.xsdt->header.length -sizeof(system.xsdt->header))/8;
    for(uint32_t i = 0; i < entries; i++){
        struct ACPISDTHeader *header = (struct ACPISDTHeader*) (system.xsdt->other_sdt[i] | 0xFFFF800000000000);
            print_string(&kglobals.console, "\r\n");
        print_hex64(&kglobals.console, *(uint32_t*)(header->signature));
        uint8_t res = *(uint32_t*)(header->signature) == signature;
        
        if(res!=0){
            return header;
        }
    }
    
    panic("searching for invalid acpi table");

}














