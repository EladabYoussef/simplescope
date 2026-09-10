#include <stm32f4xx_hal_adc.h>

#define N_CHANNELS 2
#define SAMPLES_PER_CHANNEL 256

extern volatile uint16_t adc_buffer[N_CHANNELS * SAMPLES_PER_CHANNEL];
void acquisition_on_dma_complete(ADC_HandleTypeDef *hadc);
void acquisition_on_dma_half_complete(ADC_HandleTypeDef *hadc);
