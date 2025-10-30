
#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
void draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void draw_character(uint8_t character, uint16_t x, uint16_t y, uint32_t fgcolor, uint32_t bgcolor, uint8_t scale);
void put_char(uint8_t character, uint16_t column, uint16_t row, uint8_t tcolor);
void render_console();
void draw_rect(uint16_t x, uint16_t y, uint16_t width,uint16_t height,uint32_t color);
void print_char(uint8_t character);
void print_string(uint8_t* string);
#endif
