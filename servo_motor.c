// servo_motor.c

#include "servo_motor.h"

static TIM_HandleTypeDef *p_servo_timer = NULL;
static uint32_t servo_channel = 0U;

void Servo_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
    if (htim != NULL)
    {
        p_servo_timer = htim;
        servo_channel = channel;
        (void)HAL_TIM_PWM_Start(htim, channel);
    }
}

void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180U)
    {
        angle = 180U;
    }

    if (p_servo_timer != NULL)
    {
        uint32_t pulse_length = (uint32_t)(((angle * 10U) / 9U) + 500U);
        __HAL_TIM_SET_COMPARE(p_servo_timer, servo_channel, pulse_length);
    }
}
