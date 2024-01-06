//
// Created by fazhehy on 2023/12/31.
//

#ifndef SNAKE_H
#define SNAKE_H

#include "common_inc.h"
#include "color.h"

#define UP      1
#define DOWN    2
#define RIGHT   3
#define LEFT    4
#define PLAY    5
#define WIN     6
#define LOSE    7

enum mode_t{
    normal,
    no_wall,
    no_enemy
};

typedef struct {
    int8_t x;
    int8_t y;
    hsv_t hsv;
}body_t;

typedef struct {
    body_t body[100];
    body_t head;
    body_t food;
    uint8_t len;
    uint8_t max_len;
    uint8_t last_len;
    uint8_t move_dir;
    uint16_t refresh_time;
    mode_t mode;
    uint8_t flag;
}snake_t;

extern snake_t snake;

void snake_init();
void snake_task();
void snake_show();
void snake_show_callback();

#endif //SNAKE_H
