#pragma once
/**
 * @brief SSD1306 OLED display driver (I2C interface)
 * 
 * @note I2C Speed Requirements:
 *   - Minimum: 100 kHz (Standard Mode)
 *   - Recommended: 400 kHz (Fast Mode)
 *   - Maximum: 1 MHz (Fast Mode+)
 *   
 * @note Timeout Configuration:
 *   - Default: 100ms (suitable for 100-400 kHz)
 *   - For 1 MHz bus: Consider reducing to 50ms or lower
 *   - Use ssd1306_set_i2c_timeout() to adjust based on your I2C speed
 */

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
