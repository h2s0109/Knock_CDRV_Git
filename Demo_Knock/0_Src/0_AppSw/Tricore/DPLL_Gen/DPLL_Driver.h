/********************************************************************************************!
 *  \file
 *  	DPLL_Driver.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/

#ifndef DPLL_DRIVER_H_
#define DPLL_DRIVER_H_

#include "Utility.h"
#include "DPLL_Cfg.h"

//*****************************************************************************/
//                           Macros                                           */
//*****************************************************************************/

/******************************************************************************/
/*                           Typedefs                                         */
/******************************************************************************/
typedef uint8  CrankTeethNum;
typedef uint32 IfxVrs_Period;
/******************************************************************************/
/*                           Enums                                            */
/******************************************************************************/
typedef enum {
	NO_SYNC = 0,
	PRE_SYNC,
	WAIT_SYNC,
	FULL_SYNC,
} CrankSyncState;

typedef enum {
	ACTIVE_NOSET = 0,
	ACTIVE_RAISING,
	ACTIVE_FALLING,
	ACTIVE_BOTH,
}Active_Edge;

typedef enum {
	ENGINE_STOPPED = 0,
	ENGINE_HOLE,
	ENGINE_RUN,
	ENGINE_EMERGENCY
}CrankMode;
/******************************************************************************/
/*                           Struct                                           */
/******************************************************************************/


typedef struct {
	CrankSyncState currentState; /*! current driver status */
	CrankMode currentMode; /*! engine status */
	IfxVrs_Period waitStart; /*!Number of clock cycles to be waited after detecting the first tooth*/
	CrankTeethNum toothEventProgrammed; /*!Tooth on which the currect/last one time event has been programmed*/
	CrankTeethNum nMinPresyncTeeth; /*!<teeth number to be waited in PRE_SYNC*/
	CrankTeethNum firstTDCTooth; /*!First tooth used for the Top Dead Centre period calculation*/
	CrankTeethNum tdcTeeth; /*!Number of teeth between two TDC [0..HALF_SCALE-1]*/
	CrankTeethNum nIgnoreTeeh; /*!<teeth number to be waited in NO_SYNC*/
	CrankTeethNum synchTeeth; /*!<teeth number in PRE_SYNC state*/
	CrankTeethNum missingTeethCnt; /*!<Missing Teeth found in 1 rotation (other than gap/long teeth)*/
	CrankTeethNum toothInGap;
	CrankTeethNum volatile cntTeeth; /*!start Angle Enabled*/
	boolean isInit; /*! engine running statis status */
	boolean isDpllLocked; /*! dpll locking status */
	boolean startAngleSignalFlag; /*!start Angle Enabled*/
	boolean eventOnToothEn;
	Ifx_GTM_DPLL_STATUS* dpllStatus; /*! pointer to the DPLL_STATUS Register */
} CrankState_struct;

/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/

extern boolean SignalGenComplete;
extern uint32 *pmem_ADT_Tx;
extern volatile uint32 *pmem_TSF_T;
extern volatile uint32 *pmem_RDT_T;
extern volatile uint32 *DT_Ti;
extern uint8 simulateGap;
extern uint8 Sync_tooth;
extern CrankState_struct CrankState;

/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/
void Reset_Procedure(void);
void DPLL_Clock_Init(void);
void DPLL_Init (void);
void DPLL_IR_Init (void);
void CrankInput_Init(void);
void CrankState_Init(void);
void CrankProfile_Init (void);
void Crank_Start(void);
void Auxiliary_ATOM40_Init(void);
void Set_IgnoreTooth(CrankTeethNum teethNumbers);
void Set_WaitEndTime(IfxVrs_Period waitTime);
void Set_NMinPresyncTeeth(CrankTeethNum teethNumbers);
void Set_EventOnTooth(CrankTeethNum toothNo);
void Timeout_Detect_Mode(void);
CrankSyncState Get_CrankState(void);
uint32 Get_InstantRpm(void);

#endif /*DPLL_DRIVER_H_*/
