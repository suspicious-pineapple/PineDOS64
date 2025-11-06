#include "../limine-protocol/include/limine.h"
#include "kernel.h"
#include "memory.h"
#include "drawing.h"
#include <stdbool.h>
#include "system.h"


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

                for(uint64_t j = 0; j < length; j+=4096){

                    
                    page_bitmap_set(j+base,1);
                    
                }
                
                break;
            }
            case LIMINE_MEMMAP_RESERVED               : {
                print_string(&kglobals.console, " reserved, ");
                break;
            }
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE       : {
                print_string(&kglobals.console, " ACPI reclaimable");

                //uint64_t signature = 'R' | ('S' << 8) | ('D' << 16) | (' ' << 24) | ('P' << 32) | ('T' << 40) | ('R' << 48) | (' ' << 56);
                
                for(uint64_t j = base; j < base+length; j+=8){
                    if( *(uint64_t*)(j|0xFFFF800000000000) == 0x2052545020445352){
                        print_string(&kglobals.console, "\r\nfound XSDT at:");
                        print_hex64(&kglobals.console, j|0xFFFF800000000000);
                    }
                }

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
            render_console(&kglobals.console);


  

    }
        print_string(&kglobals.console, "Total usable: ");
        print_hex64(&kglobals.console, total_usable);
        print_string(&kglobals.console, "\r\n");

    render_console(&kglobals.console);

}







uint64_t page_bitmap[(max_supported_memory / 4096 / 64)];


void page_bitmap_set(size_t addr, uint8_t state){
    size_t page_num = addr >> 12;
    uint8_t bit_num = page_num & 0x3F;
    size_t entry_num = page_num >> 6;
    entry_num = 400;

    uint64_t entry = page_bitmap[entry_num];
    entry = (entry& (~(1<<bit_num))) | ((state&1)<<bit_num);
    page_bitmap[entry_num] = entry;
}

uint8_t page_bitmap_get(size_t addr){
    size_t page_num = addr >> 12;
    uint8_t bit_num = page_num & 0x3F;
    size_t entry_num = page_num >> 6;

    uint64_t entry = page_bitmap[entry_num];
    uint8_t result = ((entry >> (bit_num))&1) == 1;
    return result;


}



uint64_t gdt[256];

uint16_t gdt_size;

uint64_t set_gdt_entry(uint16_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags){
    uint64_t value[8];
    value[0] = limit&0xFF;
    value[1] = (limit>>8)&0xFF;
    value[2] = (base)&0xFF;
    value[3] = (base>>8)&0xFF;
    value[4] = (base>>16)&0xFF;
    value[5] = access;
    value[6] = ((limit>>16)&0b1111) | ((flags&0b1110)<<4);
    value[7] = (base>>24)&0xFF;

    
    uint64_t qword = (value[0] << 0) | (value[1] << 8) | (value[2] << 16) | (value[3] << 24) | (value[4] << 32) | (value[5] << 40) | (value[6] << 48) | (value[7] << 56);
    gdt[entry] = qword;
    return qword;

}

void set_gdt_system_entry(uint16_t entry, uint64_t base, uint32_t limit, uint8_t access, uint8_t flags){
uint64_t value[16];
    value[0] = limit&0xFF;
    value[1] = (limit>>8)&0xFF;
    value[2] = (base)&0xFF;
    value[3] = (base>>8)&0xFF;
    value[4] = (base>>16)&0xFF;
    value[5] = access;
    value[6] = ((limit>>16)&0b1111) | ((flags&0b1110)<<4);
    value[7] = (base>>24)&0xFF;
    value[8] = (base>>32)&0xFF;
    value[9] = (base>>40)&0xFF;
    value[10] = (base>>48)&0xFF;
    value[11] = (base>>56)&0xFF;

    uint64_t qword1 = (value[0] << 0) | (value[1] << 8) | (value[2] << 16) | (value[3] << 24) | (value[4] << 32) | (value[5] << 40) | (value[6] << 48) | (value[7] << 56);
    gdt[entry] = qword1;

    uint64_t qword2 = (value[8] << 0) | (value[9] << 8) | (value[10] << 16) | (value[11] << 24);
    gdt[entry+1] = qword2;
}


uint8_t tss[0x69];

void init_gdt(){
    //magic numbers stolen from osdev wiki
    set_gdt_entry(0,0,0,0,0);
    set_gdt_entry(1,0,0xFFFF,0x9A,0xA);
    set_gdt_entry(2,0,0xFFFF,0x92,0xC);
    set_gdt_entry(3,0,0xFFFF,0xF2,0xC);
    set_gdt_entry(4,0,0xFFFF,0xFA,0xA);
    set_gdt_system_entry(5,(uint64_t)&tss,0x68,0x89,0x0);


    
    load_gdt(47, (uint64_t)&gdt);
    //load_tss(5*8);
    print_string(&kglobals.console, "\r\nGDT encoded: ");
    print_hex64(&kglobals.console,(uint64_t)&gdt);
    //print_hex64(&kglobals.console, set_gdt_entry(1,0,0xFFFF,0x9A,0xA));
    print_string(&kglobals.console, "\r\n");
}






