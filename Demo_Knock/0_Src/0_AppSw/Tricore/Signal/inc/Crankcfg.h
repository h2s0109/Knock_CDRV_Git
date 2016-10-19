/********************************************************************************************!
 *  \file
 *  	crankcfg.h
 *  \date
 *  	30-Jun-2016
 *  \author
 *  	Han sung su(Steve.Han)
 *  \brief
 *
 *
 **********************************************************************************************/
#ifndef CRANKCFG_H_
#define CRANKCFG_H_

/******************************************************************************/
/*                          Macros                                           */
/******************************************************************************/
#define PERIOD_100_ms	(1000000)
#define PERIOD_500_ms	(500000)
#define PERIOD_10_ms 	(100000)
#define PERIOD_5_ms 	(50000)
#define PERIOD_2_ms 	(20000)
#define PERIOD_1_ms 	(10000)
#define PERIOD_5_us 	(5000)
#define PERIOD_1_us 	(1000)

#define per_One_third 0.66667
#define per_50 	0.5
#define per_40 	0.4
#define per_30 	0.3
#define per_20 	0.2
#define per_10 	0.1
#define per_5 	0.05
#define per_1 	0.01

#define DEINC3	50
#define ININC3	50
#define ININC100	100
#define DEINC100	100
#define REVOLUTION 2

/********************************************************************************************/
/*								User configuration start from here							*/
/********************************************************************************************/

#define CRANK_PULSE
//#define SIMPLE_PULSE
#define CAM_PULSE
//#define STOP_MODE

#ifdef CRANK_PULSE
	#define TOTAL_TOOTH_NUM 60
	#define MISSING_TOOTH_NUM 2
	#define START_CRANK_NUM 30
	#define PERIOD_SETUP PERIOD_1_ms /*Crank period signal user define*/
	#define PERIOD_MAX PERIOD_2_ms /*Crank period signal user define*/
	#define PERIOD_MIN PERIOD_5_us /*Crank period signal user define*/
	#define INCREASE_INCREMENT ININC100 /*Crank period signal user define*/
	#define DECREASE_INCREMENT DEINC100 /*Crank period signal user define*/
	#define DUTY_SETUP per_50 /*Crank duty signal user define*/
#endif

#ifdef CAM_PULSE
	#define CAM_NUM			2/*캠 개수를 입력한다.*/
	#define CAM_EDGE_NUM	2/*에지 개수를 기록한다.*/
	#define CAM_LEVEL_HIGH	TRUE
	#define CAM_LEVEL_LOW	FALSE
#endif

#ifdef STOP_MODE
	#define PULSE_NUM 65 /*Crank signal generate number user define*/
#endif

/********************************************************************************************/
/*									User configuration end here								*/
/********************************************************************************************/

#define PULSE_END PULSE_NUM+1
#define GAP TOTAL_TOOTH_NUM
#define REAL_TOTAL_TOOTH_NUM TOTAL_TOOTH_NUM*REVOLUTION
#define GAP_TOTAL_TOOTH (MISSING_TOOTH_NUM+1)
#define BEFORE_GAP (TOTAL_TOOTH_NUM-GAP_TOTAL_TOOTH)

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

#endif /*CRANKCFG_H_*/
