#define OLED_H 64
#define OLED_W 128
#define PAGES_NUMBER 8

uint8_t FRAME_BUFFER[OLED_H][OLED_W / 8];
uint8_t SSD_FRAME_BUFFER[PAGES_NUMBER][OLED_W];

uint8_t draw_bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t** bitmap);
uint8_t draw_text(uint8_t x, uint8_t y, char* str, size_t length, int font_size);
void convert_buffer();
