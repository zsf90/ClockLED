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

uint32_t num = 0;

uint8_t key_state = 0;

extern tm1637_t tm1637;
extern EC11_t ec11_1;
ds18b20_t ds18b20;

int main(void)
{
	systick_config();
	debug_printf_init(EVAL_COM0);
	us_timer_init();
	tm1637_init(&tm1637);
	encoder_exit_config(ENCODER_MODE_EXIT);
	delay_1ms(1500);
	ds18b20_init(&ds18b20);
	while(1)
	{
		ds18b20_start();
		ds18b20_get_temp(&ds18b20);
		printf("%f\n", ds18b20.temp);

	}
}
