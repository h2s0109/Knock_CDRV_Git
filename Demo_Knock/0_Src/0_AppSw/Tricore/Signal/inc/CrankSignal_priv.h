/********************************************************************************************!
 *  \file
 *  	CrankSignal_priv.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/
#ifndef CRANKSIGNAL_PRIV_H_
#define CRANKSIGNAL_PRIV_H_

#include "CrankSignal_pub.h"

/******************************************************************************/
/*                          Macros                                           */
/******************************************************************************/

#define CAL_EOI_DATA 8

/******************************************************************************/
/*                           Typedefs                                         */
/******************************************************************************/

typedef enum
{
	INCREASE =  0,
	DECREASE
};

typedef enum
{
	NORMAL =  0,
	INCDEC
}crankpattern_enum;

typedef enum
{
	BEFORE_GAP_POSITION =  0,
	AFTER_GAP_POSITION,
	NORMAL_POSITION
}position_enum;

/******************************************************************************/
/*                           Enums                                            */
/******************************************************************************/

/******************************************************************************/
/*                           Struct                                           */
/******************************************************************************/

#ifdef CRANK_PULSE
	#ifdef CAM_PULSE
		typedef struct {
			uint8 level;
			uint8 edge;
		} cam_st;
	#endif
#endif

/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/
simulator_st CrankValue;
uint32 caltime;
uint8 cnt_ATOM2_CH0 = 0;
#ifdef CRANK_PULSE
boolean CurrentPhase;
#endif

#ifdef STOP_MODE
boolean Brake = FALSE;
boolean Mode_Stop =  TRUE;
#endif

#ifdef CAM_PULSE
uint8 Start_array[CAM_NUM] = {0,};
cam_st CAM[CAM_NUM][CAM_EDGE_NUM]={{{CAM_LEVEL_HIGH,47},{CAM_LEVEL_LOW,107}},{{CAM_LEVEL_HIGH,47},{CAM_LEVEL_LOW,107}}};
#endif

/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/

static void CrankValue_Init(simulator_st* pCrankValue);
/*ATOM Initialization*/
static void CrankSignalHw_Init(simulator_st* pCrankValue);
/*CMU value setting for Cranksignal generation*/
static void CrankSignal_ClockSetup(void);
/*Count the tooth number*/
static void CrankSignal_ToothCnt(position_enum Gap);
/*Handling Cranksignal pattern*/
static void CrankSignal_Pattern(position_enum gap);
/* Change the Period and Duty before Gap*/
/* Generate missing tooth*/
static void PeriodDutyGap(simulator_st *pCrankValue);
/* Change the Period and Duty except before Gap*/
/* Generate Normal tooth*/
static void PeriodDuty(simulator_st *pCrankValue);

static void ConvPulse_Cal(simulator_st *pCrankValue);

#ifdef STOP_MODE
	static void Crank_Signal_Brake(void);
#endif

#ifdef CAM_PULSE
	static void CamSignalHw_Init(void );
	static void CamSignalStartpoint(void);
	static void CamSignal_Handle(void);
#endif

#endif /*CRANKSIGNAL_PRIV_H_*/
