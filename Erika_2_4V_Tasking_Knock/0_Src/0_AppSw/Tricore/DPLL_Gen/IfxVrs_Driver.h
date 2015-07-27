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
 *  	IfxVrs_Driver.h
 *  \date
 *  	24-Jan-2012 5:11:54 PM
 *  \author
 *  	Alfredo Baratta
 *  \brief
 *  	Basic Implementation of the VRS - Crankshaft Management Driver for AURIX/GTM
 *
 **********************************************************************************************/

#ifndef IFXVRS_DRIVER_H
#define IFXVRS_DRIVER_H

#include "IfxPhsVrs_common.h"
#include "Ifx_Utility.h"
/*********************************************************************************/

/*********************************************************************************
 Macros                                              *
 ********************************************************************************/
#define IFXVRS_MIN_TEETH_TO_WAIT (3)
#define TIME_PRIO (0)
#define ANGLE_PRIO (1)

/*********************************************************************************
 Typedefs
 ********************************************************************************/

/*********************************************************************************
 Enums                                              *
 *******************************************************************************/

/******************************************************************************/
/**                           Struct                                           */
/******************************************************************************/

/******************************************************************************
 *                           Variables                                        */
/******************************************************************************/
extern IfxVrs_Attributes vrsAttributes;
extern IfxGtm_PhsParams ifxPhs_parameters;
/******************************************************************************
 /                           Function prototypes                               */
/******************************************************************************/

void IfxVrs_start(void);
IfxVrs_Mstatus IfxVrs_init(void );
IfxVrs_Nteeth IfxVrs_setNMinPresyncTeeth(IfxVrs_Nteeth teethNumbers);
void Set_Ignore_Tooth(IfxVrs_Nteeth teethNumbers);
IfxVrs_Mstatus IfxVrs_setWaitStart(IfxVrs_Period waitTime);
IfxVrs_Period IfxVrs_toothPeriod(IfxVrs_Mstatus *);
IfxVrs_Nteeth IfxVrs_setEventOnTooth(IfxVrs_Nteeth toothNo);
IfxVrs_FlywheelStat IfxVrs_getFlywheelStat(void);
uint32 IfxVrs_setActiveEdge(short activeEdge);

uint32 IfxVrs_setAngleOffset(uint32 angleValue);
uint32 IfxVrs_getLastTs(void);
uint32 IfxVrs_getAvrRpm(void);
uint32 IfxVrs_getIstRpm(void);
IfxVrs_Mstatus IfxVrs_recoveryResync(uint32 angle, uint32 TP);


/******************************************************************************/
#endif /*EP_DRIVER_H*/
