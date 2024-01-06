//
// Created by fazhehy on 2023/12/28.
//

#include "ws2812b.h"

void ws2812b_send_bytes(uint32_t * data, uint16_t len)
{
    HAL_TIM_PWM_Start_DMA(&RGB_TIM, RGB_TIM_CHANNEL, data, len);
//    HAL_Delay(2);
}

void ws2812b_callback()
{
//    __HAL_TIM_SET_COMPARE(&RGB_TIM, RGB_TIM_CHANNEL, 0);
    HAL_TIM_PWM_Stop_DMA(&RGB_TIM, RGB_TIM_CHANNEL);
}
