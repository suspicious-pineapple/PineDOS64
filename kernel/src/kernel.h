#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct framebuffer_t {
    volatile uint32_t *fb_ptr;
    volatile uint32_t *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
} framebuffer_t;

typedef struct kernel_values_t {
    framebuffer_t framebuffer;
    
} kernel_values_t;

extern kernel_values_t kglobals;
