//
// Created by fazhehy on 2023/12/30.
//

#include "common_inc.h"

uint8_t oled_fall_pot[127] = {0};
uint8_t rgb_fall_pot[10] = {0};
uint8_t times = 0;
uint8_t music_flag = 0;

void Main()
{
    RetargetInit(&huart1);
    oled_init();
//    music_init();
    menu_init();
//    hsv_t hsv = {.h = 51, .s = 255, .v = 80};
//    rgb_draw_hsv_vLine(0, 0, 5, hsv);
//    rgb_update();
//    rgb_loop_task(0);
    for (;;)
    {

        menu_task();
//        music_task(0);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == hadc1.Instance)
    {
        music_callback();
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == RGB_TIM.Instance)
    {
        ws2812b_callback();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance)
    {
//        printf("move\n");
        snake_show_callback();
    }
}
