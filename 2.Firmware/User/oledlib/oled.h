//
// Created by fazhehy on 2023/8/25.
//

#ifndef OLED_H
#define OLED_H

#include "gpio.h"

#define OLED_W_SCL(X) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,(GPIO_PinState)(X))
#define OLED_W_SDA(X) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,(GPIO_PinState)(X))
#define OLED_WIDTH   128
#define OLED_HEIGHT  64

extern const uint8_t oled_font_6x8[][6];
extern const uint8_t oled_font_8x16[][16];

void oled_init();
void oled_clear();
void oled_update_screen();
void oled_clear_buffer();
void oled_show_bmp(uint8_t x, uint8_t y, const uint8_t *bmp, uint8_t w, uint8_t h);
void oled_show_char(uint8_t x, uint8_t y, char ch);
void oled_show_string(uint8_t x, uint8_t y, const char * str);
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void oled_draw_point(uint8_t x, uint8_t y);
void oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void oled_draw_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void oled_draw_vLine(uint8_t x, uint8_t y, uint8_t len);
void oled_draw_hLine(uint8_t x, uint8_t y, uint8_t len);
void oled_draw_circle(uint8_t x, uint8_t y, uint8_t r);
void oled_draw_fill_circle(uint8_t x, uint8_t y, uint8_t r);
void oled_draw_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void oled_draw_fill_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void oled_draw_fill_round_rect_xor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
uint8_t oled_printf(uint8_t x, uint8_t y, const char *format, ...);

#endif //OLED_H
