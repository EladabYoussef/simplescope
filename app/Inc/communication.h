#pragma once

#include <stdint.h>
#include "config.h"
#include "stm32f4xx_hal_uart.h"

#define PACKET_MAX_SAMPLES SAMPLES_PER_CHANNEL

typedef struct {
    selected_channel_e channel;
    uint16_t data[PACKET_MAX_SAMPLES];
    uint32_t length;
    uint16_t crc;
}packet_t;

extern UART_HandleTypeDef *uart2_handle;
extern packet_t tx_packet;

packet_status_e create_packet(const selected_channel_e channel, const uint16_t *data, const uint32_t length);
transmit_status_e transmit_packet(const packet_t *packet);