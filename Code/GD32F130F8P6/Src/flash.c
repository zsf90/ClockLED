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
#include "gd32f1x0.h"
#include "flash.h"
#include "string.h"

#define FMC_PAGE_SIZE           ((uint16_t)0x400)   /* 1024 1KB */
#define FMC_WRITE_START_ADDR    ((uint32_t)0x0800FC00U)
#define FMC_WRITE_END_ADDR      ((uint32_t)0x0800FFFFU) /* 64KB */

param_t system_params = {
    .is_first = 0xf1,
    .temp_h = 35.5f,
    .temp_l = 12.5f,
    .time_timer_num = 500,
    .seg_light = 4,
    .time_week = 3,
    .time_hour = 7,
    .time_minutes = 30,
    .time_count = 120,
};

static void fmc_param_pages_erase(void)
{
    fmc_unlock();
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    while(FMC_BUSY == fmc_page_erase(FMC_WRITE_START_ADDR)); /* 从 FMC_WRITE_START_ADDR 开始擦除 */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    fmc_lock();
}

/**
 * @brief 写 param 参数到 Flash
 * 
 */
static void fmc_param_program(uint32_t *data, int data_len)
{
    fmc_unlock();   /* 解锁 */
    uint32_t address = FMC_WRITE_START_ADDR;
    while(address <= FMC_WRITE_END_ADDR) {
        if(data_len <= 0) break;
        fmc_word_program(address, *data);
        address += 4U;
        data++;
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
        data_len -= 4;
    }
    fmc_lock();     /* 上锁 */
}

int system_param_write(param_t *params)
{
    if(params == NULL) return -1;
    param_union_t param_union;
    memset(&param_union, 0, sizeof(param_union_t));
    memcpy(&param_union.system_param, params, sizeof(param_t));
    fmc_param_pages_erase();
    fmc_param_program((uint32_t*)&param_union, sizeof(param_union_t));

    return 0;
}

/**
 * @brief 读取配置参数到 param 结构体
 * 
 * @param param 
 */
int system_param_read(param_t *param)
{
    static uint32_t i = 0;
    param_union_t param_union;
    uint8_t *ptr = (uint8_t*)FMC_WRITE_START_ADDR;
    if(*ptr == 0xf1)
    {
        memcpy(&param_union, ptr, sizeof(param_union_t));
        memcpy(param, &param_union.system_param, sizeof(param_t));
        return 0;
    }
    return -1;
}
