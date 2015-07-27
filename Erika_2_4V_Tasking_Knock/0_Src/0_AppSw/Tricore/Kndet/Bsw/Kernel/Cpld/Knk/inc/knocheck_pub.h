/*
 * knocheck_pub.h
 *
 *  Created on: 2015. 2. 26.
 *      Author: hansteve
 */

#ifndef KNOCHECK_PUB_H_
#define KNOCHECK_PUB_H_
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Ifx_Types.h"
#include "knocheck_ATest_Switch.h"
#include "IfxCpu.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define START_NUM_CHANNEL 1
#define NUM_CHANNELS 2
//#define BUFFER_SIZE  250

#ifdef DOUBLE_BUFFRING_TEST
	#define BUFFER_SIZE  28
	#define TRANSFER_COUNT BUFFER_SIZE-7
#endif/*End of DOUBLE_BUFFRING_TEST*/

#if (defined(SIMPLE_FILTER_DOUBLEBUF_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST))
	#define BUFFER_SIZE  150
#endif/*End of (defined(SIMPLE_FILTER_DOUBLEBUF_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST))*/

#if defined(CHANNEL_FILTER_SELECT_TEST)
	#define BUFFER_SIZE  30
	#define KNK_FILTERSET_SIZE			3		///< Max filter set

	#ifdef REAL_FILTER_NUM_TEST
		#define KNK_KDF_ORDER				48
	#else
		#define KNK_KDF_ORDER				16
	#endif
extern IfxCpu_mutexLock resourceLock;
extern boolean flag;
#endif/*End of CHANNEL_FILTER_SELECT_TEST*/

#if defined(CHANGE_FILTER_DOUBLEBUF_TEST)
	/*These defines come from "Knk_Runtime.h"*/
	#define KNK_FILTERSET_SIZE			4		///< Max filter set
	#define KNK_KDF_ORDER				16
#endif

#if (defined(DOUBLE_BUFFRING_TEST)|| defined(SIMPLE_FILTER_DOUBLEBUF_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))
	#define BUFFER_EXT_POSITION_1 BUFFER_SIZE
	#define BUFFER_EXT_POSITION_2 BUFFER_SIZE+1
	#define BUFFER_PLUS_TWO BUFFER_SIZE+2
	#define ONLY_ONE 1
	#define ONLY_ONE_ARRAY 0
#endif/*End of (defined(DOUBLE_BUFFRING_TEST)|| defined(SIMPLE_FILTER_DOUBLEBUF_TEST)|| defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/



/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
extern void KnoCheck_Dma_Ini(void);
extern void KnoCheck_Dsadc_ChanEnDis(uint8 Ch, uint8 Enable);
extern void KnoCheck_DSADC_Ini(void);
#ifdef KNOCHECK_DSADC_TEST
	extern void KnoCheck_DSADC_Demo(void);
#endif
extern void KnoWindow_Output_Ini(void);

#ifdef BASIC_FILTER_TEST
	extern void FIR_Filter_Test(void);
#endif /*End of BASIC_FILTER_TEST*/

#ifdef DYNAMIC_DATA_FILTER_TEST
	extern sint16 destination_copy[];
	extern sint16 destination[];
	extern void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes);
	extern void Memcopy_Test(void);
	extern void FIR_Filter_Test(void);
#endif /*End of HMC_FILTER_TEST*/

#ifdef SETCTRL_TEST

	#define KNK_CH1						0		///< Knock Channel 1
	#define KNK_CH2						1		///< Knock Channel 2
	#define KNK_CH3						2		///< Knock Channel 3
	#define KNK_CH4						3		///< Knock Channel 4
	#define KNK_CHX						7		///< Reserved
	#define KNK_CH_LENGTH				4		///< Max channel
	#define KNK_FILTERSET_SIZE			11		///< Max filter set
	#define KNK_PARALLELFILTER_SIZE		5		///< Parallel filter selection per ch, must be more than 0
	#define KNK_KDF_ORDER				52

	extern uint8 filter_sel_switch;
	extern uint8 FilterIndices[KNK_PARALLELFILTER_SIZE];
	extern void SetCtrl_selection(void);
	extern void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl);
