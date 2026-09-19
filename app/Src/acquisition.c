#include "acquisition.h"


volatile uint16_t adc_buffer[N_CHANNELS * SAMPLES_PER_CHANNEL];
volatile uint32_t data_transfer_flag = NO_TRANSFER;
ADC_HandleTypeDef *adc1_handle;


void acquisition_on_dma_half_complete(ADC_HandleTypeDef *hadc) {
    if(hadc != NULL && hadc->Instance == ADC1) {
        data_transfer_flag = FIRST_HALF_TRANSFER;
    }
}

void acquisition_on_dma_complete(ADC_HandleTypeDef *hadc) {
    if (hadc != NULL && hadc->Instance == ADC1) {
        data_transfer_flag = SECOND_HALF_TRANSFER;
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    acquisition_on_dma_half_complete(hadc);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    acquisition_on_dma_complete(hadc);
}