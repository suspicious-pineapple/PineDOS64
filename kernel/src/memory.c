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
    uint64_t total_usable = 0;
    for(uint16_t i = 0; i < memmap_request.response->entry_count; i++){
        uint64_t type = memmap_request.response->entries[i]->type;
        uint64_t base = memmap_request.response->entries[i]->base;
        uint64_t length = memmap_request.response->entries[i]->length;
        if(type==LIMINE_MEMMAP_USABLE){
            total_usable+=length;
        }
        
        print_string(&kglobals.console, "base: ");
        print_hex64(&kglobals.console, base);
        print_string(&kglobals.console, ",length:");
        print_hex64(&kglobals.console, length);

        switch(type){
            case LIMINE_MEMMAP_USABLE: {
                print_string(&kglobals.console, " usable, ");

                break;
            }
            case LIMINE_MEMMAP_RESERVED               : {
                print_string(&kglobals.console, " reserved, ");
                break;
            }
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE       : {
                print_string(&kglobals.console, " ACPI reclaimable");
                break;
            }
            case LIMINE_MEMMAP_ACPI_NVS               : {
                print_string(&kglobals.console, " ACPI NVS");
                
                break;
            }
            case LIMINE_MEMMAP_BAD_MEMORY             : {
                print_string(&kglobals.console, " Bad RAM");
                break;
            }
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE : {
                print_string(&kglobals.console, " Bootloader reclaimable");
                break;
            }
            case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES: {
                print_string(&kglobals.console, " Executable/Modules");
                break;
            }
            case LIMINE_MEMMAP_FRAMEBUFFER             : {
                print_string(&kglobals.console, " Framebuffer");
                break;
            }
            case LIMINE_MEMMAP_ACPI_TABLES             : {
                print_string(&kglobals.console, " ACPI tables");
                break;
            }
        }

        print_string(&kglobals.console, "\r\n");
        

  

    }
        print_string(&kglobals.console, "Total usable: ");
        print_hex64(&kglobals.console, total_usable);
        print_string(&kglobals.console, "\r\n");

    render_console(&kglobals.console);

}







uint64_t page_bitmap[(max_supported_memory / 4096 / 64)];


void page_bitmap_set(size_t addr){

}















