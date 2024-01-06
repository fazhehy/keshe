//
// Created by fazhehy on 2023/12/24.
//

#ifndef KEY_H
#define KEY_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define KEY1_VALUE()    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define KEY2_VALUE()    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)
#define KEY3_VALUE()    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
#define KEY4_VALUE()    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)

uint8_t get_key_num();

#endif //KEY_H
