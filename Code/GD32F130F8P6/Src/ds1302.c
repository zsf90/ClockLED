/**
 * @file ds1302.h
 * @author your name (you@domain.com)
 * @brief DS1302 时钟芯片
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "ds1302.h"
#include "delay.h"
#include "tools.h"

#define IN  0
#define OUT 1

uint8_t time_buffer[7];     /* BCD 转换后的数组 */

/* DS1302 读写地址寄存器 秒：分：时：日-月-周-年 */
uint8_t READ_RTC_ADDR[7]    = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
uint8_t WRITE_RTC_ADDR[7]   = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

/* DS1302 初始化2016年5月7日星期六12点00分00秒 */
/* 存储顺序是秒分时日月周年,存储格式是用BCD码 */
uint8_t TIME[7] = {0, 0, 0x12, 0x07, 0x05, 0x06, 0x16};


/* 静态函数声明 */
static void ds1302_dio_mode(uint8_t in_or_out);
static void ds1302_write_cmd(uint8_t cmd_or_addr);

/**
 * @brief 初始化时钟,设置时钟时间
 * 
 */
void ds1302_init(void)
{
    uint8_t n;
    ds1302_write(0x8e, 0x00);
    for (n=0; n<7; n++) {
        ds1302_write(WRITE_RTC_ADDR[n], HEX2BCD(TIME[n])); /* 要把 hex 转换为 BCD */
    }
    ds1302_write(0x8e, 0x80);
    delay_ms(1000);
}

/**
 * @brief 读时间
 * 
 */
void ds1302_read_time(void)
{
    uint8_t n;
    for (n=0; n<7; n++) {
        time_buffer[n] = BCD2HEX(ds1302_read(READ_RTC_ADDR[n]));
    }
}

/**
 * @brief GPIO初始化
 * 
 */
void ds1302_gpio_init(void)
{
    rcu_periph_clock_enable(DS1302_CLK_RCU);
    rcu_periph_clock_enable(DS1302_RST_RCU);

    gpio_mode_set(DS1302_CLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_CLK_PIN);
    gpio_mode_set(DS1302_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_RST_PIN);

    gpio_output_options_set(DS1302_CLK_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DS1302_CLK_PIN);
    gpio_output_options_set(DS1302_RST_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DS1302_RST_PIN);

    GPIO_BC(DS1302_CLK_PORT) = DS1302_CLK_PIN;
    GPIO_BC(DS1302_RST_PORT) = DS1302_RST_PIN;
}

/**
 * @brief 写字节
 * 
 * @param addr 
 * @param data 
 */
void ds1302_write(uint8_t addr, uint8_t data)
{
    DS1302_RST_WRITE(0);
    delay_1us(1);

    DS1302_CLK_WRITE(0);
    delay_1us(1);
    DS1302_RST_WRITE(1);
    delay_1us(1);

    ds1302_write_cmd(addr);

    ds1302_write_cmd(data);

    DS1302_RST_WRITE(0);
    delay_1us(1);
}

/**
 * @brief 读字节
 * 
 * @param addr 
 * @return uint8_t 
 */
uint8_t ds1302_read(uint8_t addr)
{
    uint8_t n, data, data1;
    data = 0;
    data1 = 0;
    DS1302_RST_WRITE(0);
    delay_1us(1);

    DS1302_CLK_WRITE(0);
    delay_1us(1);
    DS1302_RST_WRITE(1);
    delay_1us(1);

    ds1302_write_cmd(addr);
    
    delay_1us(1);

    ds1302_dio_mode(IN);
    for (n=0; n<8; n++) {
        data1 = DS1302_DIO_READ();
        data = (data>>1) | (data1<<7);
        DS1302_CLK_WRITE(1);
        delay_1us(1);
        DS1302_CLK_WRITE(0);
        delay_1us(1);
    }
    DS1302_RST_WRITE(0);
    delay_1us(1);
    DS1302_CLK_WRITE(1);
    delay_1us(1);
    DS1302_DIO_WRITE(0);
    delay_1us(1);
    DS1302_DIO_WRITE(1);
    delay_1us(1);
    
    return data;
}

/**
 * @brief 设置 DIO IO 方向，IN or OUT
 * 
 * @param in_or_out 
 */
static void ds1302_dio_mode(uint8_t in_or_out)
{
    /* 使能外设时钟 */
    rcu_periph_clock_enable(DS1302_DIO_RCU);
    
    if (in_or_out == IN)
    {
        /* 配置 GPIO 模式：输入 | 无上拉下拉电阻 */
        gpio_mode_set(DS1302_DIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DS1302_DIO_PIN);
    }
    else if (in_or_out == OUT)
    {
        /* 配置 GPIO 模式：输出 | 无上拉下拉电阻 */
        gpio_mode_set(DS1302_DIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS1302_DIO_PIN);
    }
    /* 设置 GPIO 其他属性：类型|速度，类型：推挽|开漏 */
    gpio_output_options_set(DS1302_DIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DS1302_DIO_PIN);
    /* GPIO 位清除 */
    GPIO_BC(DS1302_DIO_PORT) = DS1302_DIO_PIN;
}

/**
 * @brief 写命令与地址
 * 
 */
static void ds1302_write_cmd(uint8_t cmd_or_addr)
{
    uint8_t i;

    ds1302_dio_mode(OUT);
    for (i=0; i<8; i++) {
        DS1302_DIO_WRITE(cmd_or_addr & 0x01);
        cmd_or_addr >>= 1;
        DS1302_CLK_WRITE(1);
        delay_1us(1);
        DS1302_CLK_WRITE(0);
        delay_1us(1);
    }
}
