/*
 * knocheck_priv.h
 *
 *  Created on: 2015. 2. 27.
 *      Author: hansteve
 */

#ifndef KNOCHECK_PRIV_H_
#define KNOCHECK_PRIV_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Ifx_Types.h"                 /* Do not remove this include */
#include "knocheck_pub.h"

#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))
	#include "Knk_Asw.h"
#endif


#include "IfxCpu_reg.h"
#include "IfxPort_reg.h"
#include "IfxGtm_reg.h"

#include "knocheck_dma_def.h"
#include "knocheck_dsadc_def.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))

	typedef enum
	{
		DMA_ISR_FLAG       	= 0,
		WINDOW_ISR_FLAG 	= 1
	} KNOCHECK_ISR_FLAG;

	typedef enum
	{
		BUFFER0       	= 0,
		BUFFER1 		= 1
	} BUFFER_FLAG;

	typedef enum
	{
		BUFFER0_DMA 		= 1111,
		BUFFER1_DMA 		= 2222,
		BUFFER0_KNWINDOW 	= 3333,
		BUFFER1_KNWINDOW 	= 4444,
	} BUFFER_ISR_FLAG;

#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/

#if (defined(SETCTRL_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))

	typedef enum
	{
		NORMAL       		= 0,
		FILTER_INDICES_OVER = 1,
		FILTERSET_OVER 		= 2,
		CHANNEL_OVER 		= 4
	} FILTER_SEL_EXP;

#endif /*End of (defined(SETCTRL_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/
/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

#ifdef KNOCHECK_TEST
	extern uint32 default_value;
	extern volatile sint16 source[NUM_CHANNELS];
	extern volatile sint16 destination[BUFFER_SIZE];
	#ifdef KNOCHECK_DOUBLE_BUFFERING
		extern volatile sint16 destination_2[BUFFER_SIZE];
	#endif
#endif

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
extern Cpu_ResourceCpu Cpu_getCoreId(void);
/*************************************	*****************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

#if defined(TOTAL_FILTER_TEST) || defined(SETCTRL_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)
	extern KnoCirBuff Knocheck_FiltInfo[KNK_CH_LENGTH][KNK_FILTERSET_SIZE];
#endif /*End of defined(TOTAL_FILTER_TEST) || defined(SETCTRL_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)*/

#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))
	extern KNOCHECK_ISR_FLAG ISR_Dist_Flag;
	extern uint8 numBuffer;
#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#endif /* KNOCHECK_PRIV_H_ */
