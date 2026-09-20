#include "display.h"
#include "ssd1306_hal.h"

#define ADC_MAX_VALUE 4095u

static void draw_pixel(uint8_t x, uint8_t y)
{
	FRAME_BUFFER[y][x / 8u] |= (uint8_t)(1u << (x % 8u));
}

static uint8_t sample_to_row(uint16_t sample)
{
	if (sample > ADC_MAX_VALUE) {
		sample = ADC_MAX_VALUE;
	}

	return (uint8_t)((OLED_H - 1u) -
		((uint32_t)sample * (OLED_H - 1u)) / ADC_MAX_VALUE);
}

static void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	int16_t dx = (int16_t)x1 - x0;
	int16_t dy = (int16_t)y1 - y0;
	int16_t steps = (dx < 0 ? -dx : dx) > (dy < 0 ? -dy : dy)
		? (dx < 0 ? -dx : dx) : (dy < 0 ? -dy : dy);

	if (steps == 0) {
		draw_pixel(x0, y0);
		return;
	}

	for (int16_t step = 0; step <= steps; step++) {
		uint8_t x = (uint8_t)(x0 + dx * step / steps);
		uint8_t y = (uint8_t)(y0 + dy * step / steps);
		draw_pixel(x, y);
	}
}

void display_signal(const volatile uint16_t *data, uint32_t length, selected_channel_e channel)
{
	if (data == NULL || (channel != CHANNEL_1 && channel != CHANNEL_2)) {
		return;
	}

	uint32_t offset = channel == CHANNEL_1 ? 0u : 1u;
	if (length <= offset) {
		return;
	}

	uint32_t sample_count = (length - offset + 1u) / 2u;
	if (sample_count == 0u) {
		return;
	}

	clear_frame_buffer();

	uint8_t previous_x = 0;
	uint8_t previous_y = sample_to_row(data[offset]);
	draw_pixel(previous_x, previous_y);

	for (uint32_t sample = 1u; sample < sample_count; sample++) {
		uint8_t x = (uint8_t)(sample * (OLED_W - 1u) / (sample_count - 1u));
		uint8_t y = sample_to_row(data[offset + sample * 2u]);
		draw_line(previous_x, previous_y, x, y);
		previous_x = x;
		previous_y = y;
	}

	(void)ssd1306_update_screen();
}
