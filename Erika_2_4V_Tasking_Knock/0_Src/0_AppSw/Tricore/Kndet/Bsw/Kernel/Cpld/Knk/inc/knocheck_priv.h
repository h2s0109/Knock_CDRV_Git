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
#include "Ifx_Types.h"			/* will be changed to HMC header*/
#include "IfxCpu_Intrinsics.h"	/* will be changed to HMC header*/
#include "IfxScuWdt.h"			/* will be changed to removed*/
#include "IfxSrc_reg.h"			/* will be changed to removed*/
#include "IfxCpu_reg.h"
#include "IfxPort_reg.h"
#include "IfxGtm_reg.h"

#include "knocheck_config.h"
#include "knocheck_pub.h"
#include "knocheck_def.h"
#include "knocheck_dma_def.h"
#include "knocheck_dsadc_def.h"
#include "dsplib.h"
#include "Knk_Asw.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define ABS(X)                       ( ((X) < 0) ? -(X) : (X) )
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
	extern volatile sint16 TestCntsource[MAX_CHANNEL];
	extern uint8 FilterIndices[KNK_PARALLELFILTER_SIZE];
	extern uint8 Errortrace;
	extern uint8 SwitchWin;
	extern uint8 Enable_control;
	extern uint8 SwithEnDis;
	extern sint16 cntDATASET;
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

#if(FINAL==CONTROL_EN)
	#pragma section neardata
	#pragma align 8
	extern sint16 Knocheck_DestCopyRam[BUFFER_SIZE];
	#pragma align 8
	extern sint16 Knocheck_FilterResult[KNK_PARALLELFILTER_SIZE][BUFFER_SIZE];
	extern uint16 Knocheck_Integrated[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	extern uint16 Knocheck_Integr[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	extern uint16 Knocheck_NumIputData;
	#pragma section neardata restore
#elif(FINAL==CONTROL_DIS)
	#define DATA_TEST_COUNT  50
	#define BUFFER_EXT_POSITION_1 	BUFFER_SIZE
	#define BUFFER_EXT_POSITION_2 	BUFFER_SIZE+1
	#define BUFFER_PLUS_TWO 		BUFFER_SIZE+2
	#define ONLY_ONE 				1
	#define ONLY_ONE_ARRAY 			0
	extern sint16 cntDATASET;
	extern uint32 cntDATA;
	extern uint16 Remain_Data_Num[];
	extern uint16 Knocheck_NumIputData[];
	extern uint16 temp_Data_Num;
	extern uint16 Knocheck_NumIputData_last;
	/* Input: Data in 1Q15 */
	#pragma section neardata
	#pragma align 8
	extern sint16 Knocheck_DestCopyRam[DATA_TEST_COUNT][BUFFER_PLUS_TWO];
	#pragma section neardata restore
	#pragma align 8
	extern sint16 Knocheck_FilterResult[DATA_TEST_COUNT][KNK_PARALLELFILTER_SIZE][BUFFER_PLUS_TWO];
	extern uint16 Knocheck_Integrated[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	extern uint16 Knocheck_Integr[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
#endif
	/*End of #if(FINAL==CONTROL_EN)*/
	extern sint16 KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNK_KDF_ORDER];
	extern sint16  Coeff_Dly[KNK_PARALLELFILTER_SIZE][KNK_KDF_ORDER];
	extern KNOCHECK_CIRBUFF cptr_Coeff;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_0;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_1;
	extern KNOCHECK_CIRBUFF cptr_Coeff_Dly_2;
/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
	extern Cpu_ResourceCpu Cpu_getCoreId(void);
#if(FINAL==CONTROL_EN)
	extern void KnoCheck_Filter(void);
	extern void KnoCheck_Memcopy(void);
	extern void KnoCheck_ClearCoeffDly(void);
	extern void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes);
#elif(FINAL==CONTROL_DIS)
	extern void FIR_Filter_Test(void);
	extern void Memcopy_Test(void);
#endif
/*End of #if(FINAL==CONTROL_EN)*/

/*************************************	*****************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/




/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

#endif /* KNOCHECK_PRIV_H_ */
