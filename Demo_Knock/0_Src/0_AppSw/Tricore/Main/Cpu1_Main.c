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

#include <Crankcfg.h>
#include <DPLL_Driver.h>
#include "IfxScuWdt.h"
#include "CrankSignal_pub.h"

void core1_main (void)
{
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
    IfxScuWdt_disableSafetyWatchdog (IfxScuWdt_getSafetyWatchdogPassword ());

	IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
    GTM_CLC.U = 0;
    GTM_OCS.B.SUS_P = 1;
    GTM_OCS.B.SUS 	= 1;
    GTM_OCS.B.SUS_P = 0;
	IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
	__enable(); //Enable Interrupts
	CrankSimulationGen(&CrankValue);
	DebugPort_Ini();
	//test_aa();
	//SignalGenComplete =TRUE; /*This variable is for sync with DPLL*/

	while(1)
	{

	}
}
