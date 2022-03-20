/*******************************************************************************
 * @file        menus.c
 * @date        2022.1.12
 * @author      信念D力量
 * @brief       数码管菜单程序
 * @github      https://github.com/zsf90/GD32F150C8T6-EXAMPLE
 * @bilibili    https://space.bilibili.com/334715750
 * 
 * 顺时针：向下翻页
 * 逆时针：向上翻页
 * 长按：  进入设置页面（二级菜单）
 * 单击：确定
 * 双击退出（返回）
 ******************************************************************************/
#include "menus.h"
#include "encoder.h"
#include "tm1637.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "delay.h"

extern ds18b20_t ds18b20;
extern uint32_t loop_display_count;
extern uint32_t loop_display_flag; /* 1: 显示温度 2: 显示时间 */
extern EC11_t ec11_1;

uint16_t temp = 0;
uint32_t clk_count = 0;
uint32_t test_number;

static uint8_t now_index = 0;

menu_t menu_list[] = {
    /* 当前 下 上 enter 退出 */
    {0, 0, 0, 1, 0, main_page}, // 主页，索引 1
    /* 二级菜单 */
    {1, 2, 5, 6,  0, fu_01 },    /* 二级菜单的第一个Item 索引 1 */
    {2, 3, 1, 7,  0, fu_02 },    /* 二级菜单的第一个Item 索引 2 */
    {3, 4, 2, 8, 0, fu_03 },    /* 二级菜单的第一个Item 索引 3 */
    {4, 5, 3, 9, 0, fu_04 },    /* 二级菜单的第一个Item 索引 4 */
    {5, 1, 4, 10, 0, fu_05 },    /* 二级菜单的第一个Item 索引 5 */

    // 二级菜单
    {6, 6, 6, 6, 0, fu_01_fun},
    {7, 7, 7, 7, 0, fu_02_fun},
    {8, 8, 8, 8, 0, fu_03_fun},
    {9, 9, 9, 9, 0, fu_04_fun},
    {10, 10, 10, 10, 0, fu_05_fun},
};


/* 主页 */
void main_page(void)
{
    if(loop_display_flag == 1)
    {
    	ds18b20_get_temp(&ds18b20);
    	// printf("当前温度: %.3f°C\n", ds18b20.temp);
    	tm1637_display_float(ds18b20.temp); /* 显示温度 */
    	delay_ms(100);
    } else if(loop_display_flag == 2) {
    	ds1302_read_time();
    	tm1637_display_time(&tm1637); /* 显示时间 */
    }
}

/* 一级菜单 */
void fu_01(void)
{
    tm1637_display_menu(FU01);
}

void fu_02(void)
{
    tm1637_display_menu(FU02);
}

void fu_03(void)
{
    tm1637_display_menu(FU03);
}

void fu_04(void)
{
    tm1637_display_menu(FU04);
}

/**
 * @brief 数码管亮度设置 
 * 
 */
void fu_05(void)
{
    tm1637_display_menu(FU05);
}

void fu_01_fun(void)
{
    while(1){
        ec11_return_result _ec11_results = encoder_handle();
        switch (_ec11_results)
        {
        case EC11_CW:
            test_number+=1;
            ec11_1.return_reslut = EC11_NONE;
            break;
        case EC11_CCW:
            test_number-=1;
            ec11_1.return_reslut = EC11_NONE;
            break;
        case EC11_CLICK:
            ec11_1.return_reslut = EC11_NONE;
            goto end_fu_04;
        default:
            break;
        }
        tm1637_display_int(test_number);
    }

    end_fu_04:
    now_index = 1; // 退出 fu_04 函数，并设置 now_index 为二级菜单的第一个 Item

}

void fu_02_fun(void)
{

}

void fu_03_fun(void)
{

}

void fu_04_fun(void)
{

}

void fu_05_fun(void)
{
    uint8_t brightness = 4;
    while(1){
        ec11_return_result _ec11_results = encoder_handle();
        switch (_ec11_results)
        {
        case EC11_CW:
            if(brightness < 8) {
                brightness++;
                tm1637.brightness = TM1637_ON + brightness -1;
            }
            ec11_1.return_reslut = EC11_NONE;
            break;
        case EC11_CCW:
            if(brightness > 1) {
                brightness--;
                tm1637.brightness = TM1637_ON + brightness -1;
            }
            ec11_1.return_reslut = EC11_NONE;
            break;
        case EC11_CLICK:
            ec11_1.return_reslut = EC11_NONE;
            goto end_fu_05;
        default:
            break;
        }
        tm1637_display_int(brightness);
    }

    end_fu_05:
    now_index = 1; // 退出函数，并设置 now_index 为二级菜单的第一个 Item
}

void menu_loop(void)
{
    
    ec11_return_result _ec11_results = encoder_handle();
    switch (_ec11_results)
    {
    case EC11_CW:
        now_index = menu_list[now_index].down_index;
        ec11_1.return_reslut = EC11_NONE;
        break;
    case EC11_CCW:
        now_index = menu_list[now_index].up_index;
        ec11_1.return_reslut = EC11_NONE;
        break;
    case EC11_LONG_PRESS: // 长按退出菜单
        now_index = menu_list[now_index].esc_index;
        ec11_1.return_reslut = EC11_NONE;
        /* code */
        break;
    case EC11_DOUBLE_CLICK: // 双击进入菜单
        now_index = menu_list[now_index].enter_index;
        ec11_1.return_reslut = EC11_NONE;
        break;

    case EC11_CLICK: // 单击进入设置界面
        now_index = menu_list[now_index].enter_index;
        ec11_1.return_reslut = EC11_NONE;
        break;
    
    default:
        break;
    }
    ec11_1.return_reslut = EC11_NONE;
    menu_list[now_index].operate();
    
}
