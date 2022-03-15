/**
 * @file ds18b20.h
 * @author your name (you@domain.com)
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
#include <stdbool.h>

#define DS18B20_RCU     RCU_GPIOA
#define DS18B20_PORT    GPIOA
#define DS18B20_PIN     GPIO_PIN_6

#define DS18B20_OUT(x) { gpio_mode_set(DS18B20_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS18B20_PIN); \
                         gpio_bit_write(DS18B20_PORT, DS18B20_PIN, x); } \



typedef struct
{
    float temp;    // 温度
    int8_t th;      // 高报值
    int8_t tl;      // 低报值
} ds18b20_t;


void                ds18b20_init(ds18b20_t *ds18b20);
bool                ds18b20_ack(void);
void                ds18b20_write_byte(uint8_t data);
uint8_t             ds18b20_read_byte(void);
bool                ds18b20_start(void);
bool                ds18b20_get_temp(ds18b20_t *ds18b20);

static void         gpio_init(void);
static FlagStatus   ds18b20_io_read(void);

#endif // __DS18B20_H
