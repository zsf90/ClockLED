/**
 * @file flash.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __FLASH_H
#define __FLASH_H
#include "gd32f1x0.h"

#define FMC_READ(addrx) (*(volatile uint32_t*)(uint32_t)(addrx))


typedef struct
{
    int8_t      temp_h;             /* 温度高报 */
    int8_t      temp_l;             /* 温度低报 */
    uint8_t     seg_light;          /* 数码管亮度 1~8 */
    uint8_t     time_week;          /* 闹钟“周” */
    uint8_t     time_hour;          /* 小时 */
    uint8_t     time_minutes;       /* 分钟 */
    uint8_t     time_count;         /* 计数器单位秒 */
    uint16_t    time_timer_num;     /* 定时时长单位秒 */
} param_t;


void param_write(void);
void param_read(param_t param);

#endif /* __FLASH_H */