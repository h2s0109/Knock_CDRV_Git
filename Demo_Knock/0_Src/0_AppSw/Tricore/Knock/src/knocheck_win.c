/*
 * knocheck_Win.c
 *
 *  Created on: 2015. 5. 11.
 *      Author: hansteve
 */


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <knocheck_priv.h>
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
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.MODE 		= 0x1; 				/*01B ATOM Signal Output Mode Compare (SOMC)*/
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.TB12_SEL 	= 0x0;				/*(TBU_TS1 selected for comparison)*/
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.ARU_EN 		= 0x0; 				/*ARU Input stream disabled*/
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.ACB 			= 0x15;
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.RST_CCU0 	= 0;
	/*Set output signal level to 1
	Serve Last: Compare in CCU0 and then in CCU1
	using TBU_TS1 or TBU_TS2. Output signal level
	when CCU0 matches is defined by combination
	of SL, ACB10(5) and ACB10(4). On the CCU1
	match the output level is toggled.*/

	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.CMP_CTRL 	= 0; 				/*Greater/equal compare against TBU time base values (TBU_TSx >= CMx)*/
	GTM_KNOCk_MODUL_CHANNEL_CTRL.B.SL 			= 0; 				/*Low Level*/

	//ATOM Global registers settingsl7
	GTM_KNOCk_MODUL_AGC_GLB_CTRL.B.UPEN_C 		= 0x2;
	GTM_KNOCk_MODUL_AGC_ENDIS_CTRL.B.ENDIS_C 	= 0x2;
	GTM_KNOCk_MODUL_AGC_OUTEN_CTRL.B.OUTEN_C 	= 0x2;
	GTM_KNOCk_MODUL_AGC_FUPD_CTRL.B.FUPD_C 		= 0x2;
	GTM_KNOCk_MODUL_AGC_ENDIS_STAT.B.ENDIS_S 	= 0x2;
	GTM_KNOCk_MODUL_AGC_GLB_CTRL.B.HOST_TRIG 	= 1;
	/*Infineon recommand IR*/
	GTM_KNOCk_MODUL_CHANNEL_IRQ_MODE.U 			= 0x2;
	/*CCU0TC and CCU1TC support*/
	GTM_KNOCk_MODUL_CHANNEL_IRQ_EN.U 			= 0X3;
	/* 1<11 => ON CPU1, 1<<10 => Enable*, KNOCK_WINDOW_PRIO INT_PRIO*/
	SRC_KNOCK_WINDOW.U = (1 << 11) | (1 << 10) | KNOCK_WINDOW_PRIO;
	/*connect From ATOM to DSADC*/
	GTM_DSADCOUTSEL10.B.SELGTMTODSADC = KNOCK_CHNNEL;
	/*connect From ATOM to IO*/
	GTM_TOUTSEL2.B.SEL4=2;
	P32_IOCR0.B.PC0=0x11;

}
