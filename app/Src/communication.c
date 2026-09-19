#include "communication.h"
#include "stm32f4xx_hal_uart.h"
#include <string.h>

#define TRANSMIT_TIMEOUT 100

packet_t tx_packet;
UART_HandleTypeDef *uart2_handle;

packet_status_e create_packet(const selected_channel_e channel, const uint16_t *data, const uint32_t length) {
    if (channel == NO_CHANNEL_SELECTED || data == NULL || length > PACKET_MAX_SAMPLES) {
        return PACKET_ERROR;
    }

    tx_packet.channel = channel;
    tx_packet.length = length;
    memcpy(tx_packet.data, data, length * sizeof(uint16_t));

    uint16_t crc = 0;
    for (uint32_t i = 0; i < length; i++) {
        crc += (uint16_t)data[i];
    }
    tx_packet.crc = crc;

    return PACKET_OK;
}

transmit_status_e transmit_packet(const packet_t *packet) {
    if (packet == NULL || packet->length > PACKET_MAX_SAMPLES
        || packet->channel == NO_CHANNEL_SELECTED) {
        return INVALID_PACKET_ERROR;
    }

    if (HAL_UART_Transmit(uart2_handle, (uint8_t *)packet, sizeof(packet_t), TRANSMIT_TIMEOUT) != HAL_OK) {
        return TRANSMIT_ERROR;
    }
    return TRANSMIT_OK;
}

