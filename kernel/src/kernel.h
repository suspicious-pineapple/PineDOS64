
#ifndef KERNEL_H
#define KERNEL_H

#define console_max_columns 72
#define console_max_rows 40

#define max_supported_memory 274877906944



#include <stdint.h>
#include <stddef.h>
typedef struct framebuffer_t {
    volatile uint32_t *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
} framebuffer_t;

typedef struct __attribute__((packed)) character_entry {
    uint8_t character;
    uint8_t flags;
    uint32_t foreground;
    uint32_t background;
} character_entry_t;

typedef struct console_buffer_t {
    character_entry_t buffer[console_max_columns*console_max_rows];
    uint16_t rows;
    uint16_t columns;
    uint16_t cursor_col;
    uint16_t cursor_row;
    uint32_t default_background;
    uint32_t default_foreground;
    uint8_t display_scale;
} console_buffer_t;


typedef struct kernel_values_t {
    framebuffer_t framebuffer;
    console_buffer_t console;
    uint64_t MADT;
} kernel_values_t;

extern kernel_values_t kglobals;



void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memmove(void *restrict dest, const void *restrict src, size_t n);





#endif