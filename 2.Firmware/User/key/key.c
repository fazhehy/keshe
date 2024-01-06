//
// Created by fazhehy on 2023/12/24.
//

#include "key.h"

uint8_t get_key_num()
{
    bool isLongPress = false;
    uint32_t tick;
    if (KEY1_VALUE() == 0)
    {
        HAL_Delay(20);
        tick = HAL_GetTick();
        while (KEY1_VALUE() == 0){
            if (HAL_GetTick() >= tick + 500){
                isLongPress = true;
            }
        }
        HAL_Delay(20);
        if (!isLongPress)
            return 1;
        else
            return 5;
    }

    if (KEY2_VALUE() == 0)
    {
        HAL_Delay(20);
        tick = HAL_GetTick();
        while (KEY2_VALUE() == 0){
            if (HAL_GetTick() >= tick + 500){
                isLongPress = true;
            }
        }
        HAL_Delay(20);
        if (!isLongPress)
            return 2;
        else
            return 6;

    }

    if (KEY3_VALUE() == 0)
    {
        HAL_Delay(20);
        tick = HAL_GetTick();
        while (KEY3_VALUE() == 0){
            if (HAL_GetTick() >= tick + 500){
                isLongPress = true;
            }
        }
        HAL_Delay(20);
        if (!isLongPress)
            return 3;
        else
            return 7;
    }

    if (KEY4_VALUE() == 0)
    {
        HAL_Delay(20);
        tick = HAL_GetTick();
        while (KEY4_VALUE() == 0){
            if (HAL_GetTick() >= tick + 500){
                isLongPress = true;
            }
        }
        HAL_Delay(20);
        if (!isLongPress)
            return 4;
        else
            return 8;
    }

    return 0;
}
