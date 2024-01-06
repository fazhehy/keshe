//
// Created by fazhehy on 2023/12/31.
//

#include "task.h"

uint16_t show_time = 0;
int8_t task_time_part1 = 0;
int8_t task_time_part2 = 0;
void task_flash_read()
{
    flash_unlock();
    uint16_t temp;
    flash_read(&temp, SNAKE_REFRESH_TIME_INDEX);
    snake.refresh_time = temp;
    flash_read(&temp, SNAKE_MAX_LEN_INDEX);
    snake.max_len = temp;
    flash_read(&temp, SNAKE_LAST_LEN_INDEX);
    snake.last_len = temp;
    flash_read(&temp, SNAKE_MODE_INDEX);
    snake.mode = temp;
    flash_read(&temp, TASK_SHOW_TIME_INDEX);
    show_time = temp;
    flash_read(&temp, TASK_TASK_TIME1_INDEX);
    task_time_part1 = (int8_t)temp;
    flash_read(&temp, TASK_TASK_TIME2_INDEX);
    task_time_part2 = (int8_t)temp;
    flash_lock();
}

void task_flash_write()
{
    flash_unlock();
    flash_erase();
    uint16_t temp;
    temp = snake.refresh_time;
    flash_write(&temp, SNAKE_REFRESH_TIME_INDEX);
    temp = snake.mode;
    flash_write(&temp, SNAKE_MODE_INDEX);
    temp = snake.max_len;
    flash_write(&temp, SNAKE_MAX_LEN_INDEX);
    temp = snake.last_len;
    flash_write(&temp, SNAKE_LAST_LEN_INDEX);
    temp = show_time;
    flash_write(&temp, TASK_SHOW_TIME_INDEX);
    temp = (uint16_t )task_time_part1;
    flash_write(&temp, TASK_TASK_TIME1_INDEX);
    temp = (uint16_t )task_time_part2;
    flash_write(&temp, TASK_TASK_TIME2_INDEX);
    flash_lock();
}

uint8_t task_param_show(uint8_t keyNum)
{
    static uint8_t times = 0;
    if (times == 0){
        task_flash_read();
        times = 1;
    }
    oled_show_string(2, 40, "show time:");
    oled_show_num(92, 40, show_time, 3);
    oled_show_string(2, 54, "task time:");
    oled_show_num(92, 54, task_time_part1, 1);
    oled_show_char(98, 54, '.');
    oled_show_num(104, 54, task_time_part2, 1);
    oled_show_char(110, 54, 's');
//    oled_update_screen();
    if (keyNum == 4)
        times = 0;
    return keyNum;
}

uint8_t task1(uint8_t keyNum)
{
    oled_clear_buffer();
    oled_show_string(0, 0, "task1 is running");
    oled_update_screen();
    hsv_t hsv = {.h = 60, .s = 255, .v = 0};
    int8_t x = 0;
    while (1) {
        for (int i = 0; i <= 9; ++i) {
            for (int j = 0; j <= x; ++j) {
                rgb_draw_hsv_point(j, i, hsv);
            }
        }
        rgb_update();
        HAL_Delay(show_time);
        x++;
        if (x == 10) {
            break;
        }
        hsv.v += 20;
    }
    x = 9;
    while (1){
        for (int i = 9; i >= 0; --i) {
            for (int j = 9; j >= x; --j) {
                rgb_draw_hsv_point(j, i, hsv);
            }
        }
        rgb_update();
        HAL_Delay(show_time);
        x --;
        if (x == -1){
            break;
        }
        if (hsv.v != 0)
            hsv.v -= 20;
    }
    x = 9;
    while (1){
        for (int i = 0; i <= 9; ++i) {
            for (int j = 9; j >= x; --j) {
                rgb_draw_hsv_point(j, i, hsv);
            }
        }
        rgb_update();
        HAL_Delay(show_time);
        x--;
        if (x == -1) {
            break;
        }
        hsv.v += 20;
    }
    x = 0;
    while (1){
        for (int i = 0; i <= 9; ++i) {
            for (int j = 0; j <= x; ++j) {
                rgb_draw_hsv_point(j, i, hsv);
            }
        }
        rgb_update();
        HAL_Delay(show_time);
        x++;
        if (x == 10) {
            break;
        }
        if (hsv.v != 0)
            hsv.v -= 20;
    }
    while (1){
        hsv.v = 200;
        rgb_hsv_fill(hsv);
        rgb_update();
        HAL_Delay(show_time*5);
        hsv.v = 0;
        rgb_hsv_fill(hsv);
        rgb_update();
        HAL_Delay(show_time*5);
        break;
    }
    rgb_clear_buffer();
    rgb_update();
    return 4;
}

