//
// Created by fazhehy on 2023/12/26.
//

#include "menu.h"
#include "oled.h"
#include "key.h"

xPage pageBuffer[MAX_PAGE_NUM];
uint8_t pageBufferPointer = 0;

xpPage add_page(char * name, xpPage parentPage, uint8_t(*func)(uint8_t))
{
    if (pageBufferPointer >= MAX_PAGE_NUM)
        return NULL;
    xpPage subPage = pageBuffer+pageBufferPointer;
    pageBufferPointer ++;

    subPage->name = name;
    subPage->parentPage = parentPage;
    subPage->func = func;

    if (parentPage == NULL)
    {
        subPage->lastPage = NULL;
        subPage->nextPage = NULL;
    }
    else
    {
        if (parentPage->subPageHead == NULL)
        {
            subPage->lastPage = NULL;
            parentPage->subPageHead = subPage;
            parentPage->subPageTail = subPage;
            parentPage->selectSubPage = subPage;
            parentPage->show.showStartPage = subPage;
        }
        else
        {
            subPage->lastPage = parentPage->subPageTail;
            parentPage->subPageTail->nextPage = subPage;
            parentPage->subPageTail = subPage;
        }
    }
    return subPage;
}

xpPage nowPage;
uint8_t menu_move(uint8_t keyNum)
{
    oled_clear_buffer();
    if (nowPage->func != NULL)
        keyNum = nowPage->func(keyNum);

    uint8_t moveDir = 0;
    if (keyNum == 2)
    {
        if (nowPage->selectSubPage->nextPage != NULL)
        {
            nowPage->selectSubPage = nowPage->selectSubPage->nextPage;
            moveDir = 1;
        }
    }
    else if (keyNum == 1)
    {
        if (nowPage->selectSubPage->lastPage != NULL)
        {
            nowPage->selectSubPage = nowPage->selectSubPage->lastPage;
            moveDir = 2;
        }
    }
    else if (keyNum == 3)
    {
        if (nowPage->selectSubPage->subPageHead != NULL || nowPage->selectSubPage->func != NULL)
        {
            nowPage = nowPage->selectSubPage;
        }
    }
    else if (keyNum == 4)
    {
        if (nowPage->parentPage != NULL)
        {
            nowPage = nowPage->parentPage;
        }
    }
    else if (keyNum == 0xff)
    {
        moveDir = 3;
    }

    return moveDir;
}

void menu_show(uint8_t moveDir)
{
    if (moveDir != 3)
    {
        oled_clear_buffer();
        if (moveDir == 1)
        {
            if (nowPage->show.framePos >= MAX_SHOW_NUM-1)
            {
                xpPage pPage = nowPage->show.showStartPage;
                uint8_t i = 0;
                while (pPage != NULL)
                {
                    pPage = pPage->nextPage;
                    i ++;
                }
                if (i >= MAX_SHOW_NUM-1)
                    nowPage->show.showStartPage = nowPage->show.showStartPage->nextPage;
            }
            else
            {
                nowPage->show.frameX = 0;
                nowPage->show.frameY += 12;
                nowPage->show.framePos ++;
            }
        }
        else if (moveDir == 2)
        {
            if (nowPage->show.framePos == 0)
            {
                if (nowPage->show.showStartPage->lastPage != NULL)
                    nowPage->show.showStartPage = nowPage->show.showStartPage->lastPage;
            }
            else
            {
                nowPage->show.frameX = 0;
                nowPage->show.frameY -= 12;
                nowPage->show.framePos --;
            }
        }
        nowPage->show.frameLen = strlen(nowPage->selectSubPage->name)*6+4;
        xpPage showSubPage = nowPage->show.showStartPage;
        oled_show_string(0, 0, nowPage->name);
        uint8_t i = 1;
        while (i < MAX_SHOW_NUM+1)
        {
            oled_show_string(1, i*12, showSubPage->name);
            if (showSubPage->nextPage == NULL)
                break;
            showSubPage = showSubPage->nextPage;
            i ++;
        }
        if (nowPage->show.frameY < 0)
            nowPage->show.frameY = 0;
        oled_draw_fill_round_rect_xor(nowPage->show.frameX, nowPage->show.frameY-2, nowPage->show.frameLen, 12, 3);
        oled_update_screen();
    }
}

