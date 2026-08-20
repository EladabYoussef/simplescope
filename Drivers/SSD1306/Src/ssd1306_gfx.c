
#include "ssd1306_gfx.h"
uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t** bitmap) {
    if(x > OLED_H || y > OLED_W) {
        return 1;
    }
    uint8_t final_x = (x + width) > OLED_W ? OLED_W : (x + width);
    uint8_t final_y = (y + height) > OLED_H ? OLED_H : (y + height);

    for(uint8_t i = x; i < final_x; i++) {
        memcpy(&FRAME_BUFFER[i][y / 8], &bitmap[i - x][0], (final_y - y) / 8);
    }
    return 0;
}