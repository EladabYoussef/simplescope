#include "ssd1306_gfx.h"


void ssd1306_write_cmd(uint8_t cmd);
void ssd1306_write_data(uint8_t *data, size_t length);
void ssd1306_init(void);
void ssd1306_update_screen(void);
