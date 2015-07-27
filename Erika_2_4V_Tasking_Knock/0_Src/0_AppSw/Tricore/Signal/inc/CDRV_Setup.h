#ifndef CDRV_SETUP_H
#define CDRV_SETUP_H

#define PERIOD_100_ms	(1000000)
#define PERIOD_500_ms	(500000)
#define PERIOD_10_ms 	(100000)
#define PERIOD_5_ms 	(50000)
#define PERIOD_1_ms 	(10000)
#define PERIOD_5_us 	(50)
#define PERIOD_1_us 	(10)

#define per_One_third 0.66667
#define per_50 	0.5
#define per_40 	0.4
#define per_30 	0.3
#define per_20 	0.2
#define per_10 	0.1
#define per_5 	0.05
#define per_1 	0.01

extern uint32 Crank_Period;
extern float Crank_Duty;


/******************************************************************/
/**************************User Setup******************************/
/******************************************************************/

//#define SIMPLE_PULSE
#define CRANK_PULSE

//#define STOP_MODE

#ifdef CRANK_PULSE
	#define TOTAL_TOOTH_NUM 60
	#define MISSING_TOOTH_NUM 2
	#define CAM_PULSE
#endif

#ifdef STOP_MODE
	#define PULSE_NUM 65 /*Crank signal generate number user define*/
#endif

#define PERIOD_SETUP PERIOD_1_us*100*3 /*Crank period signal user define*/
#define DUTY_SETUP per_30+per_1 /*Crank duty signal user define*/

/******************************************************************/
/******************************************************************/
/******************************************************************/

#ifdef CRANK_PULSE
	#define GAP TOTAL_TOOTH_NUM
	#define REAL_TOTAL_TOOTH_NUM TOTAL_TOOTH_NUM*2
	#define START_CRANK_NUM 30
	#define GAP_TOTAL_TOOTH (MISSING_TOOTH_NUM+1)
	#define BEFORE_GAP (TOTAL_TOOTH_NUM-GAP_TOTAL_TOOTH)
	#define GAP_PERIOD_SETUP PERIOD_SETUP*(GAP_TOTAL_TOOTH) /*GAP period*/
	#define GAP_DUTY_SETUP ((uint32)((uint32)GAP_PERIOD_SETUP*(float)per_One_third)+(uint32)((uint32)PERIOD_SETUP*(float)DUTY_SETUP)) /*GAP Duty*/

	#ifdef CAM_PULSE
		#define CAM_NUM			2/*캠 개수를 입력한다.*/
		#define CAM_EDGE_NUM	2/*에지 개수를 기록한다.*/
		#define CAM_LEVEL_HIGH	TRUE
		#define CAM_LEVEL_LOW	FALSE
		typedef struct {
			uint8 level;
			uint8 edge;
		} CAM_style;
	#endif

#endif



#define PULSE_END PULSE_NUM+1




#endif
