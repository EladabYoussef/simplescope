#include "ssd1306_gfx.h"
#include "fonts.h"

uint8_t FRAME_BUFFER[OLED_H][OLED_W / 8];
uint8_t SSD_FRAME_BUFFER[PAGES_NUMBER][OLED_W];

static inline uint8_t bitmap_stride(uint8_t width) {
    return (width + 7) >> 3;
}

static inline uint16_t bitmap_bytes(uint8_t width, uint8_t height) {
    return (uint16_t)bitmap_stride(width) * height;
}

uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap) {
    if(bitmap == NULL) return SSD1306_ERR_INVALID_CHAR;
    if(x >= OLED_W || y >= OLED_H) return SSD1306_ERR_CLIPPED;

    uint8_t final_x = (x + width) > OLED_W ? OLED_W : (x + width);
    uint8_t final_y = (y + height) > OLED_H ? OLED_H : (y + height);
    uint8_t begin_off = x % 8, end_off = final_x % 8;
    uint8_t begin_col = x/8, final_col = final_x/8;
    uint8_t stride = bitmap_stride(width);
 
    for(uint8_t row = y; row < final_y; row++) {
        uint8_t carry = 0;
        if (begin_col == final_col) {
            uint8_t mask = (0xFFu >> begin_off) & ~(0xFFu >> end_off);
            uint8_t data = bitmap[(row-y)*stride + 0] >> begin_off;
            FRAME_BUFFER[row][begin_col] &= ~mask;
            FRAME_BUFFER[row][begin_col] |= data & mask;
            continue;
        }
 
        uint8_t mask = 0xFFu & ((1<<(8-begin_off)) - 1);
 
        for(uint8_t col = begin_col; col < final_col; col++) {
            uint8_t src_idx = col - begin_col;
            uint8_t src_byte = (src_idx < stride) ? bitmap[(row-y)*stride + src_idx] : 0;
            carry |= (src_byte >> begin_off);
            FRAME_BUFFER[row][col] &= ~(mask);
            FRAME_BUFFER[row][col] |= carry;
            mask = 0xFFu;
            carry = (src_byte << (8-begin_off));
        }
        if(end_off){
            mask = 0xFFu & ~((1<<(8-end_off)) - 1);
            uint8_t src_idx = final_col - begin_col;
            if (src_idx < stride) {
                carry |= (bitmap[(row-y)*stride + src_idx] >> begin_off) & mask;
            } else {
                carry &= mask;
            }
            FRAME_BUFFER[row][final_col] &= ~(mask);
            FRAME_BUFFER[row][final_col] |= carry;
        }
    }
    if(final_x != x+width || final_y != y+height) return SSD1306_ERR_CLIPPED;
 
    return SSD1306_OK;
}

uint8_t draw_text(uint8_t x, uint8_t y, char* str, size_t length, uint8_t font_size) {
    if((x+font_size) > OLED_W || (y+font_size) > OLED_H) return SSD1306_ERR_CLIPPED;
    
    const uint8_t *font_data = NULL;

    uint16_t bytes_per_char = bitmap_bytes(font_size, font_size);
    uint8_t runningX = x;
    uint8_t runningY = y;
    uint8_t flag = SSD1306_OK;
    switch (font_size) {
        default:
            font_data = CGA_16x16;
    }
    for(size_t i = 0; i < length; i++) {
        if (str[i] < ' ' || str[i] > '~') {
            flag |= SSD1306_ERR_INVALID_CHAR;
            continue;
        }
        uint32_t index = (uint32_t)bytes_per_char * (uint32_t)(str[i] - ' ');
        if((runningX + font_size) > OLED_W) {
            runningX = x;
            runningY += font_size;
        }
        if((runningY+font_size) > OLED_H) {
            flag |= SSD1306_ERR_CLIPPED;
            return flag;
        }
        flag |= draw_bitmap(runningX, runningY, font_size, font_size, &font_data[index]);
        runningX += font_size;
    }
    return flag;
}