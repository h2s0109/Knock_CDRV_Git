/*
 * knocheck_Win_ISR.c
 *
 *  Created on: 2015. 5. 7.
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
#ifdef KNOCHECK_TEST
	uint32 Window_Open_Cnt;
	uint32 Window_Close_Cnt;
#endif

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/
#ifdef KNOCHECK_TEST
	sint32 Test_Integrated[KNK_PARALLELFILTER_SIZE];
#endif
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
#pragma section code "cpu1_psram"
IFX_INTERRUPT (Knocheck_Window_ISR, 0, KNOCK_WINDOW_PRIO)
{
	/*uint32  cntChan;*/
	if (GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU0TC == TRUE )
	{
		GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU0TC=1;
#ifdef KNOCHECK_TEST
		Window_Open_Cnt++;
#endif
	}
	/*End of (GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU0TC == TRUE )*/
	else if(GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU1TC == TRUE)
	{
		GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU1TC=1;
#ifdef KNOCHECK_TEST
		Window_Close_Cnt++;
#endif

		Ifx_DMA *psDMA = &MODULE_DMA;
		if((uint16)psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.TCOUNT != BUFFER_SIZE)
		{
			KnoCheck_IsrDistFlg = WINDOW_ISR_FLAG;
			KnoCheck_NumBuffer = psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.BUFFER;
			#if(FINAL==CONTROL_EN)
			KnoCheck_Memcopy();
			KnoCheck_Filter();
			#elif(FINAL==CONTROL_DIS)
			Memcopy_Test();
			FIR_Filter_Test();
			#endif
			/*End of #if(FINAL==CONTROL_EN)*/
		}
			#if(FINAL==CONTROL_EN)
			KnoCheck_ClearCoeffDly();
			#endif

			/*Reset process*/
	    	psDMA->TSR[DMA_CHANNEL_SELECT].B.DCH =1;
	    	psDMA->TSR[DMA_CHANNEL_SELECT].B.RST =1;
	    	while(psDMA->TSR[DMA_CHANNEL_SELECT].B.RST == 1);  /*Should be wait here until RST cleared by HW*/
	    	psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.FROZEN = 0;
	    	psDMA->CH[DMA_CHANNEL_SELECT].DADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer0_Dest[0]);
	    	psDMA->CH[DMA_CHANNEL_SELECT].SHADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer1_Dest[0]);
	    	psDMA->CH[DMA_CHANNEL_SELECT].CHCFGR.B.TREL = 10;
	    	KnoCheck_IgnDataCnt = 0;
#ifdef KNOCHECK_TEST
	    	cntDATASET = 0;
	    	++TestCntsource[0];
	    	//psDMA->CH[DMA_CHANNEL_SELECT].SADR.U =CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &TestCntsource[0]);
	    	psDMA->CH[DMA_CHANNEL_SELECT].SADR.U =CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &DSADC_RESM);
	    	Knk_GetIntgDataEx(Knocheck_Ready_Chan,Test_Integrated,3);
#else
	    	psDMA->CH[DMA_CHANNEL_SELECT].SADR.U =CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &DSADC_RESM);
#endif
			if((Knocheck_Ready_Chan < (MAX_CHANNEL-1))&&((MAX_CHANNEL-1)>0))
			{
				Knocheck_Ready_Chan++;
			}
			else
			{
				Knocheck_Ready_Chan = 0;
			}
	}
	/*End of (GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY.B.CCU1TC == TRUE)*/
}
/*End of IFX_INTERRUPT (Knocheck_Window_ISR, 0, 52)*/

#pragma section code restore
