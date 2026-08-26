#include "ssd1306_hal.h"
#include "ssd1306_gfx.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2c.h"
#include <string.h>

#define SSD1306_ADDRESS 0x3C << 1

#define SSD1306_DATA_CONTROL_BYTE 0x40
#define SSD1306_CMD_CONTROL_BYTE 0x00

#define SSD1306_CMD_SET_CONTRAST_CTRL      0x81
#define SSD1306_CMD_ENTIRE_DISPLAY_RESUME  0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON      0xA5
#define SSD1306_CMD_NORMAL_DISPLAY         0xA6
#define SSD1306_CMD_INVERT_DISPLAY         0xA7
#define SSD1306_CMD_DISPLAY_OFF            0xAE
#define SSD1306_CMD_DISPLAY_ON             0xAF

#define SSD1306_CMD_DEACTIVATE_SCROLL      0x2E

#define SSD1306_CMD_SET_MEMORY_ADDR_MODE   0x20
#define SSD1306_ADDR_MODE_HORIZONTAL       0x00
#define SSD1306_ADDR_MODE_VERTICAL         0x01
#define SSD1306_ADDR_MODE_PAGE             0x02

#define SSD1306_CMD_SET_START_LINE         0x40
#define SSD1306_CMD_SET_SEGMENT_REMAP_OFF  0xA0
#define SSD1306_CMD_SET_SEGMENT_REMAP_ON   0xA1
#define SSD1306_CMD_SET_MULTIPLEX_RATIO    0xA8
#define SSD1306_CMD_SET_COM_SCAN_INC       0xC0
#define SSD1306_CMD_SET_COM_SCAN_DEC       0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET     0xD3
#define SSD1306_CMD_SET_COM_PINS_CONFIG    0xDA

#define SSD1306_CMD_SET_CLOCK_DIV_RATIO    0xD5
#define SSD1306_CMD_SET_PRECHARGE_PERIOD   0xD9
#define SSD1306_CMD_SET_VCOMH_DESELECT     0xDB

#define SSD1306_CMD_SET_CHARGE_PUMP        0x8D
#define SSD1306_CHARGE_PUMP_DISABLE        0x10
#define SSD1306_CHARGE_PUMP_ENABLE         0x14

#define SSD1306_CLOCK_DIV_DEFAULT          0x80
#define SSD1306_MULTIPLEX_RATIO_64         0x3F
#define SSD1306_DISPLAY_OFFSET_NONE        0x00
#define SSD1306_COM_PINS_ALT_NO_REMAP      0x12
#define SSD1306_CONTRAST_MID               0x7F
#define SSD1306_PRECHARGE_PHASE1_2_15DCLK  0xF1
#define SSD1306_VCOMH_DESELECT_0_77VCC     0x40

#define SSD1306_CMD_SET_COLUMN_ADDR        0x21 
#define SSD1306_CMD_SET_PAGE_ADDR          0x22

static uint32_t ssd1306_i2c_timeout_ms = SSD1306_I2C_TIMEOUT_DEFAULT_MS;

I2C_HandleTypeDef *i2c1_handle;

void ssd1306_set_i2c_timeout(uint32_t timeout_ms) {
    ssd1306_i2c_timeout_ms = timeout_ms;
}

uint32_t ssd1306_get_i2c_timeout(void) {
    return ssd1306_i2c_timeout_ms;
}

HAL_StatusTypeDef ssd1306_write_cmd(uint8_t cmd) {
    if (i2c1_handle == NULL) {
        return HAL_ERROR;
    }
    uint8_t buffer[] = {SSD1306_CMD_CONTROL_BYTE, cmd};
    return HAL_I2C_Master_Transmit(i2c1_handle, SSD1306_ADDRESS, buffer, sizeof(buffer), ssd1306_i2c_timeout_ms);
}

HAL_StatusTypeDef ssd1306_write_data(uint8_t *data, size_t length) {
    if (i2c1_handle == NULL || data == NULL) {
        return HAL_ERROR;
    }
    uint8_t buffer[length + 1];
    buffer[0] = SSD1306_DATA_CONTROL_BYTE;
    memcpy(&buffer[1], data, length);
    return HAL_I2C_Master_Transmit(i2c1_handle, SSD1306_ADDRESS, buffer, length + 1, ssd1306_i2c_timeout_ms);
}

HAL_StatusTypeDef ssd1306_init(void)
{
    if (i2c1_handle == NULL) {
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_DISPLAY_OFF)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_CLOCK_DIV_RATIO)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_CLOCK_DIV_DEFAULT)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_MULTIPLEX_RATIO)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_MULTIPLEX_RATIO_64)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_DISPLAY_OFFSET)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_DISPLAY_OFFSET_NONE)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_START_LINE | 0x00)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_CHARGE_PUMP)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_CHARGE_PUMP_ENABLE)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_MEMORY_ADDR_MODE)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_ADDR_MODE_HORIZONTAL)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_SEGMENT_REMAP_ON)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_COM_SCAN_DEC)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_COM_PINS_CONFIG)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_COM_PINS_ALT_NO_REMAP)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_CONTRAST_CTRL)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_CONTRAST_MID)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_PRECHARGE_PERIOD)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_PRECHARGE_PHASE1_2_15DCLK)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_VCOMH_DESELECT)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_VCOMH_DESELECT_0_77VCC)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_ENTIRE_DISPLAY_RESUME)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(SSD1306_CMD_NORMAL_DISPLAY)) != HAL_OK) return status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_DISPLAY_ON)) != HAL_OK) return status;

    return HAL_OK;
}

HAL_StatusTypeDef ssd1306_update_screen(void) {
    if (i2c1_handle == NULL) {
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status;

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_COLUMN_ADDR)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(0)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(OLED_W - 1)) != HAL_OK) return status;  // column range

    if ((status = ssd1306_write_cmd(SSD1306_CMD_SET_PAGE_ADDR)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(0)) != HAL_OK) return status;
    if ((status = ssd1306_write_cmd(PAGES_NUMBER - 1)) != HAL_OK) return status;  // page range (all pages)

    convert_frame_buffer_to_ssd_frame_buffer();
    if ((status = ssd1306_write_data((uint8_t *)SSD_FRAME_BUFFER, sizeof(SSD_FRAME_BUFFER))) != HAL_OK) return status;

    return HAL_OK;
    
}