/**
 * \file Cpu2_Main.c
 * \brief Main function definition for Cpu core 2 .
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
#include <DPLL_Driver.h>
#include "IfxScuWdt.h"
#include "Ifx_reg.h"
#include"knocheck_pub.h"
static uint32 speedIstRpm = 0;
uint8 Sync_tooth = DEFAULT_SYNC;
void core2_main (void)
{
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
    IfxScuWdt_disableSafetyWatchdog (IfxScuWdt_getSafetyWatchdogPassword ());
    __enable ();

#ifdef KNOCHECK_USE
	KnoCheck_Dsadc_Ini();
	KnoCheck_Dma_Ini();
	KnoWindow_Output_Ini();
	Knocheck_Ini();
#endif
    	//Reset_Procedure();
		DPLL_Clock_Init();
		DPLL_IR_Init ();
		DPLL_Init();

		CrankState_Init ();
		CrankProfile_Init();
		CrankInput_Init();

		Auxiliary_ATOM40_Init();

		Set_WaitEndTime(1000);
		Set_IgnoreTooth(3);
		Set_NMinPresyncTeeth (5);

		Crank_Start();

		while (Get_CrankState() != FULL_SYNC);
		Set_EventOnTooth (Sync_tooth);
		while (1)
		{
			if (GTM_DPLL_STATUS.B.LOCK1 == 1)
			{
				speedIstRpm = Get_InstantRpm ();
			}
		}
}
