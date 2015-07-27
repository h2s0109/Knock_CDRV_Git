/**
 * \file Cpu1_Main.c
 * \brief Main function definition for Cpu core 1 .
 *
 * \copyright Copyright (c) 2012 Infineon Technologies AG. All rights reserved.
 *
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

#include "Ifx_Types.h"
#include "IfxCpu_Intrinsics.h"
#include "IfxScuWdt.h"
#include "Crank_Signal.h"
#include "Ifx_reg.h"
#include "CDRV_setup.h"
#include "Ifx_Utility.h"

int rpm;
double revs;
double tooth;
uint32 compare;

int core1_main (void)
{
	uint16 endinitPw;
	endinitPw = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(endinitPw);
	GTM_CLC.U = 0;
	IfxScuWdt_setCpuEndinit(endinitPw);

	__enable(); //Enable Interrupts
    //Configure CMU

	Reset_Procedure();
	DPLL_Clock_Ini();

	uint8 Pulse_Select = 0;

		switch (Pulse_Select) {
			/****************************************************************************************/
		case 0:
			/*
			 * ATOM2_CH0 ====> P00.9 ====> TIM1_CH0
			 * PWM 200ms (5.0Hz), 50% Duty Cycle
			 */

			setupGtmClocks(); // CMU_CLK0 100Mhz (10ns), CMU_CLK1 25Mhz (40ns)

		    	Init_Complete =TRUE; /*This variable is for sync with DPLL*/



			Crank_Period =(uint32)PERIOD_SETUP;
			Crank_Duty =(float)DUTY_SETUP;
			Crank_Signal(Crank_Period,Crank_Duty); //Generate a PWM on ATOM2_CH0 => P00.9
#ifdef CAM_PULSE
			CAM_Signal();
#endif
			//setupGtmTbu();
			/*!
			 * 1. Try to change the config. to count only the Rising/Falling Edge
			 * 2. Enable CMU_CLK1
			 * 3. Change the ATOM2_CH0 CLK_SRC from CMU_CLK0 to CMU_CLK1
			 */
			break;
			/************************************************************************************/
		default:
			break;
		}

    while (TRUE)
    {

    }
    return (1);
}
