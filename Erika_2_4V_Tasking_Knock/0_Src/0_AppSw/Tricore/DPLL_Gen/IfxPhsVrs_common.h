/*
 * \copyright Copyright (C) Infineon Technologies (2010)                                
 *                                                                           
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * IFX License for more details (IFX_License.txt).
 * 
 * This file may be used, copied, and distributed, with or without
 * modification, provided that all copyright notices are retained;
 * that all modifications to this file are prominently noted in the
 * modified file; and that this paragraph is not modified.
 *
 *****************************************************/

/********************************************************************************************!
 *  \file
 *  	IfxPhsVrs_common.h
 *  \date
 *  	24-Jan-2012 5:11:54 PM
 *  \author
 *  	Alfredo Baratta
 *  \brief
 *  	Basic Configuration Parameters for the VRS/PHS driver
 *
 **********************************************************************************************/

#ifndef IFXPHSVR_DRIVER_H
#define IFXPHSVR_DRIVER_H

#include "Ifx_Types.h"
#include "IfxCpu_Intrinsics.h"
#include <IfxPort_reg.h>
#include <IfxGtm_reg.h>
#include <IfxSrc_reg.h>
#include "IfxVrs_Cfg.h"
#include "IfxDpll_ISR.h"

//*****************************************************************************/
//                           Macros                                           */
//*****************************************************************************/

#define GTM_MCS0_MEM  (*( unsigned int *) 0xF0138000u)

#define GTM_DPLL_TS_T       (*(uint32 *) (0xF0128400))
#define GTM_DPLL_TS_T_OLD   (*(uint32 *) (0xF0128404))
#define GTM_DPLL_ADT_T 	    (*(uint32 *) (0xf012C800))
#define GTM_DPLL_ADT_S 	    (*(uint32 *) (0xf0128800))

#define TOOTH_NOT_SET     (0xFF)
#define ENGINE_STOP_TIMEOUT (50)
#define WAIT_START (10000)
#define CMU_CLK1 (1)
#define TIM_TIPM_MODE (3)

#define DEFAULT_SYNC 10
extern uint8 Sync_tooth;



/******************************************************************************/
/*                           Typedefs                                         */
/******************************************************************************/
typedef uint8  IfxVrs_Nteeth;
typedef uint32 IfxVrs_Period;
typedef uint32 IfxVrs_FlywheelAngle;
typedef void (*IfxVrsPhs_setCrankProfileHook)(void);
typedef void (*IfxVrsPhs_setCamProfileHook)(void);
typedef void (*IfxVrsPhs_callbackFuncPtr)(void *);

/******************************************************************************/
/*                           Enums                                            */
/******************************************************************************/
/**!
 *
 */
typedef enum {
	IfxVrs_DiagnosticMode_noFault = 0, ///!< IfxVrs_DiagnosticMode_noFault Normal working mode, Crankshaft used
	IfxVrs_DiagnosticMode_fault,  //!< IfxVrs_DiagnosticMode_fault Emergency mode, Camshaft used
	IfxVrs_DiagnosticMode_scGnd,  //!< IfxVrs_DiagnosticMode_scGnd
	IfxVrs_DiagnosticMode_Ol,     //!< IfxVrs_DiagnosticMode_Ol
	IfxVrs_DiagnosticMode_scVcc,  //!< IfxVrs_DiagnosticMode_scVcc
	IfxVrs_DiagnosticMode_notAv,  //!< IfxVrs_DiagnosticMode_notAv
} IfxVrs_DiagnosticMode;

/*!
 *
 */
typedef enum {
	IfxVrs_SwitchMode_on=0,    //!< IfxVrs_SwitchMode_on
	IfxVrs_SwitchMode_off,     //!< IfxVrs_SwitchMode_off
	IfxVrs_SwitchMode_previous,//!< IfxVrs_SwitchMode_previous
} IfxVrs_SwitchMode;

/*!
 *
 */
typedef enum {
	IfxVrs_Mstatus_notOk=0,   //!< IfxVrs_Mstatus_notOk
	IfxVrs_Mstatus_ok,        //!< IfxVrs_Mstatus_ok
	IfxVrs_Mstatus_parNotOk,  //!< IfxVrs_Mstatus_parNotOk
	IfxVrs_Mstatus_busy,      //!< IfxVrs_Mstatus_busy
	IfxVrs_Mstatus_delayed,   //!< IfxVrs_Mstatus_delayed
	IfxVrs_Mstatus_notEnabled,//!< IfxVrs_Mstatus_notEnabled
	IfxVrs_Mstatus_error,     //!< IfxVrs_Mstatus_error
	IfxVrs_Mstatus_intErr,    //!< IfxVrs_Mstatus_intErr
	IfxVrs_Mstatuse_sMax,     //!< IfxVrs_Mstatuse_sMax
} IfxVrs_Mstatus;

/*!
 *
 */
typedef enum {
	IfxVrs_Bool_false=0,//!< IfxVrs_Bool_false
	IfxVrs_Bool_true,   //!< IfxVrs_Bool_true
} IfxVrs_Bool;

/******************************************************************************/
/*                           Struct                                           */
/******************************************************************************/

typedef struct {
	uint32 capturedtAngle;
	uint32 capturedTime;
	uint32 currentTooth;
	uint32 currentAngleCounter;
} IfxVrs_CallBackParams;


/*!
 *
 */
