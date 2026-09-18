#pragma once

#include "ssd1306_gfx.h"
#include "config.h"

void display_signal(const uint16_t *data, uint32_t length, selected_channel_t channel);