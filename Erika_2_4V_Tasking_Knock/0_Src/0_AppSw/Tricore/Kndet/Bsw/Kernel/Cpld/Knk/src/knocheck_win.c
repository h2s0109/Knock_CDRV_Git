/*
 * knocheck_Win.c
 *
 *  Created on: 2015. 5. 11.
 *      Author: hansteve
 */


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "knocheck_priv.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void KnoWindow_Output_Ini(void)
{
	GTM_ATOM2_CH5_CTRL.B.MODE = 0x1; 		/*01B ATOM Signal Output Mode Compare (SOMC)*/
	GTM_ATOM2_CH5_CTRL.B.TB12_SEL = 0x0;	/*(TBU_TS1 selected for comparison)*/
	GTM_ATOM2_CH5_CTRL.B.ARU_EN =0x0; 		/*ARU Input stream disabled*/
	GTM_ATOM2_CH5_CTRL.B.ACB = 0x15;
	GTM_ATOM2_CH5_CTRL.B.RST_CCU0 = 0;
	/*Set output signal level to 1
	Serve Last: Compare in CCU0 and then in CCU1
	using TBU_TS1 or TBU_TS2. Output signal level
	when CCU0 matches is defined by combination
	of SL, ACB10(5) and ACB10(4). On the CCU1
	match the output level is toggled.*/

	GTM_ATOM2_CH5_CTRL.B.CMP_CTRL = 0; /*Greater/equal compare against TBU time base values (TBU_TSx >= CMx)*/
	GTM_ATOM2_CH5_CTRL.B.SL = 0; /*Low Level*/
	/*GTM_ATOM2_CH5_CTRL.B.CLK_SRC = 0; //Forward the Period Trigger to the next Channel*/


	//ATOM2 Global registers settingsl7
	GTM_ATOM2_AGC_GLB_CTRL.B.UPEN_CTRL5 = 0x2;
	GTM_ATOM2_AGC_ENDIS_CTRL.B.ENDIS_CTRL5 = 0x2;
	GTM_ATOM2_AGC_OUTEN_CTRL.B.OUTEN_CTRL5 = 0x2;
	GTM_ATOM2_AGC_FUPD_CTRL.B.FUPD_CTRL5 = 0x2;
	GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT5 = 0x2;
	GTM_ATOM2_AGC_GLB_CTRL.B.HOST_TRIG = 1;

	GTM_ATOM2_CH5_IRQ_MODE.U = 0x2;
	GTM_ATOM2_CH5_IRQ_EN.U = 0X3;/*CCU0TC and CCU1TC support*/

	SRC_GTMATOM22.U = (2 << 11) | (1 << 10) | 52; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/
	GTM_DSADCOUTSEL10.B.SEL1 = 5; /*connected to ATOM2_5*/

	GTM_TOUTSEL2.B.SEL3=2;
	P33_IOCR12.B.PC13=0x11;

}
