/*******************************************************************************
 * @file        menus.h
 * @date        2022.1.12
 * @author      信念D力量
 * @brief       数码管菜单程序
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 ******************************************************************************/
#ifndef __MENUS_H
#define __MENUS_H


#include "gd32f1x0.h"

/* 定义菜单结构体 */
typedef struct
{
    uint8_t current_index; /* 当前菜单索引 */
    uint8_t down_index;
    uint8_t up_index;
    uint8_t enter_index;
    uint8_t esc_index;
    void (*operate)();
} menu_t;

/* 主页 */
void main_page(void);

/* 一级菜单 */
void fu_01(void);
void fu_02(void);
void fu_03(void);
void fu_04(void);
void fu_05(void);

void fu_01_fun(void);
void fu_02_fun(void);
void fu_03_fun(void);
void fu_04_fun(void);
void fu_05_fun(void);

void menu_loop(void);


#endif
