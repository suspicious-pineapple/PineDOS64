#include "system.h"
#include "kernel.h"
#include "drawing.h"
volatile sysinfo_t system;

void init_system(){
    system.xsdp = system.xsdp; 
    //system.xsdt = (struct XSDT*)((system.xsdp->xsdt_addr));
    
    print_string(&kglobals.console, "\r\nParsing XSDP..\r\nOEM ID:");
    char oem_str[7];
    memcpy(oem_str, system.xsdp->oem_id,6);
    oem_str[6]=0;
    
    print_string(&kglobals.console, oem_str);
    print_string(&kglobals.console, "\r\nACPI ver.:");
    print_hex_byte(&kglobals.console, system.xsdp->revision);
    print_string(&kglobals.console, "\r\n");
    

    uint8_t checksum;
    for(uint16_t i = 0; i < sizeof(struct XSDP);i++){
        checksum = *(uint8_t*)(i+(uint64_t)system.xsdp);

    print_hex64(&kglobals.console, i);
 
    print_string(&kglobals.console, "\r\n");


    }


    print_hex64(&kglobals.console, checksum);
 
    print_string(&kglobals.console, "\r\n");

}

















