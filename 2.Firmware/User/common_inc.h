//
// Created by fazhehy on 2023/12/30.
//

#ifndef COMMON_INC_H
#define COMMON_INC_H

#include <stdbool.h>

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"

#include "retarget.h"
#include "rgb.h"
#include "task.h"
#include "snake.h"
#include "oled.h"
#include "key.h"
#include "menu.h"
#include "flash.h"
#include "music.h"

void Main();

#endif //COMMON_INC_H
