/**
 * @file ds18b20.h
 * @author 信念D力量 (zsf.cn.90@gmail.com)
 * @brief DS18B20 单总线温度传感器。
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __DS18B20_H
#define __DS18B20_H

#include "gd32f1x0.h"

#define DS18B20_RCU     RCU_GPIOA
#define DS18B20_PORT    GPIOA
#define DS18B20_PIN     GPIO_PIN_6

#define DS18B20_WRITE(x)    gpio_bit_write(DS18B20_PORT, DS18B20_PIN, x)
#define DS18B20_READ()      gpio_input_bit_get(DS18B20_PORT, DS18B20_PIN)


typedef struct
{
    float       temp;    // 温度
    uint16_t    tempHLData;
    int8_t      th;      // 高报值
    int8_t      tl;      // 低报值
    uint16_t    refresh_rate; /* 刷新频率 */
} ds18b20_t;

extern ds18b20_t ds18b20;


void                ds18b20_init(ds18b20_t *ds18b20);
uint8_t             ds18b20_ack(void);
float               ds18b20_get_temp(ds18b20_t *ds18b20);

#endif // __DS18B20_H
