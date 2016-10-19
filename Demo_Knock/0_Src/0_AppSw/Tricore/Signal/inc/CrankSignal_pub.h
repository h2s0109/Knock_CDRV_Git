/********************************************************************************************!
 *  \file
 *  	CrankSignal_pub.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/
#ifndef CRANKSIGNAL_PUB_H_
#define CRANKSIGNAL_PUB_H_

#include "Utility.h"
#include "Crankcfg.h"

/******************************************************************************/
/*                          Macros                                           */
/******************************************************************************/

/******************************************************************************/
/*                           Typedefs                                         */
/******************************************************************************/

/******************************************************************************/
/*                           Enums                                            */
/******************************************************************************/

/******************************************************************************/
/*                           Struct                                           */
/******************************************************************************/

typedef struct
{
	uint32  Period;
	uint32  PeriodMax;
	uint32  PeriodMin;
	uint32  Period_IncreasInc;
	uint32  Period_DecreasInc;
	float  	Dutypercent;
	uint32  Duty;
	uint32  Pattern;
	uint32  GapPeriod;
	uint32  GapDuty;
	boolean CurrentPhase;
	uint8	CurrToothNum1Rev;
	uint8 	CurrToothNum2Rev;
} simulator_st;

/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/
extern const uint32 Cal_EOI[];
extern uint8 Sel_CalEOI;
extern uint32 caltime;
/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/
extern simulator_st CrankValue;
extern void CrankSimulationGen(simulator_st *pCrankValue);

#endif /*CRANKSIGNAL_PUB_H_*/
