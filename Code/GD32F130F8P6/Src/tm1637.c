/**
 * @file tm1637.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "tm1637.h"
#include "delay.h"
#include <stdio.h>
#include "ds1302.h"


tm1637_t tm1637;

extern uint8_t TIME[];


// dp g f e d c b a
// sg1 ~ sg8
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---  (DP)
//      D
// 共阴
const uint8_t SEGData[]={
    /*0*/0x3f,  // 0
    /*1*/0x06,  // 1
    /*2*/0x5b,  // 2
    /*3*/0x4f,  // 3
    /*4*/0x66,  // 4
    /*5*/0x6d,  // 5
    /*6*/0x7d,  // 6
    /*7*/0x07,  // 7
    /*8*/0x7f,  // 8
    /*9*/0x6f,  // 9
    /*10*/0x77, // A
    /*11*/0x7c, // B
    /*12*/0x39, // C
    /*13*/0x5e, // D
    /*14*/0x79, // E
    /*15*/0x71, // F
    /*16*/0x50, // r
    /*17*/0x6e, // y
    /*18*/0x1c, // u
    /*19*/0x3e, // U
    /*20*/0x78, // t
    /*21*/0x31, // T
    /*22*/0x76, // H
    /*23*/0x74, // h
    /*24*/0x1e, // j
    /*25*/0x38, // L
    /*26*/0x54, // n
    /*27*/0x5c, // o
    /*28*/0x67, // q
    /*29*/0x80, // .
    /*30*/0x40, // - 减号
    /*31*/0x08, // _ 下划线
    /*32*/0x00 // 关闭显示
};

// 菜单表
const uint8_t menu_table[][4] = {
    {0x71, 0x3e, 0x3f, 0x06},   // FU01 Index->0
    {0x71, 0x3e, 0x3f, 0x5b},   // FU02 Index->1
    {0x71, 0x3e, 0x3f, 0x4f},   // FU03 Index->2
    {0x71, 0x3e, 0x3f, 0x66},   // FU04 Index->3
    {0x71, 0x3e, 0x3f, 0x6d},   // FU05 Index->4
    {0x71, 0x3e, 0x3f, 0x7d},   // FU06 Index->5
    {0x71, 0x3e, 0x3f, 0x07},   // FU07 Index->6
    {0x71, 0x3e, 0x3f, 0x7f},   // FU08 Index->7
    {0x71, 0x3e, 0x3f, 0x6f},   // FU09 Index->8
    {0x71, 0x3e, 0x06, 0x3f},   // FU10 Index->9
    {0x71, 0x3e, 0x06, 0x06},   // FU11 Index->10
    {0x71, 0x3e, 0x06, 0x5b},   // FU12 Index->11
    {0x79, 0x50, 0x50, 0x3f},   // ERR0 Index->12
    {0x79, 0x50, 0x50, 0x06}    // ERR1 Index->13
};



/**
 * @brief       GPIO Config
 * @param       Void
 * @retval      None
 */
void tm1637_gpio_init(void)
{
    /* CLK Config */
    rcu_periph_clock_enable(TM1637_CLK_RCU);
    /* configure led GPIO port */
    gpio_mode_set(TM1637_CLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TM1637_CLK_PIN);
    gpio_output_options_set(TM1637_CLK_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, TM1637_CLK_PIN);

    GPIO_BC(TM1637_CLK_PORT) = TM1637_CLK_PIN;

    /* SDA Config */
    rcu_periph_clock_enable(TM1637_DIO_RCU);
    /* configure led GPIO port */
    gpio_mode_set(TM1637_DIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TM1637_DIO_PIN);
    gpio_output_options_set(TM1637_DIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, TM1637_DIO_PIN);

    GPIO_BC(TM1637_DIO_PORT) = TM1637_DIO_PIN;
}


/**
 * @brief   lock 上锁
 * 
 * @param   tm1637
 * @retval  None
 */
void tm1637_lock(tm1637_t *tm1637)
{
    while (tm1637->lock == 1)
    {
        delay_ms(1);
    }
    tm1637->lock = 1;
}

/**
 * @brief lock 解锁
 * 
 * @param tm1637
 * @retval None 
 */
void tm1637_unlock(tm1637_t *tm1637)
{
    tm1637->lock = 0;
}


/**
 * @brief tm1637 初始化
 * 
 * @param tm1637 
 */
void tm1637_init(tm1637_t *tm1637)
{
	tm1637_gpio_init(); // GPIO 初始化
    tm1637->brightness = TM1637_ON + 3;
}


