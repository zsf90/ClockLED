/**
 * @file ds18b20.c
 * @author your name (you@domain.com)
 * @brief DS18B20 单总线温度传感器。
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "ds18b20.h"
#include "delay.h"
#include <stdio.h>

ds18b20_t ds18b20;

#define IN  1
#define OUT 2

/**
 * @brief Static 函数声明 
 * 
 */
static void ds18b20_mode(uint8_t in_or_out);
static void ds18b20_write_byte(uint8_t data);
static uint8_t ds18b20_read_byte(void);
static float caculate_temp(uint8_t tmh, uint8_t tml);

/**
 * @brief DS18B20 结构体初始化和GPIO初始化
 * 
 * @param ds18b20 
 */
void ds18b20_init(ds18b20_t *ds18b20)
{
    ds18b20->tl = 10;
    ds18b20->th = 40;
    ds18b20->refresh_rate = 100; /* 100ms 刷新一次 */
}

/**
 * @brief 判断 DS18B20 是否存在
 * 
 * @return uint8_t
 * \ 1: DS18B20 不存在
 * \ 0: DS18B20 存在
 */
uint8_t ds18b20_ack(void)
{
    uint8_t ds_flag;    // DS18B20 是否存在标志
    ds18b20_mode(OUT);  // 设置 DS18B20 为输出模式
    DS18B20_WRITE(0);   // 输出 0
    delay_1us(600);     // 延时 600us
    DS18B20_WRITE(1);   // 输出 1
    delay_1us(30);      // 延时 30us
    ds18b20_mode(IN);   // 设置 DS18B20 为输入模式
    ds_flag = DS18B20_READ();
    delay_1us(500);     // 延时 500us

    return ds_flag;
}

/**
 * @brief 写一个字节
 * 
 * @param data 
 */
static void ds18b20_write_byte(uint8_t data)
{
    uint8_t i;
    for (i=8; i>0; i--)
    {
        ds18b20_mode(OUT);
        DS18B20_WRITE(0);   // 开始发送
        delay_1us(2);       // 至少延时 1us
        if((data & 0x01) == 1)
        {
            DS18B20_WRITE(1);
        } else
        {
            DS18B20_WRITE(0);
        }
        delay_1us(70);  // 至少 60us
        DS18B20_WRITE(1);
        data >>= 1;
    }
}


/**
 * @brief 读一个字节
 * 
 * @return uint8_t 
 */
static uint8_t ds18b20_read_byte(void)
{
    uint8_t i, data = 0;
    // 1. data >>= 1 右移一位数据抛弃低位的数据
	// 2. data |= 0x80; // data 与 0x80 按位或运算，最高位写1
	// 两者配合完成数据的接收，从低到高接收
    for (i=8; i>0; i--)
    {
        ds18b20_mode(OUT);
        DS18B20_WRITE(0);
        delay_1us(2);
        DS18B20_WRITE(1);
        delay_1us(15);
        data >>= 1;
        if(DS18B20_READ() == 1)
        {
            data |= 0x80;
        }
        delay_1us(50);
    }

    return data;
}


/**
 * @brief 读取温度
 * 
 * @param ds18b20 
 * @return float 
 */
float ds18b20_get_temp(ds18b20_t *ds18b20)
{
    /*
    读到的温度数据 Byte0,Byte1 (12位精度)
    TempL  2^3  2^2  2^1  2^0  2^-1  2^-2  2^-3  2^-4
    TempH   S    S    S    S    S    2^6   2^5   2^4
    S=0(温度为正)  S=1(温度为负)
    */
    uint8_t tempL, tempH;

    ds18b20_ack();
    ds18b20_write_byte(0xcc); /* 跳过读取序列号(ROM) */
    ds18b20_write_byte(0x44); /* 启动温度检测 */

    delay_ms(800);

    ds18b20_ack();
    ds18b20_write_byte(0xcc); /* 跳过读取序列号(ROM) */
    ds18b20_write_byte(0xbe); /* 读取温度寄存器内容(读取头两个值为温度) */

    tempL = ds18b20_read_byte();
    tempH = ds18b20_read_byte();

    ds18b20->temp = caculate_temp(tempH, tempL);

    return ds18b20->temp;
}

/**
 * @brief 设置 DS18B20 IO 方向
 * // in_or_out: 控制 IO 口的方向标志，1：输入 2：输出
 * 
 */
static void ds18b20_mode(uint8_t in_or_out)
{
    /* 使能外设时钟 */
    rcu_periph_clock_enable(DS18B20_RCU);
    
    if (in_or_out == IN)
    {
        /* 配置 GPIO 模式：输入 | 无上拉下拉电阻 */
        gpio_mode_set(DS18B20_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DS18B20_PIN);
    }
    else if (in_or_out == OUT)
    {
        /* 配置 GPIO 模式：输出 | 无上拉下拉电阻 */
        gpio_mode_set(DS18B20_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS18B20_PIN);
    }
    /* 设置 GPIO 其他属性：类型|速度，类型：推挽|开漏 */
    gpio_output_options_set(DS18B20_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DS18B20_PIN);
    /* GPIO 位清除 */
    GPIO_BC(DS18B20_PORT) = DS18B20_PIN;
}


/**
 * @brief 温度计算
 * 
 * @param tmh 
 * @param tml 
 * @return float 
 */
static float caculate_temp(uint8_t tmh, uint8_t tml)
{
    uint8_t th, tl;
    float temp = 0;

    tl = tml & 0x0f;

    th = (tmh << 4) + (tml >> 4);

    temp = (int)th;

    if(tmh > 0x08)
    {
        th = ~th + 1;
        temp = -th;
    }

    temp += tl * 0.0625;
    // printf("temp: %.3f\n", temp);

    return temp;
}
