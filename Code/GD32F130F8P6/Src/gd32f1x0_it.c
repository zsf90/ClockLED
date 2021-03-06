/*!
    \file    gd32f1x0_it.c
    \brief   interrupt service routines

    \version 2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    \version 2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    \version 2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2019-11-20, V3.2.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2020-09-21, V3.3.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f1x0_it.h"
#include "main.h"
#include "systick.h"

#include "encoder.h"
#include "ds18b20.h"
#include "buzzer.h"
#include "flash.h"

extern uint32_t num;
extern EC11_t ec11_1;
extern uint32_t loop_display_count;
extern uint32_t loop_display_flag; /* 1: ???????????? 2: ???????????? */


/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();

    // ??????????????????
    if(ec11_1.sw_down_flag == 1){
        ec11_1.sw_down_time++; // ??????????????????++
    }

    // ??? SW ?????????????????? led_auto_setp++
    if(ec11_1.return_reslut == EC11_LONG_PRESS)
    {
        ec11_1.sw_long_press_time++;
    }

    loop_display_count++;
	if(loop_display_count <= 4000) {
		loop_display_flag = 1;
	} else if (loop_display_count > 4000 && loop_display_count < 12000) {
		loop_display_flag = 2;
	} else if (loop_display_count > 12000) {
		loop_display_count = 0;
	}

}

/*******************************************************************************
 * @brief 0-1??????
 * @brief CLK ??? DT
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    
}

/*******************************************************************************
 * @brief 2-3 ????????????
 * @brief SW
 ************************************************************************** ****/
void EXTI4_15_IRQHandler(void)
{
    /**
     * @brief ?????????????????????????????????
     * 
     */
    if(SET == exti_interrupt_flag_get(ENCODER_SW_EXTI_LINE)){
        ec11_1.sw_down_flag = 1;
        exti_interrupt_flag_clear(ENCODER_SW_EXTI_LINE);
    }

    //???????????????????????? SW ????????????/////////////////////////////////////////////////////////////////////////////////

    // ec11_1.clk_count == 0
    if(SET == exti_interrupt_flag_get(ENCODER_CLK_EXTI_LINE) && clk_value == RESET && ec11_1.clk_count == 0)
    {
        ec11_1.clk_flag = 0;
        if(dt_value) ec11_1.clk_flag = 1;

        ec11_1.clk_count = 1;
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
    }

    // ec11_1.clk_count == 1
    if(SET == exti_interrupt_flag_get(ENCODER_CLK_EXTI_LINE) && clk_value == SET && ec11_1.clk_count == 1)
    {
        // ?????????
        if(dt_value == RESET && ec11_1.clk_flag == 1)
        {
            #if ENABLE_LONG_PRESS == ENABLE
            ec11_1.return_result = EC11_CW;
            #elif ENABLE_LONG_PRESS == DISABLE
            if(sw_value == RESET && ec11_1.sw_down_500ms_1000ms_flag == true)
            {
                ec11_1.return_reslut = EC11_DOWN_CW; // ?????????????????????????????????
                ec11_1.is_rotate_flag = true;
            } else {
                ec11_1.return_reslut = EC11_CW; // ?????????
            }
            #endif 
        }
        
        // ?????????
        if(dt_value && ec11_1.clk_flag == 0)
        {
            if(sw_value == RESET && ec11_1.sw_down_500ms_1000ms_flag == true)
            {
                ec11_1.return_reslut = EC11_DOWN_CCW; // ?????????????????????????????????
                ec11_1.is_rotate_flag = true;
            } else {
                ec11_1.return_reslut = EC11_CCW; // ?????????
            }
            
        }
        ec11_1.clk_count = 0;
        exti_interrupt_flag_clear(ENCODER_CLK_EXTI_LINE);
    }
}

/**
 * @brief ????????? 14
 * 
 */
void TIMER14_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER14, TIMER_INT_UP)){
        if(ds18b20.temp > system_params.temp_h) {
            GPIO_OCTL(BUZZER_PORT) ^= BUZZER_PIN; 
        } else {
            buzzer_off();
        }
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER14, TIMER_INT_FLAG_UP);
    }
}
