/**
 * @file ds1302.h
 * @author your name (you@domain.com)
 * @brief DS1302 时钟芯片
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 * DS1302_CLK: PA0
 * DS1302_DIO: PA1
 * DS1302_RST: PA5
 */
#ifndef __DS1302_H
#define __DS1302_H

#include "gd32f1x0.h"

#define DS1302_CLK_RCU  RCU_GPIOA
#define DS1302_CLK_PIN  GPIO_PIN_0
#define DS1302_CLK_PORT     GPIOA

#define DS1302_DIO_RCU  RCU_GPIOA
#define DS1302_DIO_PIN  GPIO_PIN_1
#define DS1302_DIO_PORT     GPIOA

#define DS1302_RST_RCU  RCU_GPIOA
#define DS1302_RST_PIN  GPIO_PIN_5
#define DS1302_RST_PORT     GPIOA

#define DS1302_CLK_WRITE(x)     gpio_bit_write(DS1302_CLK_PORT, DS1302_CLK_PIN, x)
#define DS1302_DIO_WRITE(x)     gpio_bit_write(DS1302_DIO_PORT, DS1302_DIO_PIN, x)
#define DS1302_RST_WRITE(x)     gpio_bit_write(DS1302_RST_PORT, DS1302_RST_PIN, x)

#define DS1302_DIO_READ()       gpio_input_bit_get(DS1302_DIO_PORT, DS1302_DIO_PIN)

#define DS1302_CONTROL              0x8e        /* 控制写寄存器 */
#define DS1302_CHARGER              0x90

#define DS1302_WRITE_CLOCK_BURST    0xbe        /* 写时钟突发模式寄存器 */

#define DS1302_READ_RAM_BURST       0xff        /* 读RAM突发模式寄存器 */
#define DS1302_WRITE_RAM_BURST      0xfe        /* 写RAM突发模式寄存器 */

#define RAMSIZE                     0x31        /* Ram Size in bytes */
#define DS1302_RAMSTART             0xc0        /* First Address */

/**
 * @brief 时钟枚举
 * 
 */
typedef enum
{
    SECOND=0,   /* 秒 */
    MINUTES,    /* 分钟 */
    HOUR,       /* 小时 */
    DAY,        /* 日 */
    MONTH,      /* 月 */
    WEEK,       /* 周 */
    YEAR,       /* 年 */
} ds1302_time_enum;

/**
 * @brief 函数声明 
 * 
 */
void ds1302_gpio_init(void);
void ds1302_init(void);
void ds1302_read_time(void);
void ds1302_write(uint8_t addr, uint8_t data);
uint8_t ds1302_read(uint8_t addr);

extern uint8_t time_buffer[];
#endif // __DS1302_H
