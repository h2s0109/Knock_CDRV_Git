/********************************************************************************************!
 *  \file
 *  	Ifx_Utility.h
 *  \date
 *  	24-Jan-2012 5:11:54 PM
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *  	Basic Configuration Parameters for the VRS/PHS driver
 *
 **********************************************************************************************/

#ifndef IFXUTILITY_H
#define IFXUTILITY_H

#include "Ifx_Types.h"
#include "IfxCpu_Intrinsics.h"
#include "Ifx_reg.h"
#include "IfxPhsVrs_common.h"


//*****************************************************************************/
//                           Macros                                           */
//*****************************************************************************/
//#define DPLL
#define CLK_CNT_1ms (99999)
#define CLK_CNT_100ns (9)
#define CLK_EN  (2)
#define CLK_DIS (1)
#define CLK_ENABLED  (3)
#define PULSE_NOTIFY (2)
#define LOW_LES  (1)

#define IRQ_DISABLE (0)
#define IRQ_ENABLE (1)

#define IRQ_ACK (1)

#define RISING_EDGE (1)
#define FALLING_EDGE (2)
#define BOTH_EDGE (3)

/*TIM Operative Modes */
#define TPWM               0
#define TPIM               1
#define TIEM               2
#define TIPM               3
#define TIEM_RF            20
#define TIEM_R             21
#define TIEM_F             22
#define TIPM_RF            30
#define TIPM_R             31
#define TIPM_F             32

/******************************************************************************/
/*                           Typedefs                                         */
/******************************************************************************/

/******************************************************************************/
/*                           Enums                                            */
/******************************************************************************/

/******************************************************************************/
/*                           Struct                                           */
/******************************************************************************/


/******************************************************************************/
/*                           Variables                                        */
/******************************************************************************/

extern boolean Init_Complete;
extern IfxVrs_Attributes vrsAttributes;
extern uint32 firstGap;
extern uint32 *pmem_ADT_Tx;
extern volatile uint32 *pmem_TSF_T;
extern volatile uint32 *pmem_RDT_T;
extern volatile uint32 *DT_Ti;
extern uint8 simulateGap;


/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/
void Reset_Procedure(void);
void DPLL_Clock_Ini(void);
void Dpll_Ini (void);
void Dpll_Interrupt_Ini (void);
void Signal_Input_Ini (void);
void Timeout_Detect_Mode(void);
void Debug_port_Ini(void);
void port_0_10(void);
void port_2_0(void);
void port_2_1(void);
void port_2_2(void);
void port_2_3(void);
void port_20_6(void);

#endif /*IFXPHS*/
