//
// Created by fazhehy on 2023/12/28.
//

#ifndef WS2812B_H
#define WS2812B_H

#include "common_inc.h"

#define RGB_TIM             htim2
#define RGB_TIM_CHANNEL     TIM_CHANNEL_1

void ws2812b_send_bytes(uint32_t * data, uint16_t len);
void ws2812b_callback();

#endif //WS2812B_H
