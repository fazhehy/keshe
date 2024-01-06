//
// Created by fazhehy on 2024/1/2.
//

#include "music.h"
#include "stm32_dsp.h"
#include "table_fft.h"

#define NPT 256
#define PI2 6.28318530717959
//采样率计算
//分辨率：Fs/NPT
//#define Fs	10000
#define Fs	9984
//取9984能出来整数的分辨率 9984/256 = 39Hz

uint32_t adcValue[NPT] = {0};
long lBufInArray[NPT];
long lBufOutArray[NPT/2];
long lBufMagArray[NPT/2];

void Creat_Single(void)
{
    float fx=0.0f;

    for(int i=0; i<NPT; i++)
    {
        fx = 2048*sin(PI2 * i * 300.0 / Fs)+
             3100*sin(PI2 * i * 800.0 / Fs)+
             1300*sin(PI2 * i * 1200.0 / Fs);
        lBufInArray[i] = ((signed short)fx) << 16;
    }
}

void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);

        //除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)
            lBufMagArray[i] = (int32_t)(Mag * 32768);
        else
            lBufMagArray[i] = (int32_t)(Mag * 65536);
    }
}

void music_init()
{
    HAL_ADCEx_Calibration_Start(&hadc1);    //AD校准
    HAL_ADC_Start_DMA(&hadc1, adcValue, NPT);
    HAL_TIM_Base_Start(&htim3);
}

void music_stop()
{
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_Base_Stop(&htim3);
}

void music_oled_show()
{
    extern uint8_t oled_fall_pot[127];
    uint8_t len = 0, y = 0, x = 0;
    oled_clear_buffer();
    for (int i = 0; i < NPT/2-1; ++i) {
        x = i;
        len = (lBufMagArray[i+1]/2);
        if (len > 63)
            len = 63;

        y = 63-len;
        oled_draw_vLine(x, y, len);

        if (oled_fall_pot[x] > y){
            oled_fall_pot[x] = y;
        }
        else{
            if (oled_fall_pot[x] > 63)
                oled_fall_pot[x] = 63;
            oled_draw_point(x, oled_fall_pot[x]);
            oled_fall_pot[x] += 2;
        }
    }
    oled_update_screen();
}

void music_rgb_show()
{
    extern uint8_t times;
    extern uint8_t rgb_fall_pot[10];
    uint8_t len, x, y;
    hsv_t fall_pot_hsv = {.h = 55, .s = 0, .v = 80};
    hsv_t pot_hsv = {.h = 0, .s = 255, .v = 80};
    rgb_clear_buffer();
    times ++;
    times %= 4;
    for (int i = 0; i < 10; ++i) {
        x = i*10;
        len = (lBufMagArray[x+1]/10);
        if (len >= 10)
            len = 9;

        y = 9-len;
        if (y >= 9)
            y = 9;
        rgb_draw_hsv_vLine(i, y, len+1, pot_hsv);
        pot_hsv.h = i*18+1;

        if (rgb_fall_pot[i] > y){
            times = 0;
            rgb_fall_pot[i] = y;
        }
        else{
            if (times == 0) {
                rgb_fall_pot[i] += 1;
            }
            if (rgb_fall_pot[i] >= 8)
                rgb_fall_pot[i] = 8;
            rgb_draw_hsv_point(i, rgb_fall_pot[i], fall_pot_hsv);
        }
    }
    rgb_update();
}

uint8_t music_task(uint8_t keyNum)
{
    extern uint8_t music_flag;
    if (music_flag == 0){
        music_flag = 1;
        music_init();
    }
    if (keyNum == 4){
        rgb_clear_buffer();
        rgb_update();
        music_stop();
        music_flag = 0;
        return 4;
    }

    return 0xff;
}

void music_callback()
{
    HAL_ADC_Stop_DMA(&hadc1);
    for (int i = 0; i < NPT; ++i) {
        lBufInArray[i] = ((short )(adcValue[i]-2048))<<16;
    }
    cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
    GetPowerMag();
    music_oled_show();
    music_rgb_show();
    HAL_ADC_Start_DMA(&hadc1, adcValue, NPT);
}
