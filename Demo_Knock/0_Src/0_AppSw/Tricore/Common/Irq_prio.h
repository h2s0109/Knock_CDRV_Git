/********************************************************************************************!
 *  \file
 *  	Irq_prio.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/
#ifndef IRQ_PRIO_H_
#define IRQ_PRIO_H_

/******************************************************************************/
/*                          Macros                                           */
/******************************************************************************/

/*INTERRUPTS PRIO */
#define SASI_INT_NUM           1
#define ATOM03_INT_NUM         6
#define ATOM10_INT_NUM         7
#define ATOM11_INT_NUM         8
#define GLI_INT_NUM           10
#define TE1_INT_NUM           11
#define MTI_INT_NUM           12
#define TE0_INT_NUM           13
#define TE2_INT_NUM           15
#define TE3_INT_NUM           77
#define TE4_INT_NUM           17
#define TISI_INT_NUM          18
#define CDTI_INT_NUM          19
#define ATOM00_INT_NUM        20
#define TIM00_INT_NUM         21
#define PWI_INT_NUM           22 /*! Plausability Check Interrupt */
#define TORI_INT_NUM          23
#define ATOM02_INT_NUM        25
#define MCS00_INT_NUM         55
#define ATOM40_INT_NUM        56
#define ATOM30_INT_NUM        59
#define ATOM41_INT_NUM        57
#define ATOM22_INT_NUM        52
#define MCS0_0_INT_NUM        40
#define LLI_INT_NUM           58 /*! DPLL Lost Lock Interrupt */
#define TASI_INT_NUM          65
#define ATOM2_CH0_PRIO     	  244
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

#endif /*IRQ_PRIO_H_*/
