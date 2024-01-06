//
// Created by fazhehy on 2023/12/28.
//

#ifndef RGB_H
#define RGB_H

#include "common_inc.h"
#include "ws2812b.h"
#include "color.h"
#include <string.h>
#include <stdlib.h>

#define RGB_COL_NUM     (10)
#define RGB_ROW_NUM     (10)
#define RGB_NUM         ((RGB_COL_NUM)*(RGB_COL_NUM))

void rgb_update();
void rgb_draw_rgb_point(uint8_t x, uint8_t y, rgb888_t rgb888);
void rgb_draw_hsv_point(uint8_t x, uint8_t y, hsv_t hsv);
void rgb_rgb_fill(rgb888_t rgb888);
void rgb_hsv_fill(hsv_t hsv);
void rgb_clear_buffer();
void rgb_test();
void rgb_show_char(uint8_t x, uint8_t y, char ch, hsv_t hsv);
void rgb_show_string1(char* str, hsv_t hsv, uint8_t times);
void rgb_draw_hsv_vLine(uint8_t x, uint8_t y, uint8_t len, hsv_t hsv);

#endif //RGB_H
