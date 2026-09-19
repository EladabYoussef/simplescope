#pragma once

#define N_CHANNELS 2
#define SAMPLES_PER_CHANNEL 256

typedef enum {
    NO_CHANNEL_SELECTED,
    CHANNEL_1,
    CHANNEL_2
} selected_channel_e;

typedef enum {
    NO_TRANSFER,
    FIRST_HALF_TRANSFER,
    SECOND_HALF_TRANSFER
} buffer_half_transfer_e;

typedef enum {
    PACKET_OK,
    PACKET_ERROR
} packet_status_e;

typedef enum {
    INVALID_PACKET_ERROR,
    TRANSMIT_OK,
    TRANSMIT_ERROR
} transmit_status_e;