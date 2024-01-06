//
// Created by fazhehy on 2023/8/4.
//

#include "flash.h"

static FLASH_EraseInitTypeDef EraseInitStruct = {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .PageAddress = 0x0801fc00,
        .NbPages = 1
};

uint32_t addr = 0x0801fc00;

//void flash_clear_addr()
//{
//    addr = 0x0801fc00;
//}

bool flash_write(const uint16_t *data, uint8_t n)
{
    if (n >= 255)
        return false;
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr+n*4, *(uint32_t*)data)==HAL_OK){

    }
    return true;
}

void flash_read(uint16_t * data, uint8_t n)
{
    *data = *(__IO uint16_t *)(addr+n*4);
}

void flash_erase()
{
    uint32_t PageError = 0;
    __disable_irq();                             //擦除前关闭中断
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK){
    }
    else{

        while(1);
    }
    __enable_irq();
}

void flash_unlock()
{
    HAL_FLASH_Unlock();
}

void flash_lock()
{
    HAL_FLASH_Lock();
}