void tm1637_display_auto(uint8_t n)
{
    uint8_t i;
    tm1637_write_command(TM1637_ADDRESS_MODE_AUTO);
    i2c_stop();

    tm1637_write_command(TM1637_ADDRESS);

    for (i=0; i<5; i++) {
        i2c_write_byte(n);
        i2c_ask();
    }
    i2c_stop();

    tm1637_write_command(tm1637.brightness);
    i2c_stop();
}

/**
 * @brief 以固定地址单独显示一位 1 2 3 4 5 6
 * 
 * @param addr 
 * @param data 
 */
void tm1637_display_fixed(uint8_t addr, uint8_t data)
{
    tm1637_write_command(TM1637_ADDRESS_MODE_FIXED);
    i2c_stop();

    tm1637_write_command(TM1637_ADDRESS + addr -1);

    i2c_write_byte(data);
    i2c_ask();
    i2c_stop();

    tm1637_write_command(tm1637.brightness);

    i2c_stop();
}

/**
 * @brief 显示整数，最大 9999
 * 
 * @param num 
 */
void tm1637_display_int(uint16_t num)
{
    uint8_t ge      = num % 10;
    uint8_t shi     = num / 10 % 10;
    uint8_t bai     = num / 100 % 10;
    uint8_t qian    = num / 1000 % 10;

    tm1637_display_fixed(5, SEGData[SEG_None]);
    
    if(num > 999) {
        tm1637_display_fixed(4, SEGData[ge]);
        tm1637_display_fixed(3, SEGData[shi]);
        tm1637_display_fixed(2, SEGData[bai]);
        tm1637_display_fixed(1, SEGData[qian]);
    }

    if(num < 1000 && num > 99){
        tm1637_display_fixed(4, SEGData[ge]);
        tm1637_display_fixed(3, SEGData[shi]);
        tm1637_display_fixed(2, SEGData[bai]);
        tm1637_display_fixed(1, SEGData[SEG_None]);
    }

    if(num < 100 && num > 9) {
        tm1637_display_fixed(4, SEGData[ge]);
        tm1637_display_fixed(3, SEGData[shi]);
        tm1637_display_fixed(2, SEGData[SEG_None]);
        tm1637_display_fixed(1, SEGData[SEG_None]);
    }

    if(num<10){
        tm1637_display_fixed(4, SEGData[ge]);
        tm1637_display_fixed(3, SEGData[SEG_None]);
        tm1637_display_fixed(2, SEGData[SEG_None]);
        tm1637_display_fixed(1, SEGData[SEG_None]);
    }
}

/**
 * @brief 显示菜单 
 * 
 * @param menu 
 */
void tm1637_display_menu(menu_enum menu)
{
    uint8_t i;
    for(i=0;i<4;i++)
    {
        tm1637_display_fixed(i+1, menu_table[menu][i]);
    }
    tm1637_display_fixed(5, SEGData[SEG_None]);
}

/**
 * @brief 显示小数 （23.32）
 * 
 * @param fnum 
 */
void tm1637_display_float(float fnum)
{
    uint16_t int_part; /* 整数部分 */
    int_part = (uint16_t)fnum;

    uint8_t ge = int_part % 10;
    uint8_t shi = int_part / 10 % 10;
    uint8_t bai = int_part / 100 % 10;
    uint8_t qian = int_part / 1000;

    float dec_part; /*小数部分 */
    dec_part = fnum - int_part;

    tm1637_display_fixed(5, SEGData[SEG_None]);

    if(int_part < 9) {
        /**
         * @brief 小于9时显示1位整数+两位小数，整数显示在第3位
         * 
         */
        tm1637_display_fixed(1, SEGData[SEG_None]);
        tm1637_display_fixed(2, SEGData[ge] | 0x80);

        /* 显示小数部分 : 0.352 */
        uint8_t dec_part_x100;
        dec_part_x100 = dec_part * 100;

        uint8_t dec_ge, dec_shi;
        dec_ge = dec_part_x100 % 10;
        dec_shi = dec_part_x100 / 10 % 10;
        tm1637_display_fixed(3, SEGData[dec_shi]);
        tm1637_display_fixed(4, SEGData[dec_ge]);
    } else if (int_part > 9 && int_part < 100) {
        /**
         * @brief 显示2位整数+2位小数
         * 
         */
        tm1637_display_fixed(1, SEGData[shi]);
        tm1637_display_fixed(2, SEGData[ge] | 0x80);

        /* 显示小数部分 : 0.352 */
        uint8_t dec_part_x100;
        dec_part_x100 = dec_part * 100;

        uint8_t dec_ge, dec_shi;
        dec_ge = dec_part_x100 % 10;
        dec_shi = dec_part_x100 / 10 % 10;
        tm1637_display_fixed(3, SEGData[dec_shi]);
        tm1637_display_fixed(4, SEGData[dec_ge]);
    } else if (int_part > 100 && int_part < 1000) {
        /**
         * @brief 显示 3位整数+1位小数
         * 
         */
        tm1637_display_fixed(1, SEGData[bai]);
        tm1637_display_fixed(2, SEGData[shi]);
        tm1637_display_fixed(3, SEGData[ge] | 0x80);

        /* 显示小数部分 : 0.352 */
        uint8_t dec_part_x100;
        dec_part_x100 = dec_part * 100;

        uint8_t dec_shi;
        dec_shi = dec_part_x100 / 10 % 10;
        tm1637_display_fixed(4, SEGData[dec_shi]);
    } else if (int_part > 999) {
        tm1637_display_fixed(4, SEGData[ge]);
        tm1637_display_fixed(3, SEGData[shi]);
        tm1637_display_fixed(2, SEGData[bai]);
        tm1637_display_fixed(1, SEGData[qian]);
    }
}

