/**
 * @file flash.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "flash.h"

#define FMC_PAGE_SIZE           ((uint16_t)0x400)   /* 1024 1KB */
#define FMC_WRITE_START_ADDR    ((uint32_t)0x0800FC00U)
#define FMC_WRITE_END_ADDR      ((uint32_t)0x08010000U) /* 64KB */


/* 定义与初始化 param_t */
param_t system_param = {
    .temp_h             = 0,
    .temp_l             = 0,
    .seg_light          = 4,
    .time_week          = 0x3f,
    .time_hour          = 7,
    .time_minutes       = 0,
    .time_count         = 0,
    .time_timer_num     = 60 * 3, /* 3 分钟*/
};

/**
 * @brief 写 param 参数到 Flash
 * 
 */
void param_write(void)
{

}

/**
 * @brief 读取配置参数到 param 结构体
 * 
 * @param param 
 */
void param_read(param_t *param)
{
    static uint32_t i = 0;
    param->temp_h = FMC_READ(FMC_WRITE_START_ADDR);
    ++i;
    param->temp_l = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->seg_light = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->time_week = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->time_hour = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->time_minutes = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->time_count = FMC_READ(FMC_WRITE_START_ADDR+i);
    ++i;
    param->time_timer_num = FMC_READ(FMC_WRITE_START_ADDR+i);
}
