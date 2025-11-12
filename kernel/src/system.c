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

    if(system.xsdp->revision != 0 && checksum_xsdp==0){
        print_string(&kglobals.console, "XSDP supported!\r\n");
        
        
    } else if(system.xsdp->revision==0 && checksum_rsdp==0){
        print_string(&kglobals.console, "Falling back to RSDP\r\n");
        
    } else {
        print_string(&kglobals.console, "Something's fucked. where tf is the rsdp\r\n");
        render_console(&kglobals.console);
        while(1){};
    }

    

    


    print_string(&kglobals.console, "\r\n");

}

















