// servo_motor.h

#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32f4xx_hal.h"

void Servo_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void Servo_SetAngle(uint8_t angle);

#endif
