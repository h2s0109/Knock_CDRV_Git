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
#include "knocheck_ATest_Switch.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

typedef struct
{
	uint16 MeasWndIntgCnt; ///< �ֱ� Measure Window���� KDF ����� Integration �� Ƚ��
	sint16 DiagEvalValueAtMeasWndBeginSignalA; ///< Signal A at measure window begin : 0 when FAINxP=Vref/2, Positive when FAINxP>Vref/2, Negative Otherwise, 32767 when FAINxP=Vref/2+Vref, -32767 when FAINxP=Vref/2-Vref
	sint16 DiagEvalValueAtMeasWndBeginSignalB; ///< Signal B at measure window end : 0 when FAINxN=Vref/2, Positive when FAINxN>Vref/2, Negative Otherwise, 32767 when FAINxN=Vref/2+Vref, -32767 when FAINxN=Vref/2-Vref
	boolean b_AddMeasureWindowLength;
} Knk_DiagDataT;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
extern void Knocheck_Ini(void);
extern void KnoCheck_Dma_Ini(void);
extern void KnoCheck_Dsadc_Ini(void);
extern void KnoWindow_Output_Ini(void);


#ifdef KNOCHECK_TEST
	extern void En_Test(void);
	extern void SetCtrlData_Test(void);
	extern void UpdateMeasureWindow_Test(void);
#endif

#endif /* KNOCHECK_PUB_H_ */
