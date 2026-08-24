#include "ssd1306_gfx.h"
#include <stdint.h>
#include <string.h>

uint8_t FRAME_BUFFER[OLED_H][OLED_W / 8];
uint8_t SSD_FRAME_BUFFER[PAGES_NUMBER][OLED_W];

static inline uint8_t bitmap_stride(uint8_t width) {
    return (width + 7) >> 3;
}

static inline uint16_t bitmap_bytes(uint8_t width, uint8_t height) {
    return (uint16_t)bitmap_stride(width) * height;
}

static inline uint8_t reverse_bits(uint8_t value)
{
    value = ((value & 0x55u) << 1) | ((value >> 1) & 0x55u);
    value = ((value & 0x33u) << 2) | ((value >> 2) & 0x33u);
    return (value << 4) | (value >> 4);
}

static inline void transpose8x8(uint8_t A[8]) {
    uint32_t x, y, t;

    x = ((uint32_t)A[0] << 24) | ((uint32_t)A[1] << 16) | ((uint32_t)A[2] << 8) | A[3];
    y = ((uint32_t)A[4] << 24) | ((uint32_t)A[5] << 16) | ((uint32_t)A[6] << 8) | A[7];

    t = (x ^ (x >> 7))  & 0x00AA00AAu;  x ^= t ^ (t << 7);
    t = (y ^ (y >> 7))  & 0x00AA00AAu;  y ^= t ^ (t << 7);

    t = (x ^ (x >> 14)) & 0x0000CCCCu;  x ^= t ^ (t << 14);
    t = (y ^ (y >> 14)) & 0x0000CCCCu;  y ^= t ^ (t << 14);

    t = (x & 0xF0F0F0F0u) | ((y >> 4) & 0x0F0F0F0Fu);
    y = ((x << 4) & 0xF0F0F0F0u) | (y & 0x0F0F0F0Fu);
    x = t;

    A[0] = x >> 24; A[1] = x >> 16; A[2] = x >> 8; A[3] = x;
    A[4] = y >> 24; A[5] = y >> 16; A[6] = y >> 8; A[7] = y;
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

uint8_t draw_text(uint8_t x, uint8_t y, char* str, size_t length, font_id_t font_id) {
    if (font_id >= FONT_COUNT) return SSD1306_ERR_INVALID_CHAR;

    const font_t *font = &font_cga_16;
    uint8_t font_width = font->byte_width * 8;
    uint16_t bytes_per_char = bitmap_bytes(font_width, font->height);
    if((x+font_width) > OLED_W || (y+font->height) > OLED_H) return SSD1306_ERR_CLIPPED;

    uint8_t runningX = x;
    uint8_t runningY = y;
    uint8_t flag = SSD1306_OK;
    for(size_t i = 0; i < length; i++) {
        if (str[i] < font->lochar || str[i] > font->hichar) {
            flag |= SSD1306_ERR_INVALID_CHAR;
            continue;
        }
        uint32_t index = (uint32_t)bytes_per_char * (uint32_t)(str[i] - font->lochar);
        if((runningX + font_width) > OLED_W) {
            runningX = x;
            runningY += font->height;
        }
        if((runningY+font->height) > OLED_H) {
            flag |= SSD1306_ERR_CLIPPED;
            return flag;
        }
        flag |= draw_bitmap(runningX, runningY, font_width, font->height, &font->data[index]);
        runningX += font_width;
    }
    return flag;
}

void clear_frame_buffer(void) {
    memset(FRAME_BUFFER, 0, sizeof(FRAME_BUFFER));
    memset(SSD_FRAME_BUFFER, 0, sizeof(SSD_FRAME_BUFFER));
}

void convert_frame_buffer_to_ssd_frame_buffer(void) {
    for (uint8_t page = 0; page < PAGES_NUMBER; page++) {
        for (uint8_t col = 0; col < OLED_W_BYTES; col++) {
            uint8_t block[8];

            for(uint8_t i = 0; i < 8; i++) {
                block[i] = FRAME_BUFFER[page*8 + i][col];
            }
            transpose8x8(block);
            for(uint8_t i = 0; i < 8; i++) {
                SSD_FRAME_BUFFER[page][col*8 + i] = reverse_bits(block[7 - i]);
            }
        }
    }
}