/**
 * @brief 显示时间
 * 
 * @param tm1637 
 */
void tm1637_display_time(tm1637_t *tm1637)
{
    uint8_t hour_h, hour_l, min_h, min_l, time_point;
    hour_h = time_buffer[HOUR]  / 10 % 10;
    hour_l = time_buffer[HOUR] % 10;
    min_h = time_buffer[MINUTES] / 10 % 10;
    min_l = time_buffer[MINUTES] % 10;

    time_point = time_buffer[SECOND];
    tm1637_display_fixed(1, SEGData[hour_h]);
    tm1637_display_fixed(2, SEGData[hour_l]);
    tm1637_display_fixed(3, SEGData[min_h]);
    tm1637_display_fixed(4, SEGData[min_l]);
    if (time_point % 2 == 0) {
        tm1637_display_fixed(5, SEGData[SEG_None]);
    } else {
        tm1637_display_fixed(5, SEGData[SEG_point]);
    }
}

/* 亮度设置 1~8 */
void tm1637_set_brightness(tm1637_t *tm1637, uint8_t brightness)
{
    tm1637->brightness = TM1637_ON + brightness - 1;
}

/***************************************** 静态函数 *****************************************/

/**
 * @brief DIO 读
 * 
 * @return FlagStatus 
 */
static FlagStatus tm1637_dio_read(void)
{
    TM1637_DIO_IN;
    return gpio_input_bit_get(TM1637_DIO_PORT, TM1637_DIO_PIN);
}


/**
 * @brief IIC Start
 * 
 */
static void i2c_start(void)
{
    TM1637_CLK_HIGH();      // 1
    TM1637_DIO_HIGH();      // 1
    delay_1us(IIC_DELAY);
    TM1637_DIO_LOW();       // 0
}

/**
 * @brief IIC ASK 应答
 * 
 */
static void i2c_ask(void)
{
    TM1637_CLK_LOW();           // 0
    delay_1us(5);
    while(tm1637_dio_read());
    TM1637_CLK_HIGH();          // 1
    delay_1us(2);
    TM1637_CLK_LOW();           // 0
}

/**
 * @brief IIC Stop 
 * 
 */
static void i2c_stop(void)
{
    TM1637_CLK_LOW();
    delay_1us(IIC_DELAY);
    TM1637_DIO_LOW();
    delay_1us(IIC_DELAY);

    TM1637_CLK_HIGH();
    delay_1us(IIC_DELAY);
    TM1637_DIO_HIGH();
}


/**
 * @brief IIC 写字节
 * 
 * @param data 
 */
static void i2c_write_byte(uint8_t data)
{
    uint8_t i;
    for (i=0; i<8; i++) {
        TM1637_CLK_LOW();
        if ((data & 0x01) != 0) {
            TM1637_DIO_HIGH();
        } else {
            TM1637_DIO_LOW();
        }
        delay_1us(IIC_DELAY);
        data = data>>1;
        TM1637_CLK_HIGH();
        delay_1us(IIC_DELAY);
    }
}

/**
 * @brief TM1637 写命令
 * 
 * @param cmd 
 */
static void tm1637_write_command(uint8_t cmd)
{
    i2c_start();
    i2c_write_byte(cmd);
    i2c_ask();
}

static void tm1637_clear(void)
{
    tm1637_display_fixed(1, 0);
    tm1637_display_fixed(2, 0);
    tm1637_display_fixed(3, 0);
    tm1637_display_fixed(4, 0);
}
