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
#include "dsplib.h"
#include "knocheck_config.h"
#include "knocheck_def.h"
#include "knocheck_dma_def.h"
#include "knocheck_dsadc_def.h"
#include "knocheck_pub.h"
#include "Ifx_Types.h"			/* will be changed to HMC header*/
#include "IfxCpu_Intrinsics.h"	/* will be changed to HMC header*/
#include "IfxScuWdt.h"			/* will be changed to removed*/
#include "IfxSrc_reg.h"			/* will be changed to removed*/
#include "IfxCpu_reg.h"
#include "IfxPort_reg.h"
#include "IfxGtm_reg.h"


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define ABS(X)                       ( ((X) < 0) ? -(X) : (X) )
#define KNOCK_CH_1						0		///< Knock Channel 1
#define KNK_CH_LENGTH				4		///< Max channel
#define kNOCHECK_CH_DISABLE				0		///< Disable
#define kNOCHECK_CH_ENABLE				1		///< Enable

#define kNOCHECK_GAIN1					1		///< Knock Gain 1
#define kNOCHECK_GAIN2					2		///< Knock Gain 2
#define kNOCHECK_GAIN4					4		///< Knock Gain 4
#define kNOCHECK_GAIN8					8		///< Knock Gain 8

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
	extern uint8 FilterIndices[KNK_PARALLELFILTER_SIZE];
	extern uint8 Errortrace;
#endif
	extern volatile sint16 Buffer0_Dest[BUFFER_SIZE];
	extern volatile sint16 Buffer1_Dest[BUFFER_SIZE];
	extern uint8 KnoCheck_NumBuffer;
	extern uint8 KnoCheck_IgnDataCnt;
	extern uint8 Knocheck_Ready_Chan;
	extern uint8 KnoCheck_RunChan;
	extern uint8 KnoCheck_RunFiltNum;
	extern uint8 KnoCheck_Gain[MAX_CHANNEL];
	extern uint16 KnoCheck_FirNumInput;
	extern KNOCHECK_ISR_FLAG KnoCheck_IsrDistFlg;
	extern KNOCHECK_CONVAR_T KnoCheck_WinCon[MAX_CHANNEL];
	extern KNOCHECK_CIRBUFF_T Knocheck_FiltInfo[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];

	#pragma section neardata
	#pragma align 8
	extern sint16 Knocheck_DestCopyRam[BUFFER_SIZE];
	#pragma align restore
	extern sint16 Knocheck_FilterResult[KNK_PARALLELFILTER_SIZE][BUFFER_SIZE];
	extern sint32 Knocheck_Integrated[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	extern sint16 Knocheck_Integr[KNK_PARALLELFILTER_SIZE][BUFFER_SIZE];
	extern uint16 Knocheck_NumIputData;
	#pragma section neardata restore

	extern sint16 KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNOCHECK_FILTER_ORDER];
	extern sint16  Coeff_Dly[KNK_PARALLELFILTER_SIZE][KNOCHECK_FILTER_ORDER];
	extern KNOCHECK_CIRBUFF cptr_Coeff;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_0;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_1;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_2;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
	extern Cpu_ResourceCpu Cpu_getCoreId(void);
	extern void KnoCheck_Filter(void);
	extern void KnoCheck_Memcopy(void);
	extern void KnoCheck_ClearCoeffDly(void);
	extern void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes);
	extern void knoCheck_Enable(uint8 Ch, uint8 Enable);
	extern void knoCheck_UpdateMeasureWin(uint8 Ch, uint16 StartAngle, uint16 EndAngle);
	extern void knoCheck_Control(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength);
	extern void knoCheck_GetIntegData(uint8 Ch, sint32* IntegratedValues,uint8 ValueLength);
/*************************************	*****************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/




/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#endif /* KNOCHECK_PRIV_H_ */
