#include "ssd1306_gfx.h"
#include <stdint.h>
uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap) {
    if(x >= OLED_W || y >= OLED_H) {
        return 1;
    }
    uint8_t final_x = (x + width) > OLED_W ? OLED_W : (x + width);
    uint8_t final_y = (y + height) > OLED_H ? OLED_H : (y + height);
    uint8_t begin_off = x % 8, end_off = final_x % 8;
    uint8_t begin_col = x/8, final_col = final_x/8;
    uint8_t stride = (width + 7) / 8;

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
            carry |= (bitmap[(row-y)*stride + (col-begin_col)] >> begin_off);
            FRAME_BUFFER[row][col] &= ~(mask);
            FRAME_BUFFER[row][col] |= carry;
            mask = 0xFFu;
            carry = (bitmap[(row-y)*stride + (col-begin_col)] << (8-begin_off));
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
    if(final_x != x+width || final_y != y+height) return 1;

    return 0;
}