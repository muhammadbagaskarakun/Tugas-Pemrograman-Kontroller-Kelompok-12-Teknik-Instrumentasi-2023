// fsr_sensor.h

#ifndef FSR_SENSOR_H
#define FSR_SENSOR_H

#include "stm32f4xx_hal.h"

void FSR_Init(ADC_HandleTypeDef *hadc);
uint16_t FSR_ReadPressure(void);

#endif