typedef struct {
	//IfxVrs_DiagnosticMode diagnosticMode; /** indicate if the flywheel signal is in a diagnosis condition or not */
	uint32 reFilterValCh0; /*! Filter Value for TIM_CH0 Rising Edge */
	uint32 feFilterValCh0; /*! Filter Value for TIM_CH0 Falling Edge */
	uint32 reFilterValCh1; /*! Filter Value for TIM_CH1 Rising Edge */
	uint32 feFilterValCh1; /*! Filter Value for TIM_CH1 Falling Edge */
	uint8 crankTeethNr; /*! Number of crankshaft teeth - 1 - HALF_SCALE  */
	uint8 crankCycles;
	uint8 crankActiveEdge;
	uint8 crankMissingTooth;
	uint8 camTeethNr; /*! Number of camshaft teeth - 1 */
	uint8 camCycles;
	uint8 gapHalfScale;
	uint8 camActiveEdge;
	uint32 microTicksTooth; /*microticks generated on every teeth */
	IfxVrsPhs_setCrankProfileHook crankSetupFunc;
	IfxVrsPhs_setCamProfileHook camSetupFunc;
} IfxVrsPhs_Config;



typedef enum {
	IfxVrs_FlywheelStat_noSync = 0,
	IfxVrs_FlywheelStat_preSync,
	IfxVrs_FlywheelStat_waitSync,
	IfxVrs_FlywheelStat_sync,
} IfxVrs_FlywheelStat;

typedef enum {
	IfxVrs_ActiveEdge_noSet = 0,
	IfxVrs_ActiveEdge_raising,
	IfxVrs_ActiveEdge_falling,
	IfxVrs_ActiveEdge_both,
} IfxVrs_ActiveEdge;

typedef enum {
	IfxVrs_FlywheelMode_engineStopped = 0,
	IfxVrs_FlywheelMode_hole,
	IfxVrs_FlywheelMode_run,
	IfxVrs_FlywheelMode_emergency
} IfxVrs_FlywheelMode;

/**
 * @struct IfxVrs_Attributes
 * Defines all the driver specific attributes
 */
typedef struct {
	IfxVrs_FlywheelStat currentState; /*! current driver status */
	IfxVrs_FlywheelMode currentMode; /*! engine status */
	IfxVrs_DiagnosticMode diagnosticMode; /*! Emercency onr Normal Running Mode */
	IfxVrs_SwitchMode enableClc; /*! Teeth Periods buffer enable status*/
	IfxVrs_SwitchMode ignoreStatus; /*! Indicates whether extra teeth have to be waited in NO_SYNC state*/
	IfxVrs_ActiveEdge activeEdge; /*! Trigger Signal Active Slope*/
	IfxVrs_Nteeth toothEventProgrammed; /*!Tooth on which the currect/last one time event has been programmed*/
	//IfxVrs_FlywheelAngle globalTeethOffset; /*!Angle offset used to compute relative angle*/
	IfxVrs_Period tdcPeriod; /*!Last Top Dead Centre period*/
	IfxVrs_Period angleOffset; /*!Last Top Dead Centre period*/
	IfxVrs_Period waitStart; /*!Number of clock cycles to be waited after detecting the first tooth*/
	IfxVrs_Nteeth nMinPresyncTeeth; /*!<teeth number to be waited in PRE_SYNC*/
	IfxVrs_Nteeth firstTDCTooth; /*!First tooth used for the Top Dead Centre period calculation*/
	IfxVrs_Nteeth tdcTeeth; /*!Number of teeth between two TDC [0..HALF_SCALE-1]*/
	IfxVrs_Nteeth nIgnoreTeeh; /*!<teeth number to be waited in NO_SYNC*/
	IfxVrs_Nteeth synchTeeth; /*!<teeth number in PRE_SYNC state*/
	IfxVrs_Nteeth missingTeethCnt; /*!<Missing Teeth found in 1 rotation (other than gap/long teeth)*/
	IfxVrs_Bool isInit; /*! engine running statis status */
	IfxVrs_Bool isDpllLocked; /*! dpll locking status */
	IfxVrs_Bool startAngleSignalFlag; /*!start Angle Enabled*/
	IfxVrs_Bool eventOnToothEn;
	IfxVrs_Nteeth toothInGap;
	IfxVrs_Nteeth volatile cntTeeth; /*!start Angle Enabled*/
	IfxVrs_SwitchMode startAngleErrorStrategy; /*! StartAngleSignal strategy */
	Ifx_GTM_DPLL_STATUS* dpllStatus; /*! pointer to the DPLL_STATUS Register */
	uint32 uTicksToT;
	uint32 uSimTP;
} IfxVrs_Attributes;

typedef struct {
	IfxVrs_SwitchMode enable;
	IfxVrs_Nteeth nCamEdge;    /*! Camshaft flywheel edge counter*/
	//IfxVrs_Nteeth nCamEdgeOld;
	uint32 currCamEdgeAbsAng;  /*! Only for debug*/
	IfxVrs_Nteeth cntTooth;
	IfxVrs_Nteeth resyncOnEgde;
	uint32 resyncOnAngle;
	IfxVrs_Bool teeth;
	IfxVrs_Bool recoveryFlag;
	uint32 simTP;
	IfxVrs_Bool enableEx;
	IfxVrs_DiagnosticMode currentOpMode;
	IfxVrs_Period edgesTimeDistance;
	IfxVrs_FlywheelAngle edgeAngle;
	IfxVrs_FlywheelAngle firstAngle;
	//IfxVrs_FlywheelAngle crankAngle[IFX_PHS_EDGE_NUM];
	IfxVrs_Nteeth edgeNum;
	IfxVrs_Nteeth timUsed;
	uint32 toothDuration;       /*! This variable stores the tooth period calculated by PHS driver */
	uint32 mls1Val;
	IfxVrs_FlywheelAngle globalAngleOffset;

} IfxGtm_PhsParams;



/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/
/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/
//extern IfxVrs_tasIsr(void);
#endif /*IFXPHS*/

