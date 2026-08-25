#pragma once

#include "ssd1306_gfx.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef *i2c1_handle;

#define SSD1306_I2C_TIMEOUT_DEFAULT_MS 100

void ssd1306_set_i2c_timeout(uint32_t timeout_ms);
uint32_t ssd1306_get_i2c_timeout(void);

HAL_StatusTypeDef ssd1306_write_cmd(uint8_t cmd);
HAL_StatusTypeDef ssd1306_write_data(uint8_t *data, size_t length);
HAL_StatusTypeDef ssd1306_init(void);
HAL_StatusTypeDef ssd1306_update_screen(void);
