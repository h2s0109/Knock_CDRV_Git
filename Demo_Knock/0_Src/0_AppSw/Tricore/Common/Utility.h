/********************************************************************************************!
 *  \file
 *  	Utility.h.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/

#ifndef UTILITY_H_
#define UTILITY_H_

#include "Ifx_Types.h"
#include "Ifx_reg.h"
#include "Irq_prio.h"

/******************************************************************************/
/*                          Macros                                           */
/******************************************************************************/
#define GTM_MCS0_MEM  (*( unsigned int *) 0xF0138000u)

#define MCS0_WRADDR0 0x077
#define MCS0_WRADDR1 0x078
#define DPLL_WRADDR0 0x17f
#define WAD0AEN0_ENABLE 0x00010100
#define TBU_MASK 0x00FFFFFF

#define GTM_DPLL_TS_T       (*(uint32 *) (0xF0128400))
#define GTM_DPLL_TS_T_OLD   (*(uint32 *) (0xF0128404))
#define GTM_DPLL_ADT_T 	    (*(uint32 *) (0xf012C800))
#define GTM_DPLL_ADT_S 	    (*(uint32 *) (0xf0128800))

#define TOOTH_NOT_SET     	0xFF
#define ENGINE_STOP_TIMEOUT 50
#define WAIT_START 			10000
#define DEFAULT_SYNC 		10


#define CLK_CNT_1ms		(99999)
#define CLK_CNT_100ns 	(9)
#define CLK_EN  	 	(2)
#define CLK_DIS 	 	(1)
#define CLK_ENABLED  	(3)
#define PULSE_NOTIFY 	(2)
#define LOW_LES  		(1)

#define IRQ_DISABLE 	(0)
#define IRQ_ENABLE 		(1)
#define IRQ_ACK 		(1)

#define RISING_EDGE 	(1)
#define FALLING_EDGE 	(2)
#define BOTH_EDGE 		(3)

#define TIEM             2

#define CPU0_IR			0
#define CPU1_IR			1
#define CPU2_IR			2
#define DISABLED_INT	0
#define ENABLED_INT		1

#define SOMC (1)
#define SOMP (2)
#define ASYNC_UPDATE (0)
#define SYNC_UPDATE  (1)

#define FXCLK0   (0)
#define FXCLK1   (1)
#define FXCLK2   (2)
#define FXCLK3   (3)
#define FXCLK4   (4)
#define CMU_CLK0 (0)
#define CMU_CLK1 (1)
#define CMU_CLK2 (2)
#define CMU_CLK3 (3)
#define CMU_CLK4 (4)

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

/******************************************************************************/
/*                           Function prototypes                              */
/******************************************************************************/
void DebugPort_Ini(void);
void port_20_6(void);
#endif /*UTILITY_H_*/
