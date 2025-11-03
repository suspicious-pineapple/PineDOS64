#include "../limine-protocol/include/limine.h"
#include "kernel.h"
#include "drawing.h"



__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request executable_address_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 3
};





void dump_memmap(){
    print_string(&kglobals.console, "\r\n");
    print_hex64(&kglobals.console, executable_address_request.response->physical_base);
    print_string(&kglobals.console, "\r\n");
    print_hex64(&kglobals.console, executable_address_request.response->virtual_base);
    print_string(&kglobals.console, "\r\n");
    
    for(uint16_t i = 0; i < memmap_request.response->entry_count; i++){
        uint64_t type = memmap_request.response->entries[i]->type;
        uint64_t base = memmap_request.response->entries[i]->base;
        uint64_t length = memmap_request.response->entries[i]->length;

       //switch(type){
       //    case LIMINE_MEMMAP_USABLE: {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_RESERVED               : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_ACPI_RECLAIMABLE       : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_ACPI_NVS               : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_BAD_MEMORY             : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_FRAMEBUFFER             : {
       //        break;
       //    }
       //    case LIMINE_MEMMAP_ACPI_TABLES             : {
       //        break;
       //    }



       //}


        print_string(&kglobals.console, "base: ");
        print_hex64(&kglobals.console, base);
        print_string(&kglobals.console, ", length: ");
        print_hex64(&kglobals.console, length);
        print_string(&kglobals.console, ", type: ");
        print_hex_byte(&kglobals.console, type);
  
        print_string(&kglobals.console, "\r\n");

    }


    render_console(&kglobals.console);

}

























