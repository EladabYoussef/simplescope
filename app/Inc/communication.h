#pragma once

#include <stdbool.h>
#include "config.h"

#define PACKET_MAX_SAMPLES SAMPLES_PER_CHANNEL

typedef struct {
    selected_channel_t channel;
    uint16_t data[PACKET_MAX_SAMPLES];
    uint32_t length;
    uint16_t crc;
}packet_t;

extern packet_t tx_packet;

bool create_packet(selected_channel_t channel, const uint16_t *data, uint32_t length);
bool transmit_packet(const packet_t *packet);