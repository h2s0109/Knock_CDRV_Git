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
#include "Ifx_Utility.h"
#include "Ifx_Types.h"
#include "IfxCpu_Intrinsics.h"
#include "IfxScuWdt.h"
#include "Ifx_reg.h"
#include "IfxVrs_Driver.h"

//#include "MCS_test.h"
static uint32 speedAvrRpm = 0;
static uint32 speedIstRpm = 0;
uint8 Sync_tooth = DEFAULT_SYNC;

int core2_main (void)
{

    uint16 endinitPw;
    endinitPw = IfxScuWdt_getCpuWatchdogPassword ();
    IfxScuWdt_disableCpuWatchdog (endinitPw);

    endinitPw = IfxScuWdt_getSafetyWatchdogPassword ();
    IfxScuWdt_disableSafetyWatchdog (endinitPw);

    __enable ();

    endinitPw = IfxScuWdt_getCpuWatchdogPassword ();
    IfxScuWdt_clearCpuEndinit (endinitPw);

    GTM_CLC.U = 0;
    GTM_OCS.B.SUS_P = 1;
    GTM_OCS.B.SUS = 1;
    GTM_OCS.B.SUS_P = 0;
    IfxScuWdt_setCpuEndinit (endinitPw);

    __enable ();

    if(Init_Complete==TRUE)
    {
    	Debug_port_Ini();
    	Dpll_Interrupt_Ini ();//검토 완료

		IfxVrs_init ();   //constructor
		IfxVrs_setWaitStart (1000); //Wait 15000xtCLK after the 1st tooth detected
		Set_Ignore_Tooth(3);
		IfxVrs_setNMinPresyncTeeth (5);    //Wait 1 teeth is PRE-SYNC
		IfxVrs_start ();            //Start VRS the driver.
		//Wait for the DPLL/Flywheel sync.
		while (IfxVrs_getFlywheelStat () != IfxVrs_FlywheelStat_sync);
		IfxVrs_setEventOnTooth (Sync_tooth);
		while (1)
		{
			if (GTM_DPLL_STATUS.B.LOCK1 == 1)
			{
				speedAvrRpm = IfxVrs_getAvrRpm ();
				speedIstRpm = IfxVrs_getIstRpm ();
			}
		}
    }
	return (1);


}
