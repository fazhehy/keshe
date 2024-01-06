//
// Created by fazhehy on 2023/12/28.
//

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb888_t;

typedef struct{
    uint8_t h;
    uint8_t s;
    uint8_t v;
}hsv_t;

void rgb_to_hsv(rgb888_t rgb888, hsv_t hsv);
void hsv_to_rgb888(hsv_t hsv, rgb888_t * rgb888);

#endif //COLOR_H
