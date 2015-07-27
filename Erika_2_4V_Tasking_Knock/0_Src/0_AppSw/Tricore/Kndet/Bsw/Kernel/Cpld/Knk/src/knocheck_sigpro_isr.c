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
IfxCpu_mutexLock resourceLock;
boolean flag;
#endif

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#pragma section code ".text.cpu1_psram"
IFX_INTERRUPT (DMA_Count_ISR, 0, 57)
{

	flag = IfxCpu_acquireMutex(&resourceLock);
if (flag){

#ifdef KNOCHECK_DOUBLE_BUFFERING
	DMA_CH1_CHCSR.B.FROZEN = 0;
	numBuffer = DMA_CH1_CHCSR.B.BUFFER;
	if (DMA_CH1_CHCSR.B.BUFFER == BUFFER1)
	{
		DMA_CH1_DADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination[0]);
	}
	else
	{
		DMA_CH1_SHADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination_2[0]);
	}
#endif /*End of KNOCHECK_DOUBLE_BUFFERING*/


#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))
	  ISR_Dist_Flag = DMA_ISR_FLAG;
	  Memcopy_Test();
#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/


#if defined(SIMPLE_FILTER_DOUBLEBUF_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)
	  FIR_Filter_Test();
#endif /*End of (defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/

#ifdef BASIC_FILTER_TEST
	   FIR_Filter_Test();
#endif /*End of BASIC_FILTER_TEST*/

#ifdef DYNAMIC_DATA_FILTER_TEST
	  Memcopy_Test();
	  FIR_Filter_Test();
#endif /*End of DYNAMIC_DATA_FILTER_TEST*/

#ifdef TOTAL_FILTER_TEST
	  Filter_Selection();
	  Memcopy_Test();
	  FIR_Filter_Test_Total();
#endif /*End of TOTAL_FILTER_TEST*/
	  DMA_CH1_CHCSR.B.CICH = 1;
	  DMA_ISR_Cnt++;
	  IfxCpu_releaseMutex(&resourceLock);
}
}
