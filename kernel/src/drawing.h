#pragma once
#include <stdint.h>
void put_pixel(uint16_t x, uint16_t y, uint32_t color);
void draw_character(uint8_t character, uint16_t x, uint16_t y, uint32_t color, uint8_t scale);
void put_char(uint8_t character, uint16_t column, uint16_t row, uint8_t tcolor);
void render_console();
void put_rect(uint16_t x, uint16_t y, uint16_t width,uint16_t height,uint32_t color);
