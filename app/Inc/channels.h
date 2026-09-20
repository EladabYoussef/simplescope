#pragma once

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "config.h"

extern volatile uint32_t active_channel_flag;

typedef struct {
    bool channel_on;
    GPIO_TypeDef *led_port;
    uint16_t led_pin;
    GPIO_TypeDef *btn_port;
    uint16_t btn_pin;

}channel_t;

extern channel_t channel1;
extern channel_t channel2;

void btn1_callback(void);
void btn2_callback(void);
void handle_event(channel_t *target, channel_t *other);