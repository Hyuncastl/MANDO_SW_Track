/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
 /*\title UART communication via ASCLIN module
 * \abstract An ASCLIN module configured for UART communication sends "Hello World!" and receives the string via the internal loopback.
 * \description The string "Hello World!" is sent and received via UART through one pin due to the internal loopback.
 *              The data can be visualized using an oscilloscope.
 *
 * \name ASCLIN_UART_1_KIT_TC275_LK
 * \version V1.0.0
 * \board AURIX TC275 lite Kit, KIT_AURIX_TC275_LITE, TC27xTP_D-Step
 * \keywords ASC, ASCLIN, ASCLIN_UART_1, AURIX, Oscilloscope, UART, communication
 * \documents https://www.infineon.com/aurix-expert-training/Infineon-AURIX_ASCLIN_UART_1_KIT_TC275_LK-TR-v01_00_00-EN.pdf
 * \documents https://www.infineon.com/aurix-expert-training/TC27D_iLLD_UM_1_0_1_12_0.chm
 * \lastUpdated 2021-06-29
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ASCLIN_UART.h"
#include "IO_PORT.h"
#include "ADC_Single_Channel.h"
unsigned char ABC;
IfxCpu_syncEvent g_cpuSyncEvent = 0;

int adc_value = 0;

sint32 r_buff_cnt = 0;


union
{
    float data;
    char  bytedata[4];
}  m_adc_value;

int core0_main(void)
{
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required*/
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    init_ASCLIN_UART();                 /* Initialize the module                  */
    pin_pull_up();
    pin_pull_down();
    IfxCpu_enableInterrupts();          /* Enable interrupts after initialization */
    //send_receive_ASCLIN_UART_message(); /* Send the string                        */
    vadcBackgroundScanInit();
    vadcBackgroundScanRun();

    while(1)
    {
        r_buff_cnt = receive_buff_count();
        ABC = receive_ASCLIN_UART_message();
        out_putt(ABC);
        push_button();
        adc_value = indicateConversionValue();
        send_ASCLIN_UART_message(ABC,adc_value);
    }
    return (1);
}
