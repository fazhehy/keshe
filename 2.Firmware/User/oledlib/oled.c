//
// Created by fazhehy on 2023/8/25.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "oled.h"

//TODO:利用断言限制xy范围

static void oled_iic_start()
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

static void oled_iic_stop()
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

static void oled_send_byte(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}

static void oled_write_command(uint8_t command)
{
    oled_iic_start();
    oled_send_byte(0x78);
    oled_send_byte(0x00);
    oled_send_byte(command);
    oled_iic_stop();
}

static void oled_write_data(uint8_t data)
{
    oled_iic_start();
    oled_send_byte(0x78);
    oled_send_byte(0x40);
    oled_send_byte(data);
    oled_iic_stop();
}

/*---------------------------------------------------------------------------------*/
static void oled_fill(uint8_t *buffer)
{
    uint8_t * p = buffer;

//    oled_write_command(0xb0);
//    oled_write_command(0x00);
//    oled_write_command(0x10);

    oled_write_command(0x20);
    oled_write_command(0x00);
    oled_write_command(0x21);
    oled_write_command(0x00);
    oled_write_command(0x7F);
    oled_write_command(0x22);
    oled_write_command(0x00);
    oled_write_command(0x07);

    oled_iic_start();
    oled_send_byte(0x78);
    oled_send_byte(0x40);
    for (uint16_t i = 0; i < 128*8; ++i)
        oled_send_byte(*p++);
    oled_iic_stop();
}

void oled_clear()
{
    oled_write_command(0x20);
    oled_write_command(0x00);
    oled_write_command(0x21);
    oled_write_command(0x00);
    oled_write_command(0x7F);
    oled_write_command(0x22);
    oled_write_command(0x00);
    oled_write_command(0x07);

    oled_iic_start();
    oled_send_byte(0x78);
    oled_send_byte(0x40);
    for (uint16_t i = 0; i < 128*8; ++i)
        oled_send_byte(0x00);
    oled_iic_stop();
}

void oled_init()
{
    HAL_Delay(100);

    oled_write_command(0xAE);//--turn off oled panel

    oled_write_command(0x00);    /*set lower column address*/
    oled_write_command(0x10);    /*set higher column address*/

    oled_write_command(0xB0);    /*set page address*/

    oled_write_command(0x81);    /*contract control*/
    oled_write_command(0x5f);    /*128*/

//        oled_write_command(0x20);    /* Set Memory addressing mode (0x20/0x21) */
//        oled_write_command(0x09);     /* 0x09 */
//
//        oled_write_command(0xA1);    /*set segment remap  0XA1 */
//        oled_write_command(0xC8);    /*Com scan direction   0Xc8  */


//        oled_write_command(0x20);    /* Set Memory addressing mode (0x20/0x21) */
//        oled_write_command(0x02);     /* 0x02 */
//
//        oled_write_command(0xA1);    /*set segment remap  0XA1 */
//        oled_write_command(0xC0);    /*Com scan direction   0Xc0  */


//        oled_write_command(0x20);    /* Set Memory addressing mode (0x20/0x21) */
//        oled_write_command(0x09);     /* 0x09 */
//
//        oled_write_command(0xA0);    /*set segment remap  0XA0 */
//        oled_write_command(0xC0);    /*Com scan direction   0Xc0  */


    oled_write_command(0x20);    /* Set Memory addressing mode (0x20/0x21) */
    oled_write_command(0x02);     /* 0x02 */

    oled_write_command(0xA0);    /*set segment remap  0XA0 */
    oled_write_command(0xC8);    /*Com scan direction   0Xc8  */


    oled_write_command(0xA4);    /*Disable Entire Display On (0xA4/0xA5)*/

    oled_write_command(0xA6);    /*normal / reverse*/

    oled_write_command(0xA8);    /*multiplex ratio*/
    oled_write_command(0x3F);    /*duty = 1/64*/

    oled_write_command(0xD3);    /*set display offset*/
    oled_write_command(0x00);    /*   0x20   */

    oled_write_command(0xD5);    /*set osc division*/
    oled_write_command(0x80);

    oled_write_command(0xD9);    /*set pre-charge period*/
    oled_write_command(0x22);

    oled_write_command(0xDA);    /* Set SEG Pins Hardware Configuration */
    oled_write_command(0x10);

    oled_write_command(0xdb);    /*set vcomh*/
    oled_write_command(0x30);

    oled_write_command(0x8d);    /*set charge pump enable*/
    oled_write_command(0x72);    /* 0x12:7.5V; 0x52:8V;  0x72:9V;  0x92:10V */

    oled_write_command(0xAF);
    oled_clear();
//    oled_write_command(0xAE); //display off
//    oled_write_command(0x20);	//Set Memory Addressing Mode
//    oled_write_command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
//    oled_write_command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
//    oled_write_command(0xc8);	//Set COM Output Scan Direction
//    oled_write_command(0x00); //---set low column address
//    oled_write_command(0x10); //---set high column address
//    oled_write_command(0x40); //--set start line address
//    oled_write_command(0x81); //--set contrast control register
//    oled_write_command(0xff); //亮度调节 0x00~0xff
//    oled_write_command(0xa1); //--set segment re-map 0 to 127
//    oled_write_command(0xa6); //--set normal display
//    oled_write_command(0xa8); //--set multiplex ratio(1 to 64)
//    oled_write_command(0x3F); //
//    oled_write_command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
//    oled_write_command(0xd3); //-set display offset
//    oled_write_command(0x00); //-not offset
//    oled_write_command(0xd5); //--set display clock divide ratio/oscillator frequency
//    oled_write_command(0xf0); //--set divide ratio
//    oled_write_command(0xd9); //--set pre-charge period
//    oled_write_command(0x22); //
//    oled_write_command(0xda); //--set com pins hardware configuration
//    oled_write_command(0x12);
//    oled_write_command(0xdb); //--set vcomh
//    oled_write_command(0x20); //0x20,0.77xVcc
//    oled_write_command(0x8d); //--set DC-DC enable
//    oled_write_command(0x14); //
//    oled_write_command(0xaf); //--turn on oled panel

    oled_clear();
}
/*--------------------------------------------------------------------------*/
uint8_t SCREEN_BUFFER[OLED_HEIGHT / 8][OLED_WIDTH] = {0};
void oled_update_screen()
{
    oled_fill(SCREEN_BUFFER[0]);
}