uint8_t task2(uint8_t keyNum)
{
    oled_clear_buffer();
    oled_show_string(0, 0, "task2 is running");
    oled_update_screen();
    hsv_t hsv = {.h = 60, .s = 255, .v = 0};
    int8_t x = 0;
    uint32_t task_time = task_time_part1*1000+task_time_part2*100;
    bool exit_flag = false, next_flag = false;
    uint32_t time;
    while (1){
        x = 0;
        while (1) {
            for (int i = 0; i <= 9; ++i) {
                for (int j = 0; j <= x; ++j) {
                    rgb_draw_hsv_point(j, i, hsv);
                }
            }
            rgb_update();
            x++;
            if (x == 10) {
                break;
            }
            hsv.v += 20;
            time = show_time-40;
            while (time --){
                keyNum = get_key_num();
                if (keyNum == 8){
                    rgb_clear_buffer();
                    rgb_update();
                    return 4;
                }
                else if (keyNum != 0){
                    next_flag = true;
                    break;
                }
                HAL_Delay(1);
            }
            if (next_flag)
                break;
        }
        if (next_flag)
            break;
        x = 9;
        while (1){
            for (int i = 9; i >= 0; --i) {
                for (int j = 9; j >= x; --j) {
                    rgb_draw_hsv_point(j, i, hsv);
                }
            }
            rgb_update();
            x --;
            if (x == -1){
                break;
            }
            if (hsv.v != 0)
                hsv.v -= 20;
            time = show_time-40;
            while (time --){
                keyNum = get_key_num();
                if (keyNum == 8){
                    rgb_clear_buffer();
                    rgb_update();
                    return 4;
                }
                else if (keyNum != 0){
                    next_flag = true;
                    break;
                }
                HAL_Delay(1);
            }
            if (next_flag)
                break;
        }
        if (next_flag)
            break;
        time = task_time-40;
        while (time --){
            keyNum = get_key_num();
            if (keyNum == 8){
                rgb_clear_buffer();
                rgb_update();
                return 4;
            }
            else if (keyNum != 0){
                next_flag = true;
                break;
            }
            HAL_Delay(1);
        }
        if (next_flag)
            break;
    }
    next_flag = false;
    rgb_clear_buffer();
    rgb_update();
    HAL_Delay(show_time);
    hsv.v = 0;
    while (1){
        x = 9;
        while (1){
            for (int i = 0; i <= 9; ++i) {
                for (int j = 9; j >= x; --j) {
                    rgb_draw_hsv_point(j, i, hsv);
                }
            }
            rgb_update();
            x--;
            if (x == -1) {
                break;
            }
            hsv.v += 20;
            time = show_time-40;
            while (time --){
                keyNum = get_key_num();
                if (keyNum == 8){
                    rgb_clear_buffer();
                    rgb_update();
                    return 4;
                }
                else if (keyNum != 0){
                    next_flag = true;
                    break;
                }
                HAL_Delay(1);
            }
            if (next_flag)
                break;
        }
        if (next_flag)
            break;
        x = 0;
        while (1){
            for (int i = 0; i <= 9; ++i) {
                for (int j = 0; j <= x; ++j) {
                    rgb_draw_hsv_point(j, i, hsv);
                }
            }
            rgb_update();
            x++;
            if (x == 10) {
                break;
            }
            if (hsv.v != 0)
                hsv.v -= 20;
            time = show_time-40;
            while (time --){
                keyNum = get_key_num();
                if (keyNum == 8){
                    rgb_clear_buffer();
                    rgb_update();
                    return 4;
                }
                else if (keyNum != 0){
                    next_flag = true;
                    break;
                }
                HAL_Delay(1);
            }
            if (next_flag)
                break;
        }
        if (next_flag)
            break;
        time = task_time-40;
        while (time --){
            keyNum = get_key_num();
            if (keyNum == 8){
                rgb_clear_buffer();
                rgb_update();
                return 4;
            }
            else if (keyNum != 0){
                next_flag = true;
                break;
            }
            HAL_Delay(1);
        }
        if (next_flag)
            break;
    }
    rgb_clear_buffer();
    rgb_update();
    HAL_Delay(show_time);
    while (1){
        hsv.v = 200;
        rgb_hsv_fill(hsv);
        rgb_update();
        HAL_Delay(show_time*5);
        hsv.v = 0;
        rgb_hsv_fill(hsv);
        rgb_update();
        time = task_time-40;
        while (time --){
            keyNum = get_key_num();
            if (keyNum != 0){
                rgb_clear_buffer();
                rgb_update();
                return 4;
            }
            HAL_Delay(1);
        }
    }
    rgb_clear_buffer();
    rgb_update();
    return 4;
}

