
#include "ssd1306_gfx.h"
#include <stdint.h>
uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t** bitmap) {
    if(x > OLED_H || y > OLED_W) {
        return 1;
    }
    uint8_t final_x = (x + height) > OLED_H ? OLED_H : (x + height);
    uint8_t final_y = (y + width) > OLED_W ? OLED_W : (y + width);
    uint8_t begin_off = y % 8,end_off = final_y % 8;
    uint8_t begin_col = y/8, final_col=final_y/8;   

    for(uint8_t row = x; row < final_x; row++) {
        uint8_t carry = 0;
        uint8_t mask = 0xFFu & ((1<<(8-begin_off)) - 1);
        
        for(uint8_t col = begin_col; col < final_col; col++) {
            carry |= (bitmap[row-x][col-begin_col] >> begin_off);
            FRAME_BUFFER[row][col] &= ~(mask);
            FRAME_BUFFER[row][col] |= carry;
            mask = 0xFFu;
            carry = (bitmap[row-x][col-begin_col] << (8-begin_off));
        }
        mask = 0xFFu & ~((1<<(8-end_off)) - 1);
        carry |= (bitmap[row-x][col-begin_col]>>begin_off) & mask;
        FRAME_BUFFER[row][final_col] &= ~(mask);
        FRAME_BUFFER[row][final_col] |= carry;
    }
    if(final_x != x+height || final_y != y+width) return 1;

    return 0;
}