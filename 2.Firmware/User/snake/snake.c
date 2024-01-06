//
// Created by fazhehy on 2023/12/31.
//

#include "snake.h"
#include <stdlib.h>

snake_t snake;

void snake_show()
{
    rgb_clear_buffer();
    for (int i = snake.len-1; i >= 0; --i)
        rgb_draw_hsv_point(snake.body[i].x, snake.body[i].y, snake.body[i].hsv);
    rgb_draw_hsv_point(snake.food.x, snake.food.y, snake.food.hsv);
    rgb_update();
}

void snake_init()
{
    snake.len = 3;
    snake.head.x = 5;
    snake.head.y = 5;
    snake.head.hsv.h = 5;
    snake.head.hsv.v = 80;
    snake.head.hsv.s = 255;
    snake.move_dir = RIGHT;
    snake.food.hsv.h = 0;
    snake.food.hsv.s = 255;
    snake.food.hsv.v = 80;

    int8_t x = snake.head.x;
    uint8_t h = snake.head.hsv.h;
    for (int i = 0; i < snake.len; ++i) {
        snake.body[i].x = x--;
        snake.body[i].y = 5;
        snake.body[i].hsv.h = h;
        snake.body[i].hsv.s = 255;
        snake.body[i].hsv.v = 80;
        h += 10;
    }
    snake.body[0].hsv.h = 0;
    snake.body[0].hsv.s = 0;
    snake.body[0].hsv.v = 200;
    snake.flag = PLAY;
}

void snake_get_dir(uint8_t keyNum)
{
    if (keyNum == 0)
        return;

    if (keyNum == 1){
        if (snake.move_dir == LEFT || snake.move_dir == RIGHT)
            snake.move_dir = UP;
    }
    else if (keyNum == 2){
        if (snake.move_dir == LEFT || snake.move_dir == RIGHT)
            snake.move_dir = DOWN;
    }
    else if (keyNum == 3){
        if (snake.move_dir == UP || snake.move_dir == DOWN)
            snake.move_dir = LEFT;
    }
    else if (keyNum == 4){
        if (snake.move_dir == UP || snake.move_dir == DOWN)
            snake.move_dir = RIGHT;
    }
}

void snake_head_update()
{
    switch (snake.move_dir) {
        case UP: snake.head.y --; break;
        case DOWN: snake.head.y ++; break;
        case LEFT: snake.head.x --; break;
        case RIGHT: snake.head.x ++; break;
    }
}

void snake_die()
{
    if (snake.mode == no_wall || snake.mode == no_enemy){
        if (snake.head.x == 10)
            snake.head.x = 0;
        else if (snake.head.x == -1)
            snake.head.x = 9;
        if (snake.head.y == 10)
            snake.head.y = 0;
        else if (snake.head.y == -1)
            snake.head.y = 9;
    }
    else{
        if (snake.head.x == 10 || snake.head.x == -1 || snake.head.y == 10 || snake.head.y == -1)
        {
            snake.flag = LOSE;
            return;
        }
    }

    if (snake.mode == no_enemy)
        return;
    for (int i = 1; i < snake.len; ++i) {
        if (snake.head.x == snake.body[i].x && snake.body[i].y == snake.head.y){
            snake.flag = LOSE;
            return;
        }
    }
}

void snake_body_update()
{
    for (int i = snake.len-1; i > 0; --i){
        snake.body[i].x = snake.body[i-1].x;
        snake.body[i].y = snake.body[i-1].y;
    }
    snake.body[0].x = snake.head.x;
    snake.body[0].y = snake.head.y;
}

void snake_food_generate()
{
//    static hsv_t hsv = {.h = 0, .s = 255, .v = 80};
    uint8_t block_ken = 100 - snake.len;
    bool isBody;
    srand(HAL_GetTick());
    uint8_t food_pos_num = (uint8_t)rand()%block_ken;
    uint8_t food_pos = 0;
    for (int8_t i = 0; i < 10; ++i) {
        for (int8_t j = 0; j < 10; ++j) {
            isBody = false;
            for (int k = 0; k < snake.len; ++k) {
                if (i == snake.body[k].y && j == snake.body[k].x){
                    isBody = true;
                    break;
                }
            }
            if (!isBody){
                food_pos ++;
                if (food_pos == food_pos_num){
                    snake.food.x = j;
                    snake.food.y = i;
                }
            }
        }
    }
}

