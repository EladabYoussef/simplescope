#pragma once

#include <stdint.h>
#include <stddef.h>

#define OLED_H 64
#define OLED_W 128
#define PAGES_NUMBER 8

typedef enum {
    SSD1306_OK           = 0,
    SSD1306_ERR_CLIPPED  = 1 << 0,
    SSD1306_ERR_INVALID_CHAR = 1 << 1,
} ssd1306_status_t;

uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* bitmap);
uint8_t draw_text(uint8_t x, uint8_t y, char* str, size_t length, uint8_t font_size);
void clear_frame_buffer(void);  