#endif /*End of SETCTRL_TEST*/

#ifdef TOTAL_FILTER_TEST

	#define KNK_CH1						0		///< Knock Channel 1
	#define KNK_CH2						1		///< Knock Channel 2
	#define KNK_CH3						2		///< Knock Channel 3
	#define KNK_CH4						3		///< Knock Channel 4
	#define KNK_CHX						7		///< Reserved
	#define KNK_CH_LENGTH				4		///< Max channel
	#define KNK_FILTERSET_SIZE			2		///< Max filter set
	#define KNK_PARALLELFILTER_SIZE		5		///< Parallel filter selection per ch, must be more than 0

	#if (FILTER_NUM == 64)
	#define KNK_KDF_ORDER				64
		#define KNK_KDF_ORDER				16
	#elif (FILTER_NUM == 16)
		#define KNK_KDF_ORDER				16
	#endif

	extern uint8 FilterIndices[KNK_PARALLELFILTER_SIZE];
	//extern KnoCirBuff Knocheck_filter_Total[KNK_CH_LENGTH];
	extern void SetCtrlData_Total_Filter_Test(void);
	extern void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl);
	extern void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes);
	extern void Memcopy_Test(void);
	extern void FIR_Filter_Test_Total(void);
	extern void Filter_Selection(void);
#endif /*End of TOTAL_FILTER_TEST*/

#if defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)
	/*These defines come from "Knk_Runtime.h"*/
	#define KNK_PARALLELFILTER_SIZE		3		///< Parallel filter selection per ch, must be more than 0
#endif /*End of defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST)*/

	/*These defines come from "Knk_Runtime.h"*/
	extern uint8 Filter_select;
	extern uint8 FilterIndices[KNK_PARALLELFILTER_SIZE];
	extern void FIR_Filter_Test(void);
	extern void SetCtrlData_Test(void);

#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST))
	/*These defines come from "Knk_Runtime.h"*/
	#define KNK_FILTERSET_SIZE			11		///< Max filter set
	#define KNK_PARALLELFILTER_SIZE		3		///< Parallel filter selection per ch, must be more than 0
	#define KNK_KDF_ORDER				16
#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST))*/

#if (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))

	typedef sint16 Knk_DspDataT;
	typedef uint16 Knk_DspDataHexT;
	typedef struct
	{
		void  *base;           /**< \brief buffer base address */
		uint16 index;                  /**< \brief buffer current index */
		uint16 length;                 /**< \brief buffer length*/
	} Knocheck_CirBuffT;

	typedef struct {
		uint16 MeasWndIntgCnt; ///< 최근 Measure Window에서 KDF 결과를 Integration 한 횟수
		sint16 DiagEvalValueAtMeasWndBeginSignalA; ///< Signal A at measure window begin : 0 when FAINxP=Vref/2, Positive when FAINxP>Vref/2, Negative Otherwise, 32767 when FAINxP=Vref/2+Vref, -32767 when FAINxP=Vref/2-Vref
		sint16 DiagEvalValueAtMeasWndBeginSignalB; ///< Signal B at measure window end : 0 when FAINxN=Vref/2, Positive when FAINxN>Vref/2, Negative Otherwise, 32767 when FAINxN=Vref/2+Vref, -32767 when FAINxN=Vref/2-Vref
		boolean b_AddMeasureWindowLength;
	} Knk_DiagDataT;

	#define KnoCirBuff Knocheck_CirBuffT
	extern void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl);
	extern void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes);
	extern void Memcopy_Test(void);
	extern void Filter_Selection(void);
#endif /*End of (defined(DOUBLE_BUFFRING_TEST) || defined(SIMPLE_FILTER_DOUBLEBUF_TEST) || defined(CHANGE_FILTER_DOUBLEBUF_TEST) || defined(CHANNEL_FILTER_SELECT_TEST))*/

#endif /* KNOCHECK_PUB_H_ */
