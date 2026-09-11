#pragma once

#define N_CHANNELS 2
#define SAMPLES_PER_CHANNEL 256

typedef enum {
    NO_CHANNEL_SELECTED,
    CHANNEL_1,
    CHANNEL_2
} selected_channel_t;

typedef enum {
    NO_TRANSFER,
    FIRST_HALF_TRANSFER,
    SECOND_HALF_TRANSFER
}buffer_half_transfer_t;