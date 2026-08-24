#pragma once

#include <stdint.h>
#include <stddef.h>
#include "fonts.h"

#define OLED_H 64
#define OLED_W 128
#define OLED_W_BYTES OLED_W/8
#define PAGES_NUMBER 8

typedef enum {
    SSD1306_OK           = 0,
    SSD1306_ERR_CLIPPED  = 1 << 0,
    SSD1306_ERR_INVALID_CHAR = 1 << 1,
} ssd1306_status_t;

extern uint8_t FRAME_BUFFER[OLED_H][OLED_W / 8];
extern uint8_t SSD_FRAME_BUFFER[PAGES_NUMBER][OLED_W];

uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* bitmap);
uint8_t draw_text(uint8_t x, uint8_t y, char* str, size_t length, font_id_t font_id);
void clear_frame_buffer(void);
void convert_frame_buffer_to_ssd_frame_buffer(void);