uint8_t frameX = 0, frameY = 0, frameLen;
void menu_show_animation(uint8_t moveDir)
{
    static uint8_t animation_flag = 0;
    static int8_t step_flag = 0, step;
    if (moveDir != 3)
    {
        if (moveDir == 1)
        {
            if (nowPage->show.framePos >= MAX_SHOW_NUM-1)
            {
                xpPage pPage = nowPage->show.showStartPage;
                uint8_t i = 0;
                while (pPage != NULL)
                {
                    pPage = pPage->nextPage;
                    i ++;
                }
                if (i >= MAX_SHOW_NUM-1)
                    nowPage->show.showStartPage = nowPage->show.showStartPage->nextPage;
            }
            else
            {
                nowPage->show.frameX = 0;
                nowPage->show.frameY += 12;
                nowPage->show.framePos ++;
            }
        }
        else if (moveDir == 2)
        {
            if (nowPage->show.framePos == 0)
            {
                if (nowPage->show.showStartPage->lastPage != NULL)
                    nowPage->show.showStartPage = nowPage->show.showStartPage->lastPage;
            }
            else
            {
                nowPage->show.frameX = 0;
                nowPage->show.frameY -= 12;
                nowPage->show.framePos --;
            }
        }
        if (nowPage->selectSubPage != NULL)
        {
            nowPage->show.frameLen = strlen(nowPage->selectSubPage->name)*6+4;
            if (step_flag == 0){
                step = (int8_t)((nowPage->show.frameLen-frameLen)/12);
                step_flag = 1;
            }
            xpPage showSubPage = nowPage->show.showStartPage;
//        oled_show_string(0, 0, nowPage->name);
            uint8_t i = 0;
            while (i < MAX_SHOW_NUM+1)
            {
                oled_show_string(2, i*12+1, showSubPage->name);
                if (showSubPage->nextPage == NULL)
                    break;
                showSubPage = showSubPage->nextPage;
                i ++;
            }
            if (nowPage->show.frameY < 2)
                nowPage->show.frameY = 2;
            if (frameY < nowPage->show.frameY)
            {
                animation_flag = 1;
            }
            if (frameY > nowPage->show.frameY)
            {
                animation_flag = 2;
            }
            if (animation_flag == 1)
            {
                if (frameY != nowPage->show.frameY)
                    frameY += 1;
                else
                    animation_flag = 0;
            }
            else if (animation_flag == 2)
            {
                if (frameY != nowPage->show.frameY)
                    frameY -= 1;
                else
                    animation_flag = 0;
            }
            if (animation_flag != 0)
                frameLen += step;
            else{
                frameLen = nowPage->show.frameLen;
                step_flag = 0;
            }
            oled_draw_fill_round_rect_xor(frameX, frameY-2, frameLen+2, 12, 3);
            oled_update_screen();
            HAL_Delay(1);
        }
    }
}

void menu_task()
{
    uint8_t keyNum = get_key_num();
    uint8_t moveDir = menu_move(keyNum);
    menu_show_animation(moveDir);
}

void menu_init()
{
    xpPage mainPage = add_page("Main Page", NULL, NULL);
    xpPage taskPage = add_page("task", mainPage, task_param_show);
    add_page("task1", taskPage, task1);
    add_page("task2", taskPage, task2);
    add_page("set time", taskPage, set_time_task);
    add_page("rgb loop", mainPage, rgb_loop_task);
    add_page("snake", mainPage, menu_snake_task);
    add_page("music", mainPage, music_task);
    add_page("warm", mainPage, warm_task);
    nowPage = mainPage;
    frameLen = strlen(nowPage->selectSubPage->name)*6+4;
}
