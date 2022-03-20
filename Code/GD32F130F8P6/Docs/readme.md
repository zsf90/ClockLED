# 项目调试笔记

## DS18B20 调试不通

刚开始调试 ds18b20 一直调试不同，后来发现是 delay_1us() 微妙延时函数出现了问题。问题是延时长了一倍，所以 ds18b20 的时序就不对了，把 delay_1us() 调准之后就可以读出温度了。

```c
/**
 * @brief 微秒延时定时器初始化
 * 
 */
void us_timer_init(void)
{
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER13); // TIMER13 16位通用定时器
    timer_deinit(TIMER13);

    timer_struct_para_init(&timer_initpara);

    /* TIMER 参数配置 */
    timer_initpara.prescaler = 36-1;    // 定时器预分频
    timer_initpara.alignedmode  = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
    timer_initpara.period   = 1; // 自动重装载值
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER13, &timer_initpara);
    timer_auto_reload_shadow_enable(TIMER13);
    timer_flag_clear(TIMER13, TIMER_FLAG_UP);
    timer_disable(TIMER13);
}

void delay_1us(uint16_t us)
{
    timer_enable(TIMER13); // 启动定时器
    while(us--)
    {
        while(timer_flag_get(TIMER13, TIMER_FLAG_UP) == RESET)
        {
            ;
        }
        timer_flag_clear(TIMER13, TIMER_FLAG_UP);
    }
    timer_disable(TIMER13);
}

void delay_ms(uint16_t ms)
{
	while(ms--)
	{
		delay_1us(1000);
	}
}

```

