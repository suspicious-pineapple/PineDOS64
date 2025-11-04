
#ifndef KERNEL_H
#define KERNEL_H

#define console_max_columns 144
#define console_max_rows 80

#define max_supported_memory 2^38



#include <stdint.h>
#include <stddef.h>

typedef struct framebuffer_t {
    volatile uint32_t *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
} framebuffer_t;

typedef struct character_entry {
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

} kernel_values_t;

extern kernel_values_t kglobals;

#endif