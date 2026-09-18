#include "channels.h"

volatile uint32_t active_channel_flag = NO_CHANNEL_SELECTED;

channel_t channel1 = {
    .channel_on = false,
    .led_port = GPIOD,
    .led_pin = GPIO_PIN_12,
    .btn_port = GPIOD,
    .btn_pin = GPIO_PIN_1
};

channel_t channel2 = {
    .channel_on = false,
    .led_port = GPIOD,
    .led_pin = GPIO_PIN_13,
    .btn_port = GPIOD,
    .btn_pin = GPIO_PIN_2
};

void btn1_callback(void) {
    active_channel_flag = CHANNEL_1;
}

void btn2_callback(void) {
    active_channel_flag = CHANNEL_2;
}

//there will always be a channel displayed, that does not matter since this is only for learning and the logic for toggling is pretty simple
void handle_event(channel_t *target, channel_t *other) {
    other->channel_on = false;
    target->channel_on = true;
    HAL_GPIO_WritePin(other->led_port, other->led_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(target->led_port, target->led_pin, GPIO_PIN_SET);
}