void oled_clear_buffer()
{
    memset(SCREEN_BUFFER, 0, OLED_WIDTH*OLED_HEIGHT/8);
}
/*--------------------------------------------------------------------------*/

void oled_show_bmp(uint8_t x, uint8_t y, const uint8_t *bmp, uint8_t w, uint8_t h)
{
    for (uint8_t y_ = y; y_ < y+h; y_+=8){
        for (uint8_t x_ = x; x_ < x+w; ++x_){
            SCREEN_BUFFER[y_/8][x_] |= bmp[(y_-y)/8*w+(x_-x)] << (y_%8);
            SCREEN_BUFFER[y_/8+1][x_] |= (bmp[(y_-y)/8*w+(x_-x)] << (y_%8))>>8;
        }
    }
}

void oled_show_char(uint8_t x, uint8_t y, char ch)
{
    for (uint8_t i = 0 ; i < 6; i++){
        SCREEN_BUFFER[y/8][x+i] = oled_font_6x8[ch-' '][i] << (y%8);
        SCREEN_BUFFER[y/8+1][x+i] = (oled_font_6x8[ch-' '][i] << (y%8))>>8;
    }
}

void oled_show_string(uint8_t x, uint8_t y, const char * str)
{
    while (*str){
        oled_show_char(x, y, *str);
        x += 6;
        str ++;
    }
}

uint32_t oled_pow(uint32_t x, uint32_t y)
{
    uint32_t result = 1;
    while (y--)
        result *= x;

    return result;
}

void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    for (uint8_t i = 0; i < len; ++i) {
        oled_show_char(x+i*6, y, '0'+num/oled_pow(10, len-i-1)%10);
    }
}

void oled_draw_point(uint8_t x, uint8_t y)
{
//    if (x >= OLED_WIDTH)
//        x = OLED_WIDTH-1;
//    if (y >= OLED_HEIGHT)
//        y = OLED_HEIGHT-1;

    SCREEN_BUFFER[y/8][x] |= 1 << (y%8);
}

void oled_draw_point_xor(uint8_t x, uint8_t y)
{
//    if (x >= OLED_WIDTH)
//        x = OLED_WIDTH-1;
//    if (y >= OLED_HEIGHT)
//        y = OLED_HEIGHT-1;

    SCREEN_BUFFER[y/8][x] ^= 1 << (y%8);
}

