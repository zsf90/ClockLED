/**
 * @file buzzer.h
 * @author 信念D力量 (zsf.cn.90@gmail.com)
 * @brief 蜂鸣器驱动
 * @version 0.1
 * @date 2022-04-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "buzzer.h"
#include "ds18b20.h"
#include "flash.h"
#include "delay.h"

void buzzer_init(void)
{
    /* CLK Config */
    rcu_periph_clock_enable(BUZZER_RCU);
    /* configure led GPIO port */
    gpio_mode_set(BUZZER_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZER_PIN);
    gpio_output_options_set(BUZZER_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BUZZER_PIN);

    GPIO_BC(BUZZER_PORT) = BUZZER_PIN;
}

void buzzer_on(uint16_t us)
{
    static __IO uint32_t time_count = 0;
    if(time_count && (system_params.temp_h > ds18b20.temp)) {
        if(us < 10) {
            gpio_bit_write(BUZZER_PORT, BUZZER_PIN, SET);
        } else {
            gpio_bit_write(BUZZER_PORT, BUZZER_PIN, RESET);
        }
        time_count--;
    } else {
        buzzer_off();
        time_count = us * 2;
    }
}

void buzzer_off(void)
{
    gpio_bit_write(BUZZER_PORT, BUZZER_PIN, RESET);
}

void buzzer_timer_init(void)
{
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER14); // TIMER14 16位通用定时器
    timer_deinit(TIMER14);

    timer_struct_para_init(&timer_initpara);

    /* TIMER 参数配置 */
    timer_initpara.prescaler = 36-1;    // 定时器预分频
    timer_initpara.alignedmode  = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    timer_initpara.period   = 10; // 自动重装载值
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER14, &timer_initpara);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER14, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER14, TIMER_INT_UP);
    nvic_irq_enable(TIMER14_IRQn, 0, 0);
    timer_enable(TIMER14);
}
