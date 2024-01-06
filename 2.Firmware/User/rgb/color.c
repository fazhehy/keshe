//
// Created by fazhehy on 2023/12/28.
//

#include "color.h"

float max_color(float a, float b, float c)
{
    float color = 0;
    color = a;
    if (color < b)
        color = b;
    if (color < c)
        color = c;

    return color;
}

float min_color(float a, float b, float c)
{
    float color = 0;
    color = a;
    if (color > b)
        color = b;
    if (color > c)
        color = c;

    return color;
}

void rgb_to_hsv(rgb888_t rgb888, hsv_t hsv)
{
    float h = 0, s, v;

    float r = (float )rgb888.r/255;
    float g = (float )rgb888.g/255;
    float b = (float )rgb888.b/255;

    float max = max_color(r, g, b);
    float min = min_color(r, g, b);

    v = max;

    if (max == 0)
        s = 0;
    else
        s = 1-(min/max);

    if (max == min)
        h = 0;
    else if (max == r && g>=b)
        h = 60*((g-b)/(max-min));
    else if (max == r && g < b)
        h = 60*((g-b)/(max-min)) + 360;
    else if (max == g)
        h = 60*((b-r)/(max-min)) + 120;
    else if (max == b)
        h = 60*((r-g)/(max-min)) + 240;

    hsv.h = (uint8_t )(h/2);
    hsv.s = (uint8_t )(s*255);
    hsv.v = (uint8_t )(v*255);
}

void hsv_to_rgb888(hsv_t hsv, rgb888_t * rgb888)
{
    float c, x, y, z;
    int i = 0;
    float h = (float )hsv.h*2;
    float s = (float )hsv.s/255;
    float v = (float )hsv.v/255;
    float r = 0, g = 0, b = 0;
    if (s == 0)
        r = g = b = v;
    else
    {
        h = h/60;
        i = (int )h;
        c = h-(float )i;

        x = v*(1-s);
        y = v*(1-s*c);
        z = v*(1-s*(1-c));
        switch (i) {
            case 0: r = v; g = z; b = x; break;
            case 1: r = y; g = v; b = x; break;
            case 2: r = x; g = v; b = z; break;
            case 3: r = x; g = y; b = v; break;
            case 4: r = z; g = x; b = v; break;
            case 5: r = v; g = x; b = y; break;
            default:
                break;
        }
    }

    rgb888->r = (uint8_t)(r*255);
    rgb888->g = (uint8_t)(g*255);
    rgb888->b = (uint8_t)(b*255);
}