void oled_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    for (uint8_t i = x; i < x + w; ++i) {
        SCREEN_BUFFER[y/8][i] |= 0xff<<(y%8);
        for (uint8_t j = y/8+1; j < (y+h-1)/8; ++j) {
            SCREEN_BUFFER[j][i] = 0xff;
        }
        SCREEN_BUFFER[(y+h-1)/8][i] |= 0xff>>(8-(y+h-1)%8);
    }
}

void oled_fill_rect_xor(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    for (uint8_t i = x; i < x + w; ++i) {
        SCREEN_BUFFER[y/8][i] ^= 0xff<<(y%8);
        for (uint8_t j = y/8+1; j < (y+h-1)/8; ++j) {
            SCREEN_BUFFER[j][i] ^= 0xff;
        }
        SCREEN_BUFFER[(y+h-1)/8][i] ^= 0xff>>(8-(y+h-1)%8);
    }
}

void oled_draw_vLine(uint8_t x, uint8_t y, uint8_t len)
{
    if (len > OLED_HEIGHT)
        len = OLED_HEIGHT;

    for (uint8_t i = 0; i < len; ++i) {
        oled_draw_point(x, y+i);
    }
}

void oled_draw_hLine(uint8_t x, uint8_t y, uint8_t len)
{
    if (len > OLED_WIDTH)
        len = OLED_WIDTH;

    for (uint8_t i = 0; i < len; ++i) {
        oled_draw_point(x+i, y);
    }
}

void oled_draw_vLine_xor(uint8_t x, uint8_t y, uint8_t len)
{
    if (len > OLED_HEIGHT)
        len = OLED_HEIGHT;

    for (uint8_t i = 0; i < len; ++i) {
        oled_draw_point_xor(x, y+i);
    }
}

void oled_draw_hLine_xor(uint8_t x, uint8_t y, uint8_t len)
{
    if (len > OLED_WIDTH)
        len = OLED_WIDTH;

    for (uint8_t i = 0; i < len; ++i) {
        oled_draw_point_xor(x+i, y);
    }
}

void oled_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int16_t dx = (int16_t)x2-x1;
    int16_t dy = (int16_t)y2-y1;

    uint8_t x = x1, y = y1;
    int16_t inc_x, inc_y;
    int16_t error_x = 0, error_y = 0;
    int16_t distance;

    if (dx > 0)
        inc_x = 1;
    else if (dx == 0)
        inc_x = 0;
    else{
        inc_x = -1;
        dx = -dx;
    }

    if (dy > 0)
        inc_y = 1;
    else if (dy == 0)
        inc_y = 0;
    else{
        inc_y = -1;
        dy = -dy;
    }

    distance = dx;
    if (dx < dy)
        distance = dy;
    
    for (uint8_t i = 0; i < distance; ++i) {
        oled_draw_point(x, y);
        error_x += dx;
        error_y += dy;

        if (error_x > distance){
            error_x -= distance;
            x += inc_x;
        }

        if (error_y > distance){
            error_y -= distance;
            y += inc_y;
        }
    }
}

void oled_draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    oled_draw_hLine(x, y, w);
    oled_draw_hLine(x, y+h-1, w);
    oled_draw_vLine(x, y, h);
    oled_draw_vLine(x+w-1, y, h);
}

void oled_draw_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    oled_fill_rect(x, y, w, h);
}

void oled_draw_circle(uint8_t x, uint8_t y, uint8_t r)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x_ = 0;
    int y_ = r;
    while (x_<y_)
    {
        if (f >= 0)
        {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }
        x_++;
        ddF_x += 2;
        f += ddF_x;
        oled_draw_point(x + x_, y + y_);
        oled_draw_point(x + y_, y + x_);
        oled_draw_point(x + x_, y - y_);
        oled_draw_point(x + y_, y - x_);
        oled_draw_point(x - y_, y + x_);
        oled_draw_point(x - x_, y + y_);
        oled_draw_point(x - y_, y - x_);
        oled_draw_point(x - x_, y - y_);
    }
    oled_draw_point(x, y + r);
    oled_draw_point(x, y - r);
    oled_draw_point(x + r, y);
    oled_draw_point(x - r, y);
}

