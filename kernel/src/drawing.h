
#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
void draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void draw_character(uint8_t character, uint16_t x, uint16_t y, uint32_t fgcolor, uint32_t bgcolor, uint8_t scale);
void put_char(console_buffer_t *console, uint8_t character, uint16_t column, uint16_t row, uint32_t foreground, uint32_t background);
void render_console(console_buffer_t *console);
void draw_rect(uint16_t x, uint16_t y, uint16_t width,uint16_t height,uint32_t color);
void print_char(console_buffer_t *console, uint8_t character);
void print_string(console_buffer_t *console, char* string);
void print_hex64(console_buffer_t* console, uint64_t number);
void print_hex_byte(console_buffer_t* console, uint8_t number);
void scroll_console(console_buffer_t *console);
#endif
