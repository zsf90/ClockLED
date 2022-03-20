/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gd32f1x0.h"
#include "systick.h"
#include "debug_printf.h"
#include <stdio.h>
#include "delay.h"
#include "tm1637.h"
#include "encoder.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "menus.h"

uint32_t num = 0;

uint8_t key_state = 0;
uint32_t loop_display_count = 0;
uint32_t loop_display_flag = 0; /* 1: 显示温度 2: 显示时间 */

int main(void)
{
	systick_config();
	debug_printf_init(EVAL_COM0);
	us_timer_init();
	tm1637_init(&tm1637);
	encoder_exit_config(ENCODER_MODE_EXIT);
	ds18b20_init(&ds18b20);
	ds1302_gpio_init();
	ds1302_init();


	if(ds18b20_ack()) {
		printf("DS18B20 Init Error!\n");
	} else {
		printf("DS18B20 Init OK!\n");
	}
	delay_ms(1000);
	while(1)
	{
		
		menu_loop();
	}
}
