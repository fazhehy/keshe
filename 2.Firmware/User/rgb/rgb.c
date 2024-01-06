//
// Created by fazhehy on 2023/12/28.
//

#include "rgb.h"

rgb888_t rgbBuffer[RGB_COL_NUM][RGB_ROW_NUM];
uint16_t rgbCodeBuffer[RGB_NUM*3*8+6] = {0};

void rgb_buffer_conversion()
{
    uint16_t rgbCodeBufferIndex = 6;
    uint8_t color = 0;
    for (int i = 0; i < RGB_COL_NUM; ++i) {
        for (int j = 0; j < RGB_ROW_NUM; ++j) {
            color = rgbBuffer[i][j].g;
            for (int k = 0; k < 8; ++k) {
                if (((color<<k) & (0x80)) == 0x80)
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 59;
                else
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 29;
            }
            color = rgbBuffer[i][j].r;
            for (int k = 0; k < 8; ++k) {
                if (((color<<k) & (0x80)) == 0x80)
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 59;
                else
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 29;
            }
            color = rgbBuffer[i][j].b;
            for (int k = 0; k < 8; ++k) {
                if (((color<<k) & (0x80)) == 0x80)
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 59;
                else
                    rgbCodeBuffer[rgbCodeBufferIndex++] = 29;
            }
        }
    }
}

void rgb_update()
{
    rgb_buffer_conversion();
    ws2812b_send_bytes((uint32_t *)rgbCodeBuffer, RGB_NUM*3*8+6);
}

void rgb_draw_rgb_point(uint8_t x, uint8_t y, rgb888_t rgb888)
{
    rgbBuffer[y][x].r = rgb888.r;
    rgbBuffer[y][x].b = rgb888.b;
    rgbBuffer[y][x].g = rgb888.g;
}

void rgb_draw_hsv_point(uint8_t x, uint8_t y, hsv_t hsv)
{
    rgb888_t rgb888;
    hsv_to_rgb888(hsv, &rgb888);
    rgbBuffer[y][x].r = rgb888.r;
    rgbBuffer[y][x].b = rgb888.b;
    rgbBuffer[y][x].g = rgb888.g;
}

void rgb_rgb_fill(rgb888_t rgb888)
{
    for (int i = 0; i < RGB_COL_NUM; ++i) {
        for (int j = 0; j < RGB_ROW_NUM; ++j) {
            rgbBuffer[i][j].r = rgb888.r;
            rgbBuffer[i][j].b = rgb888.b;
            rgbBuffer[i][j].g = rgb888.g;
        }
    }
}

void rgb_hsv_fill(hsv_t hsv)
{
    rgb888_t rgb888;
    hsv_to_rgb888(hsv, &rgb888);
    for (int i = 0; i < RGB_COL_NUM; ++i) {
        for (int j = 0; j < RGB_ROW_NUM; ++j) {
            rgbBuffer[i][j].r = rgb888.r;
            rgbBuffer[i][j].b = rgb888.b;
            rgbBuffer[i][j].g = rgb888.g;
        }
    }
}

void rgb_clear_buffer()
{
    rgb888_t rgb888 = {0};
    rgb_rgb_fill(rgb888);
}

void rgb_test()
{
    rgb888_t rgb888;
    rgb888.r = 0xff;
    rgb888.g = 0;
    rgb888.b = 0;
    rgb_rgb_fill(rgb888);
    rgb_update();
    HAL_Delay(100);
    rgb888.r = 0;
    rgb888.g = 0xff;
    rgb888.b = 0;
    rgb_rgb_fill(rgb888);
    rgb_update();
    HAL_Delay(100);
    rgb888.r = 0;
    rgb888.g = 0;
    rgb888.b = 0xff;
    rgb_rgb_fill(rgb888);
    rgb_update();
    HAL_Delay(100);
}

void rgb_show_char(uint8_t x, uint8_t y, char ch, hsv_t hsv)
{
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((oled_font_6x8[ch-' '][i]>>j)&0x01){
                rgb_draw_hsv_point(x+i, y+j, hsv);
            }
        }
    }
}

uint8_t sentence[600][8] = {0};
void rgb_show_string1(char* str, hsv_t hsv, uint8_t times)
{
    for (int i = 0; i < 600; ++i) {
        for (int j = 0; j < 8; ++j) {
            sentence[i][j] = 0;
        }
    }
    uint8_t len = strlen(str);
    uint8_t n = 0;
    uint8_t bufferLen = len*6;
    while (n != len){
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 8; ++j) {
                if ((oled_font_6x8[str[n]-' '][i]>>j)&0x01){
                    sentence[n*6+i][j] = 1;
                }
            }
        }
        n ++;
    }
    n = 0;
    uint8_t times_ = 0;
    while (times){
        rgb_clear_buffer();
        for (int i = 0; i < 10; ++i) {
            uint16_t x = n+i;
            if (x > bufferLen){
                if (times == 1)
                    break;
                x -= bufferLen;
            }
            for (int j = 0; j < 8; ++j) {
                if (sentence[x][j] == 1)
                    rgb_draw_hsv_point(i, 1+j, hsv);
            }
        }
        n++;
        if (n >= bufferLen){
            n = 0;
            times --;
        }
        rgb_update();
        HAL_Delay(150);
    }
    rgb_clear_buffer();
    rgb_update();
}

void rgb_draw_hsv_vLine(uint8_t x, uint8_t y, uint8_t len, hsv_t hsv)
{
    for (int i = 0; i < len; ++i) {
//        hsv.h -= 20;
//        if (hsv.h  == 0 || hsv.h >= 179)
//            hsv.h = 0;
        rgb_draw_hsv_point(x, y+i, hsv);

//        if (hsv.s >= 255)
//            hsv.s = 255;
    }
}
