/**
 * @file ds18b20.c
 * @author your name (you@domain.com)
 * @brief DS18B20 单总线温度传感器。
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "ds18b20.h"
#include "delay.h"
#include <stdio.h>

/**
 * @brief DS18B20 结构体初始化和GPIO初始化
 * 
 * @param ds18b20 
 */
void ds18b20_init(ds18b20_t *ds18b20)
{
    ds18b20->tl = 10;
    ds18b20->th = 40;

    gpio_init();
}

/**
 * @brief DS18B20 应答，每次通信前都要调用
 * 
 * @return true     存在
 * @return false    不存在 
 */
bool ds18b20_ack(void)
{
    DS18B20_OUT(0);
    delay_1us(125);
    DS18B20_OUT(1);
    delay_1us(68);
    bool t = ds18b20_io_read();

    return !t;
}

/**
 * @brief 写一个字节
 * 
 * @param data 
 */
void ds18b20_write_byte(uint8_t data)
{
    uint8_t mask;
    for(mask=0x01; mask!=0; mask<<=1)
    {
        DS18B20_OUT(0);
        delay_1us(2);
        if((mask&data) == 0){
            DS18B20_OUT(0);
        } else {
            DS18B20_OUT(1);
        }
    }
    delay_1us(60);
    DS18B20_OUT(1);
}


/**
 * @brief 读一个字节
 * 
 * @return uint8_t 
 */
uint8_t ds18b20_read_byte(void)
{
    uint8_t data, mask;
    for(mask=0x01; mask!=0; mask<<=1)
    {
        DS18B20_OUT(0);
        delay_1us(2);
        DS18B20_OUT(1);
        delay_1us(2);
        if (!ds18b20_io_read()) {
            data &= ~mask;
        } else {
            data |= mask;
        }
        delay_1us(60);
    }
    return data;
}

/**
 * @brief 开始温度转换
 * 
 * @return true 
 * @return false 
 */
bool ds18b20_start(void)
{
    bool ack;
    ack = ds18b20_ack();
    if(ack) {
        ds18b20_write_byte(0xCC);
        ds18b20_write_byte(0x44);
    }
    return ack;
}

/**
 * @brief 读取温度
 * 
 * @param ds18b20 
 * @return true 
 * @return false 
 */
bool ds18b20_get_temp(ds18b20_t *ds18b20)
{
    float tp;
    int16_t temp_16;
    bool ack;
    uint8_t lsb, msb;
    ack = ds18b20_ack();
    if(ack) {
        ds18b20_write_byte(0xcc);
        ds18b20_write_byte(0xbe);
        lsb = ds18b20_read_byte();
        msb = ds18b20_read_byte();

        temp_16 = msb;
        temp_16 <<= 8;
        temp_16 |= lsb;

        temp_16 >>= 4;
        temp_16 = temp_16 * 10;
        ds18b20->temp = (float)(temp_16 + (lsb & 0x0f) * 0.625);
    }

    return ack;
}

/**
 * @brief GPIO 初始化
 * 
 */
void gpio_init(void)
{
    /* CLK Config */
    rcu_periph_clock_enable(DS18B20_RCU);
    /* configure led GPIO port */
    gpio_mode_set(DS18B20_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DS18B20_PIN);
    gpio_output_options_set(DS18B20_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, DS18B20_PIN);

    GPIO_BC(DS18B20_PORT) = DS18B20_PIN;
}

/**
 * @brief IO 读
 * 
 */
static FlagStatus ds18b20_io_read(void)
{
    gpio_mode_set(DS18B20_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DS18B20_PIN);
    return gpio_input_bit_get(DS18B20_PORT, DS18B20_PIN);
}