void snake_eat()
{
    if (snake.max_len > 100 || snake.max_len == 0)
        snake.max_len = 3;
    if (snake.max_len < snake.len){
        snake.max_len = snake.len;
    }
    if (snake.head.x == snake.food.x && snake.head.y == snake.food.y){
//        snake_body_update();
        snake.len ++;
        if (snake.len == 100){
            snake.flag = WIN;
            return;
        }
        snake.body[snake.len-1].hsv.h = snake.body[snake.len-2].hsv.h + 20;
        if (snake.body[snake.len-1].hsv.h > 179)
            snake.body[snake.len-1].hsv.h -= 179;
        snake.body[snake.len-1].hsv.s = snake.body[snake.len-2].hsv.s;
        snake.body[snake.len-1].hsv.v = snake.body[snake.len-2].hsv.v;

        snake_food_generate();
    }
}

void snake_die_animation()
{
    hsv_t hsv = {.h = 0, .v = 200, .s = 255};
    rgb_show_string1("loser!!! ", hsv, 1);
}

void snake_win_animation()
{
    hsv_t hsv = {.h = 60, .v = 200, .s = 255};
    rgb_show_string1("you win!!! ", hsv, 1);
}

void snake_oled_show()
{
    oled_clear_buffer();
    oled_show_string(0, 0, "direction:");
    oled_show_string(92, 0, "right");
    oled_show_string(0, 12, "current length:");
    oled_show_num(92, 12, snake.len, 3);
    oled_show_string(0, 24, "max length:");
    oled_show_num(92, 24, snake.max_len, 3);
    oled_show_string(0, 36, "last length:");
    oled_show_num(92, 36, snake.last_len, 3);
    oled_show_string(0, 48, "current mode:");
    switch (snake.mode) {
        case no_wall: oled_show_string(80, 48, "no wall");break;
        case normal: oled_show_string(80, 48, "normal");break;
        case no_enemy: oled_show_string(80, 48, "no enemy");break;
    }
    oled_update_screen();
}

void snake_task()
{
//    snake_init();
    HAL_TIM_Base_Start_IT(&htim6);
    snake_food_generate();
    snake_oled_show();
    for (;;)
    {
        uint8_t keyNum = get_key_num();
        if (keyNum == 8)
            break;
        if (snake.flag == LOSE){
            snake_die_animation();
            break;
//            snake_init();
//            snake_food_generate();
        }
        else if (snake.flag == WIN){
            snake_win_animation();
            break;
//            snake_init();
//            snake_food_generate();
        }
        snake_get_dir(keyNum);
        switch (snake.move_dir) {
            case UP:oled_show_string(92, 0, "up    ");break;
            case DOWN:oled_show_string(92, 0, "down ");break;
            case RIGHT:oled_show_string(92, 0, "right");break;
            case LEFT:oled_show_string(92, 0, "left ");break;
        }
        switch (snake.mode) {
            case no_wall: oled_show_string(80, 48, "no wall");break;
            case normal: oled_show_string(80, 48, "normal");break;
            case no_enemy: oled_show_string(80, 48, "no enemy");break;
        }
        oled_show_num(92, 12, snake.len, 3);
        oled_show_num(92, 24, snake.max_len, 3);
        oled_show_num(92, 36, snake.last_len, 3);
        oled_update_screen();
    }
    snake.last_len = snake.len;
    rgb_clear_buffer();
    rgb_update();
    HAL_TIM_Base_Stop_IT(&htim6);
    oled_clear_buffer();
}

void snake_show_callback()
{
    static uint32_t tick = 0;
    tick ++;
    if (tick == snake.refresh_time){
        tick = 0;
        snake_head_update();
        snake_die();
        snake_eat();
        snake_body_update();
        if (snake.flag == PLAY){
            snake_show();
        }
    }
}

