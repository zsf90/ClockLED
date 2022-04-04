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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FMC_READ(addrx) REG32(addrx)

/**
 * @brief 系统参数结构体
 * 
 */
typedef struct
{
    uint8_t     is_first;            /* 0xF1：读出上次的数据；非 0xF1 写入默认参数 */
    float       temp_h;             /* 温度高报         4byte */
    float       temp_l;             /* 温度低报         4byte*/
    uint32_t    time_timer_num;     /* 定时时长单位秒    4byte*/
    uint8_t     seg_light;          /* 数码管亮度 1~8   1byte*/
    uint8_t     time_week;          /* 闹钟“周”         1byte*/
    uint8_t     time_hour;          /* 小时             1byte*/
    uint8_t     time_minutes;       /* 分钟            1byte*/
    uint8_t     time_count;         /* 计数器单位秒      1byte*/
} param_t;

/**
 * @brief 用于存储到 Flash 的 Union 公用体
 * 
 */
typedef union
{
    param_t system_param; /* 20 Byte */
    uint32_t data[sizeof(param_t)/4];
} param_union_t;

int    system_param_read(param_t *param);
int     system_param_write(param_t *params);

extern param_t system_params;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __FLASH_H */