uint8_t set_time_task(uint8_t keyNum)
{
    static uint8_t mode = 0;
    oled_show_string(2, 20, "show time:");
    oled_show_num(92, 20, show_time, 3);
    oled_show_string(2, 40, "task time:");
    oled_show_num(92, 40, task_time_part1, 1);
    oled_show_char(98, 40, '.');
    oled_show_num(104, 40, task_time_part2, 1);
    oled_show_char(110, 40, 's');
    if (keyNum == 1 || keyNum == 2){
        if (mode == 1){
            mode = 0;
        }
        else{
            mode = 1;
        }
    }
    if (mode == 0){
        if (keyNum == 3){
            show_time += 50;
            if (show_time >= 900)
                show_time = 900;
        }
        else if (keyNum == 4){
            show_time -= 50;
            if (show_time <= 50)
                show_time = 50;
        }
        oled_show_num(92, 20, show_time, 3);
        oled_draw_fill_round_rect_xor(90, 18, 3*6+6, 12, 3);
    }
    else{
        if (keyNum == 3){
            task_time_part2 += 1;
            if (task_time_part2 >= 10){
                task_time_part2 = 0;
                task_time_part1 ++;
                if (task_time_part1 >= 10)
                    task_time_part1 = 0;
            }
        }
        else if (keyNum == 4){
            task_time_part2 -= 1;
            if (task_time_part2 <= -1){
                task_time_part2 = 9;
                task_time_part1 --;
                if (task_time_part1 <= -1){}
                    task_time_part1 = 9;
            }
        }
        oled_show_num(92, 40, task_time_part1, 1);
        oled_show_num(104, 40, task_time_part2, 1);
        oled_draw_fill_round_rect_xor(90, 38, 4*6+6, 12, 3);
    }
    oled_update_screen();
    if (keyNum == 8){
        task_flash_write();
        return 4;
    }
    return 0xff;
}

uint8_t menu_snake_task(uint8_t keyNum)
{
    static int8_t mode = 0, times = 0;
    if (times == 0){
        task_flash_read();
        times = 1;
    }
    oled_show_string(2, 2, "start");
    oled_show_string(2, 16, "continue");
    oled_show_string(2, 30, "mode:");
    switch (snake.mode) {
        case normal:oled_show_string(74, 30, "normal");break;
        case no_wall:oled_show_string(74, 30, "no wall");break;
        case no_enemy:oled_show_string(74, 30, "no enemy");break;
    }
    oled_show_string(2, 44, "speed:");
    oled_show_num(92, 44, snake.refresh_time, 3);
    if (keyNum == 2){
        mode ++;
        if (mode == 4)
            mode = 0;
    }
    if (keyNum == 1){
        mode --;
        if (mode == -1)
            mode = 3;
    }
    if (mode == 0){
        oled_draw_fill_round_rect_xor(0, 0, 5*6+4, 12, 3);
        snake_init();
        if (keyNum == 3){
            snake_task();
        }
    }
    else if (mode == 1){
        oled_draw_fill_round_rect_xor(0, 14, 8*6+6, 12, 3);
        if (snake.flag == LOSE || snake.flag == WIN)
            snake_init();
        if (keyNum == 3){
            snake_task();
        }
    }
    else if (mode == 2){
        if (keyNum == 3){
            if (snake.mode == normal)
                snake.mode = no_wall;
            else if (snake.mode == no_wall)
                snake.mode = no_enemy;
            else if (snake.mode == no_enemy)
                snake.mode = normal;
        }
        switch (snake.mode) {
            case normal:oled_show_string(74, 30, "normal");break;
            case no_wall:oled_show_string(74, 30, "no wall");break;
            case no_enemy:oled_show_string(74, 30, "no enemy");break;
        }
        oled_draw_fill_round_rect_xor(72, 28, 8*6+6, 12, 3);
    }
    else if (mode == 3){
        if (keyNum == 3){
            snake.refresh_time+=50;
            if (snake.refresh_time >= 500)
                snake.refresh_time = 500;
        }
        else if (keyNum == 4){
            snake.refresh_time-=50;
            if (snake.refresh_time <= 50){
                snake.refresh_time = 50;
            }
        }
        oled_show_num(92, 44, snake.refresh_time, 3);
        oled_draw_fill_round_rect_xor(90, 42, 3*6+6, 12, 3);
    }
    oled_update_screen();
    if (keyNum == 8){
       task_flash_write();
        return 4;
    }
    return 0xff;
}

