// fsr_sensor.c

#include "fsr_sensor.h"

static ADC_HandleTypeDef *p_adc_fsr = NULL;

// Inisialisasi modul ADC untuk FSR
void FSR_Init(ADC_HandleTypeDef *hadc)
{
    if (hadc != NULL)
    {
        p_adc_fsr = hadc;
    }
}

// Membaca nilai tekanan dari sensor FSR
uint16_t FSR_ReadPressure(void)
{
    uint16_t adc_val = 0U;

    if (p_adc_fsr != NULL)
    {
        if (HAL_ADC_Start(p_adc_fsr) != HAL_OK)
        {
            return 0U; // Kesalahan saat mulai ADC
        }

        if (HAL_ADC_PollForConversion(p_adc_fsr, HAL_MAX_DELAY) == HAL_OK)
        {
            adc_val = (uint16_t)HAL_ADC_GetValue(p_adc_fsr);
        }

        (void)HAL_ADC_Stop(p_adc_fsr);
    }

    return adc_val;
}