void oled_draw_circle_helper(uint8_t x, uint8_t y, uint8_t r, uint8_t cornername)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x_ = 0;
    int y_ = r;
    while (x_<y_)
    {
        if (f >= 0)
        {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }
        x_++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x04)
        {
            oled_draw_point(x + x_, y + y_);
            oled_draw_point(x + y_, y + x_);
        }
        if (cornername & 0x02)
        {
            oled_draw_point(x + x_, y - y_);
            oled_draw_point(x + y_, y - x_);
        }
        if (cornername & 0x08)
        {
            oled_draw_point(x - y_, y + x_);
            oled_draw_point(x - x_, y + y_);
        }
        if (cornername & 0x01)
        {
            oled_draw_point(x - y_, y - x_);
            oled_draw_point(x - x_, y - y_);
        }
    }
}

void oled_draw_fill_circle(uint8_t x, uint8_t y, uint8_t r)
{
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x_ = 0;
    int y_ = r;
    while (x_ < y_) {
        if (f >= 0) {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }

        x_++;
        ddF_x += 2;
        f += ddF_x;

        oled_draw_vLine(x + x_, y - y_, 2 * y_ + 1);
        oled_draw_vLine(x + y_, y - x_, 2 * x_ + 1);
        oled_draw_vLine(x - x_, y - y_, 2 * y_ + 1);
        oled_draw_vLine(x - y_, y - x_, 2 * x_ + 1);
    }
    oled_draw_vLine(x, y-r, 2*r+1);
}

void oled_draw_fill_circle_helper(uint8_t x, uint8_t y, uint8_t r, uint8_t cornername, uint8_t delta)
{
    // used to do circles and roundrects!
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x_ = 0;
    int y_ = r;
    while (x_ < y_)
    {
        if (f >= 0)
        {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }

        x_++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            oled_draw_vLine(x+x_, y-y_, 2*y_+1+delta);
             oled_draw_vLine(x+y_, y-x_, 2*x_+1+delta);
        }

        if (cornername & 0x2)
        {
            oled_draw_vLine(x-x_, y-y_, 2*y_+1+delta);
            oled_draw_vLine(x-y_, y-x_, 2*x_+1+delta);
        }
    }
}

void oled_draw_fill_circle_helper_xor(uint8_t x, uint8_t y, uint8_t r, uint8_t cornername, uint8_t delta)
{
    // used to do circles and roundrects!
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x_ = 0;
    int y_ = r;
    while (x_ < y_)
    {
        if (f >= 0)
        {
            y_--;
            ddF_y += 2;
            f += ddF_y;
        }

        x_++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            oled_draw_vLine_xor(x+x_, y-y_, 2*y_+1+delta);
            oled_draw_vLine_xor(x+y_, y-x_, 2*x_+1+delta);
        }

        if (cornername & 0x2)
        {
            oled_draw_vLine_xor(x-x_, y-y_, 2*y_+1+delta);
            oled_draw_vLine_xor(x-y_, y-x_, 2*x_+1+delta);
        }
    }
}

void oled_draw_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    // smarter version
    oled_draw_hLine(x+r, y, w-2*r); // Top
    oled_draw_hLine(x+r, y+h-1, w-2*r); // Bottom
    oled_draw_vLine(x, y+r, h-2*r); // Left
    oled_draw_vLine(x+w-1, y+r, h-2*r); // Right
    // draw four corners
    oled_draw_circle_helper(x+r, y+r, r, 1);
    oled_draw_circle_helper(x+w-r-1, y+r, r, 2);
    oled_draw_circle_helper(x+w-r-1, y+h-r-1, r, 4);
    oled_draw_circle_helper(x+r, y+h-r-1, r, 8);
}

void oled_draw_fill_round_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    // smarter version
    oled_fill_rect(x+r, y, w-2*r, h+1);
    oled_update_screen();

    // draw four corners
    oled_draw_fill_circle_helper(x+w-r-1, y+r, r, 1, h-2*r-1);
    oled_draw_fill_circle_helper(x+r, y+r, r, 2, h-2*r-1);
}

void oled_draw_fill_round_rect_xor(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    oled_fill_rect_xor(x+r, y, w-2*r, h+1);

    oled_draw_fill_circle_helper(x+w-r-1, y+r, r, 1, h-2*r-1);
    oled_draw_fill_circle_helper(x+r, y+r, r, 2, h-2*r-1);
}

uint8_t oled_printf(uint8_t x, uint8_t y, const char *format, ...)
{
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return 0;
    }
    va_end(arg);
    oled_show_string(x, y, temp);
    return len;
}
