#include "system.h"
#include "kernel.h"
#include "drawing.h"
volatile sysinfo_t system;

void init_system(){
    system.xsdp = system.xsdp; 
    system.xsdt = (struct XSDT*)((system.xsdp->xsdt_addr));
    
    print_string(&kglobals.console, "\r\nXSDT Set! Signature: ");
    //print_string(&kglobals.console, system.xsdt->header.OEMID);
    print_hex64(&kglobals.console, (uint64_t)system.xsdt);
 
    print_string(&kglobals.console, "\r\n");

}

















