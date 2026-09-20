#pragma once

#include <stm32f4xx_hal.h>
#include "config.h"

extern volatile uint16_t adc_buffer[N_CHANNELS * SAMPLES_PER_CHANNEL];
extern volatile uint32_t data_transfer_flag;

extern ADC_HandleTypeDef *adc1_handle;

void acquisition_on_dma_complete(ADC_HandleTypeDef *hadc);
void acquisition_on_dma_half_complete(ADC_HandleTypeDef *hadc);