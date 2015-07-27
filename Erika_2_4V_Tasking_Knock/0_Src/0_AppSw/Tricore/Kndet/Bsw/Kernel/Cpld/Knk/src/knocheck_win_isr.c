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

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

IFX_INTERRUPT (Knocheck_Window_ISR, 0, 52)
{
	if (GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU0TC == TRUE )
	{
		GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU0TC=1;
		Window_Open_Cnt++;
	}/*End of (GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU0TC == TRUE )*/
	else if(GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU1TC == TRUE)
	{
		flag = IfxCpu_acquireMutex(&resourceLock);
	if (flag){
		GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU1TC=1;
		Window_Close_Cnt++;
		/*Reset process*/
	    uint32  i;
	    Ifx_DMA *psDMA = &MODULE_DMA;
	    Ifx_DSADC *psDSADC = &MODULE_DSADC;
#if (defined(KNOCHECK_TEST) && defined(KNOCHECK_DOUBLE_BUFFERING))
	#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))

	    ISR_Dist_Flag = WINDOW_ISR_FLAG;
		  numBuffer = DMA_CH1_CHCSR.B.BUFFER;
		  Memcopy_Test();
	#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/

	#if defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)
		  FIR_Filter_Test();
		if(P14_OUT.B.P8 == TRUE) /*only for debugging*/
		{
			P14_OMCR.B.PCL8=1;
		}/*only for debugging*/
		else
		{
			P14_OMSR.B.PS8=1;
		}
	#endif /*End of (defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/
#endif
	    for (i = START_NUM_CHANNEL; i <2; i++)
	    {
	    	psDMA->TSR[i].B.DCH =1;
	    	psDMA->TSR[i].B.RST =1;
	    	while(psDMA->TSR[i].B.RST == 1);  /*Should be wait here until RST cleared by HW*/
#ifdef KNOCHECK_TEST
	    	++source[0];
	    	psDMA->CH[i].SADR.U =CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &source[0]);
	#ifdef KNOCHECK_DOUBLE_BUFFERING
			DMA_CH1_CHCSR.B.FROZEN = 0;
			DMA_CH1_DADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination[0]);
			DMA_CH1_SHADR.U= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination_2[0]);
	#endif
#endif
	    	psDMA->TSR[i].B.ECH =1;
	    }
	    IfxCpu_releaseMutex(&resourceLock);
	}
	}/*End of (GTM_ATOM2_CH5_IRQ_NOTIFY.B.CCU1TC == TRUE)*/
}


