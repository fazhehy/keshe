//
// Created by fazhehy on 2023/12/26.
//

#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "common_inc.h"

#define MAX_PAGE_NUM            100
#define MAX_SHOW_NUM            5

typedef struct Page * xpPage;
typedef struct Page
{
    char * name;
    uint8_t (*func)(uint8_t);
    struct {
        uint8_t frameX;
        uint8_t frameY;
        uint8_t framePos;
        uint8_t frameLen;
        xpPage showStartPage;
    }show;
    xpPage lastPage;
    xpPage nextPage;
    xpPage selectSubPage;
    xpPage subPageHead;
    xpPage subPageTail;
    xpPage parentPage;
}xPage;

void menu_init();
void menu_task();

#endif //MENU_H
