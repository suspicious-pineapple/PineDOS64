#pragma once
#include <stdint.h>
void dump_memmap();
uint8_t page_bitmap_get(size_t addr);
void page_bitmap_set(size_t addr, uint8_t state);
void init_gdt();

typedef struct  __attribute__((packed, aligned(16))) gdtr {
    uint16_t size;
    uint64_t offset;
} gdtr_t;

extern void load_gdt(uint16_t size, uint64_t addr);
extern void load_tss(uint16_t);