//
// Created by fazhehy on 2023/8/4.
//

#ifndef NUEDC_FLASH_H
#define NUEDC_FLASH_H

#include "common_inc.h"
#include <stdbool.h>

#define SNAKE_REFRESH_TIME_INDEX    0
#define SNAKE_MODE_INDEX            1
#define SNAKE_MAX_LEN_INDEX         2
#define SNAKE_LAST_LEN_INDEX        3

#define TASK_SHOW_TIME_INDEX        4
#define TASK_TASK_TIME1_INDEX       5
#define TASK_TASK_TIME2_INDEX       6

//void flash_clear_addr();
bool flash_write(const uint16_t *data, uint8_t n);
void flash_read(uint16_t * data, uint8_t n);
void flash_unlock();
void flash_lock();
void flash_erase();

#endif //NUEDC_FLASH_H