void show_body(body_t * body, uint8_t len)
{
    rgb_clear_buffer();
    for (int i = 0; i < len; ++i) {
        rgb_draw_hsv_point(body[i].x, body[i].y, body[i].hsv);
    }
    rgb_update();
}

uint8_t rgb_loop_task(uint8_t keyNum)
{
    oled_clear_buffer();
    oled_show_string(0, 0, "loop is running");
    oled_update_screen();
    uint8_t times = 0;
    static body_t body[5], head;
    uint8_t time = 0;
    uint8_t rgb_show_time = 20;
    if (times == 0){
        int8_t x = 0;
        int8_t y = 0;
        hsv_t hsv = {.h = 60, .s = 255, .v = 80};
        head.hsv.h = hsv.h;
        head.hsv.s = hsv.s;
        head.hsv.v = hsv.v;
        head.x = 5;
        head.y = 0;
        for (int i = 0; i < 5; ++i) {
            body[i].x = x++;
            body[i].y = 0;
            body[i].hsv.h = hsv.h;
            body[i].hsv.s = hsv.s;
            body[i].hsv.v = hsv.v;
            hsv.h += 20;
            if (hsv.h >= 179)
                hsv.h -= 179;
        }
        times = 1;
    }
    show_body(body, 5);
    HAL_Delay(5);
    uint8_t dir = RIGHT;
    while (1){
        switch (dir) {
            case RIGHT: head.x ++;break;
            case LEFT: head.x --;break;
            case UP: head.y ++;break;
            case DOWN: head.y --;break;
            default:break;
        }
        if (head.x == 10){
            dir = LEFT;
            head.x = 9;
            head.y ++;
            if (head.y == 10)
                head.y = 0;
        }
        if (head.x == -1){
            dir = RIGHT;
            head.y ++;
            if (head.y == 10){
                if (head.x == -1)
                    break;
                head.y = 0;
            }
            head.x = 0;
        }
        for (int i = 4; i > 0; --i) {
            body[i].x = body[i-1].x;
            body[i].y = body[i-1].y;
        }
        body[0].x = head.x;
        body[0].y = head.y;
        show_body(body, 5);
        time = rgb_show_time;
        while (time --){
            keyNum = get_key_num();
            if (keyNum != 0){
                rgb_clear_buffer();
                rgb_update();
                times = 0;
                return 4;
            }
            HAL_Delay(1);
        }
    }
    rgb_clear_buffer();
    rgb_update();
    int8_t y_max = 9, x_max = 9;
    int8_t y_min = 0, x_min = 0;
    int8_t x = 0, y = 0;
    uint8_t direction = RIGHT;
    hsv_t hsv = {.h = 0, .s = 255, .v = 80};
    while (1){
        rgb_draw_hsv_point(x, y, hsv);
        rgb_update();
        switch (direction) {
            case RIGHT:x ++;break;
            case LEFT:x --;break;
            case UP:y --;break;
            case DOWN:y ++;break;
            default:break;
        }
        hsv.h += 18;
        if (hsv.h > 179)
            hsv.h -= 179;
        if (x == x_max+1 && direction == RIGHT){
            x = x_max;
            y_min ++;
            direction = DOWN;
        }
        else if (y == y_max+1 && direction == DOWN){
            y = y_max;
            x_max --;
            direction = LEFT;
        }
        else if (x == x_min-1 && direction == LEFT){
            x = x_min;
            y_max --;
            direction = UP;
        }
        else if (y == y_min-1 && direction == UP){
            y = y_min;
            x_min ++;
            direction = RIGHT;
        }
        if (y_max < y_min && x_max < x_min)
        {
            break;
        }
        time = rgb_show_time;
        while (time --){
            keyNum = get_key_num();
            if (keyNum != 0){
                rgb_clear_buffer();
                rgb_update();
                times = 0;
                return 4;
            }
            HAL_Delay(1);
        }
    }
    time = rgb_show_time;
    while (time --){
        keyNum = get_key_num();
        if (keyNum != 0){
            rgb_clear_buffer();
            rgb_update();
            times = 0;
            return 4;
        }
        HAL_Delay(1);
    }
    hsv.v = 0;
    x = 4;
    y = 5;
    y_max = y_min = 5;
    x_max = x_min = 4;
    direction = RIGHT;
    while (1){
        rgb_draw_hsv_point(x, y, hsv);
        rgb_update();
        switch (direction) {
            case RIGHT:x ++;break;
            case LEFT:x --;break;
            case UP:y --;break;
            case DOWN:y ++;break;
            default:break;
        }
        if (x == x_max + 1 && direction == RIGHT){
            direction = UP;
            x_max ++;
        }
        else if (y == y_min - 1 && direction == UP){
            direction = LEFT;
            y_min --;
        }
        else if (x == x_min - 1 && direction == LEFT){
            direction = DOWN;
            x_min --;
        }
        else if (y == y_max + 1 && direction == DOWN){
            direction = RIGHT;
            y_max ++;
        }
        if (x == 0 && y == 0){
            rgb_clear_buffer();
//            rgb_draw_hsv_point(0, 0, hsv);
            rgb_update();
            break;
        }
//        if (y_max == 10 && y_min == -1 && x_max == 10 && x_min == -1)
//            break;
        time = rgb_show_time;
        while (time --){
            keyNum = get_key_num();
            if (keyNum != 0){
                rgb_clear_buffer();
                rgb_update();
                times = 0;
                return 4;
            }
            HAL_Delay(1);
        }
    }
    HAL_Delay(5);
    rgb_clear_buffer();
    rgb_update();
    while (time --){
        keyNum = get_key_num();
        if (keyNum != 0){
            rgb_clear_buffer();
            rgb_update();
            times = 0;
            return 4;
        }
        HAL_Delay(1);
    }
    hsv.h = 0;
    hsv.s = 255;
    hsv.v = 80;
    for (hsv.h = 0;  hsv.h< 180; hsv.h++) {
        rgb_hsv_fill(hsv);
        rgb_update();
        time = rgb_show_time;
        while (time --){
            keyNum = get_key_num();
            if (keyNum != 0){
                rgb_clear_buffer();
                rgb_update();
                times = 0;
                return 4;
            }
            HAL_Delay(1);
        }
    }
    hsv.h = 80;
    hsv.v = 80;
    hsv.s = 255;
    rgb_show_string1("Designed by fazhehy", hsv, 1);
    rgb_clear_buffer();
    rgb_update();
    if (keyNum == 4){
        times = 0;
        return 4;
    }
    return 4;
}

uint8_t warm_task(uint8_t keyNum)
{
    static uint8_t times = 0;
    oled_clear_buffer();
    oled_show_string(0, 0, "warm is running");
    oled_update_screen();
    rgb888_t rgb888;
    rgb888.r = rgb888.b = rgb888.g = 0xff;
//    hsv_t hsv;
//    hsv.h = 15;
//    hsv.s = 255;
//    hsv.v = 255;
//    rgb_hsv_fill(hsv);
    if (times == 0){
        rgb_rgb_fill(rgb888);
        rgb_update();
        times = 1;
    }

    if (keyNum == 4){
        rgb_clear_buffer();
        rgb_update();
        times = 0;
        return 4;
    }
    return 0xff;
}
