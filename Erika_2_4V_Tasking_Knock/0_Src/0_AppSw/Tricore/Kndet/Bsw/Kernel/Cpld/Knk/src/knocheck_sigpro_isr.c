/*
 * knocheck_SigPro_ISR.c
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
	uint32 DMA_ISR_Cnt;
#endif

/*End of KNOCHECK_TEST*/
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/


IFX_INTERRUPT (DMA_FULL_ISR, 0, KNOCHECK_DMA_PRIO)
{
	/*only for debugging*/
	if(P14_OUT.B.P2 == TRUE)
	{
		P14_OMCR.B.PCL2=1;
	}
	else
	{
		P14_OMSR.B.PS2=1;
	}
	KnoCheck_IgnDataCnt++;
	Ifx_DMA *psDMA = &MODULE_DMA;

	if(KnoCheck_IgnDataCnt<3)
	{
		psDMA->CH[DMA_CHANNEL_SELECT].CHCFGR.B.TREL = PRE_BUFFER_SIZE;
		psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.FROZEN = 0;
	}
	else if(KnoCheck_IgnDataCnt>=3)
	{
		psDMA->CH[DMA_CHANNEL_SELECT].CHCFGR.B.TREL = BUFFER_SIZE;
		psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.FROZEN = 0;
	}

	KnoCheck_NumBuffer = psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.BUFFER;
	if (KnoCheck_NumBuffer == BUFFER1)
	{
		psDMA->CH[DMA_CHANNEL_SELECT].DADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer0_Dest[0]);
	}
	else
	{
		psDMA->CH[DMA_CHANNEL_SELECT].SHADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer1_Dest[0]);
	}

	if(KnoCheck_IgnDataCnt == 3||KnoCheck_IgnDataCnt == 4)
	{
		KnoCheck_IsrDistFlg = DMA_ISR_PRE_FLAG;
		#if(FINAL == CONTROL_EN)
		KnoCheck_Memcopy();
		KnoCheck_Filter();
		#elif(FINAL == CONTROL_DIS)
		Memcopy_Test();
		FIR_Filter_Test();
		#endif
	}
	else if(KnoCheck_IgnDataCnt>=5)
	{
		KnoCheck_IsrDistFlg = DMA_ISR_FLAG;
		#if(FINAL == CONTROL_EN)
		KnoCheck_Memcopy();
		KnoCheck_Filter();
		#elif(FINAL == CONTROL_DIS)
		Memcopy_Test();
		FIR_Filter_Test();
		#endif
	}

	/*only for debugging*/
	if(P14_OUT.B.P8 == TRUE)
	{
		P14_OMCR.B.PCL8=1;
	}
	else
	{
		P14_OMSR.B.PS8=1;
	}
	psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.CICH = 1;
#ifdef KNOCHECK_TEST
	DMA_ISR_Cnt++;
#endif
}
/*End of DMA_Count_ISR*/

