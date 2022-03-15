/**
 * @file tm1637.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __TM1637_H
#define __TM1637_H

#include "gd32f1x0.h"
#include <stdbool.h>

#define TM1637_GRID_1 ((uint8_t)(1<<0))
#define TM1637_GRID_2 ((uint8_t)(1<<1))
#define TM1637_GRID_3 ((uint8_t)(1<<2))
#define TM1637_GRID_4 ((uint8_t)(1<<3))
#define TM1637_GRID_5 ((uint8_t)(1<<4))
#define TM1637_GRID_6 ((uint8_t)(1<<5))
#define TM1637_GRID_ALL 0xFF

#define TM1637_KEY_READ				0x42 	// 读按键
#define TM1637_ADDRESS_MODE_AUTO	0x40	// 地址自动加一
#define TM1637_ADDRESS_MODE_FIXED	0x44	// 固定地址
#define	TM1637_ADDRESS				0xC0	// 开始地址 
#define TM1637_OFF					0x80	// 关显示
#define TM1637_ON					0x88	// 开显示

/* IO 口定义 */
#define TM1637_CLK_RCU	RCU_GPIOA
#define TM1637_CLK_PIN	GPIO_PIN_9
#define TM1637_CLK_PORT	GPIOA

#define TM1637_CLK_LOW() 				gpio_bit_write(TM1637_CLK_PORT, TM1637_CLK_PIN, RESET)
#define TM1637_CLK_HIGH() 			gpio_bit_write(TM1637_CLK_PORT, TM1637_CLK_PIN, SET)

#define TM1637_DIO_RCU	RCU_GPIOA
#define TM1637_DIO_PIN	GPIO_PIN_10
#define TM1637_DIO_PORT	GPIOA

#define TM1637_DIO_OUT				gpio_mode_set(TM1637_DIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TM1637_DIO_PIN)
#define TM1637_DIO_IN 				gpio_mode_set(TM1637_DIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TM1637_DIO_PIN)
#define TM1637_DIO_LOW()			{ TM1637_DIO_OUT; gpio_bit_write(TM1637_DIO_PORT, TM1637_DIO_PIN, RESET); }
#define TM1637_DIO_HIGH()			{ TM1637_DIO_OUT; gpio_bit_write(TM1637_DIO_PORT, TM1637_DIO_PIN, SET); }


#define IIC_DELAY					2
#define IIC_DELAY_ASK				5

extern const uint8_t SEGData[];

typedef enum
{
	SEG_0=0,
	SEG_1,
	SEG_2,
	SEG_3,
	SEG_4,
	SEG_5,
	SEG_6,
	SEG_7,
	SEG_8,
	SEG_9,
	SEG_A,
	SEG_B,
	SEG_C,
	SEG_D,
	SEG_E,
	SEG_F,
	SEG_r,
	SEG_y,
	SEG_u,
	SEG_U,
	SEG_t,
	SEG_T,
	SEG_H,
	SEG_h,
	SEG_j,
	SEG_L,
	SEG_n,
	SEG_o,
	SEG_q,
	SEG_point,
	SEG_minus,
	SEG_under_line,
	SEG_None
} seg_t;

// 菜单枚举
typedef enum
{
    FU01=0,
    FU02,
    FU03,
    FU04,
    FU05,
    FU06,
    FU07,
    FU08,
    FU09,
    FU10,
	FU11,
    FU12,
    ERR0,
    ERR1
} menu_enum;

typedef struct
{
	uint8_t hour;
	uint8_t min;
} time_t;

typedef struct
{
	uint8_t 	lock;
	uint8_t 	brightness;		// 亮度（1-8）
	bool		is_none; 			// 清屏标志
	time_t 		time;				// 时间
} tm1637_t;




void tm1637_init(tm1637_t *tm1637); 					// TM1637 初始化
void tm1637_display_auto(uint8_t n); 					// 地址自增显示
void tm1637_display_fixed(uint8_t addr, uint8_t data); 	// 固定地址显示

void tm1637_display_float(float fnum);					// 显示小数
void tm1637_display_time(tm1637_t *tm1637);				// 显示时间
void tm1637_display_int(uint16_t num); 					// 显示整数
void tm1637_display_menu(menu_enum menu); 				// 显示菜单

/**
 * 静态函数声明 
 * 
 */
static FlagStatus tm1637_dio_read(void);
static void i2c_start(void);
static void i2c_ask(void);
static void i2c_stop(void);
static void i2c_write_byte(uint8_t data);
static void tm1637_write_command(uint8_t cmd);
static void tm1637_clear(void);
static void tm1637_brightness(tm1637_t *tm1637, uint8_t brightness);
#endif /* __TM1637_H */
