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
#ifndef __BUZZER_H
#define __BUZZER_H
#include "gd32f1x0.h"

#define BUZZER_RCU  RCU_GPIOA
#define BUZZER_PORT GPIOA
#define BUZZER_PIN  GPIO_PIN_3

void buzzer_init(void);
void buzzer_on(uint16_t us);
void buzzer_off(void);
void buzzer_timer_init(void);
#endif /* __BUZZER_H */
