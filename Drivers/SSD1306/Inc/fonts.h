#pragma once
#include <stdint.h>

typedef struct {
	const uint8_t *data;
	uint8_t lochar;
	uint8_t hichar;
	uint8_t byte_width;
	uint8_t height;
} font_t;

typedef enum {
	FONT_CGA_16 = 0,
	FONT_COUNT
} font_id_t;

extern const font_t font_cga_16;