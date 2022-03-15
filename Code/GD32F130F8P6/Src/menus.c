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
#include "74hc595_led.h"
#include "encoder.h"
#include "max6675.h"


extern uint32_t test_number;
extern EC11_t ec11_1;
uint16_t temp = 0;
uint32_t clk_count = 0;

static uint8_t now_index = 0;

menu_t menu_list[] = {
    // 当前 下 上 enter 退出
    {0, 0, 0, 1, 0, main_page}, // 主页，索引 1
    {1, 2, 3, 4, 0, fu_01 },    // 二级菜单的第一个Item 索引 2
    {2, 3, 1, 9, 0, fu_02 },    // 二级菜单的第一个Item 索引 3
    {3, 1, 2, 13, 0, fu_03 },    // 二级菜单的第一个Item 索引 4

    // 二级菜单
    {4, 4, 4, 4, 0, fu_04} // 设置 test_number 的值
};


/* 主页 */
void main_page(void)
{
    // if(clk_count>=100)
    // {
    //     temp = max6675_read_data();
    //     clk_count = 0;
    // }
    hc595_show_number(test_number);
}

/* 一级菜单 */
void fu_01(void)
{
    hc595_show_strings("FU01");
}

void fu_02(void)
{
    hc595_show_strings("FU02");
}

void fu_03(void)
{
    hc595_show_strings("FU03");
}

void fu_04(void)
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
        hc595_show_number(test_number);
    }

    end_fu_04:
    now_index = 1; // 退出 fu_04 函数，并设置 now_index 为二级菜单的第一个 Item

}

void fu_05(void)
{
    hc595_show_strings("FU05");
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