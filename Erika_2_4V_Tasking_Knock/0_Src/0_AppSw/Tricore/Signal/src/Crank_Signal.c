/**
 * \file Crank_Signal.c
 * \brief Basic GTM TIM Examples.
 *
 * \license
 * You can use this file under the terms of the IFX License.
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the IFX License for more details (IFX_License.txt).
 *
 * This file may be used, copied, and distributed, with or without modification, provided
 * that all copyright notices are retained; that all modifications to this file are
 * prominently noted in the modified file; and that this paragraph is not modified.
 *
 * \copyright Copyright (C) 2013 Infineon Technologies AG
 * \author Alfredo Baratta (alfredo.baratta@infineon.com)
 *
 */

#include "Crank_Signal.h"
#include "CDRV_setup.h"
#include "IfxPort_reg.h"
#include "Ifx_reg.h"
#include "Ifx_Utility.h"
#include "Speed_Test_switch.h"

static void Gtm_Init_IR_ATOM2CH0(void);

#ifdef STOP_MODE
	static void Crank_Signal_Brake(void);
#endif



/**!
 * TIM0_CH0 is used to measure a PWM Period and Duty Cycle (on ATOM2_CH0 P00.9)
 */
#if TEST
void gtmTimTIEM(void) {

	Gtm_Init_IR_ATOM2CH0();

	GTM_TIM1_CH0_CTRL.B.TIM_MODE = TIEM;
	GTM_TIM1_CH0_CTRL.B.ISL = 1; //ignore DSL and treat both edges as active edge


	GTM_TIM1_CH0_CNT.U = 0;
	GTM_TIM1_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 0x1; // Enable NewVal Interrupt
	GTM_TIM1_CH0_CTRL.B.TIM_EN = 1; //Start TIM


	GTM_INOUTSEL_TIM1_INSEL.B.CH0SEL = 1; //TIM1_CH0 <=== P00_9 <====
	return;
}
//1. Interrupt Initialization
void Gtm_Init_IR_ATOM2CH0(void) {
	//GTM_IRQ_MODE.B.IRQ_MODE = 0x2; //Pulse,Notify
	//GTM_TIM1_CH0_IRQ_MODE.U = 0x2;
	//SRC_GTMTIM10.U = (IFX_CPU1 << 11) | (IR_ENABLE << 10) | TIM1_CH0_PRIO; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/
	GTM_IRQ_MODE.B.IRQ_MODE = 0x2; //Pulse,Notify
	GTM_ATOM2_CH0_IRQ_MODE.U = 0x2;
	GTM_ATOM2_CH0_IRQ_EN.U = 0X1;
	SRC_GTMATOM20.U = (IFX_CPU1 << 11) | (IR_ENABLE << 10) | ATOM2_CH0_PRIO; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/
}
#endif


/**************************************************************************************************/
//1. Interrupt Initialization
static void Gtm_Init_IR_ATOM2CH0(void) {
	GTM_IRQ_MODE.B.IRQ_MODE = 0x2; //Pulse,Notify
	GTM_ATOM2_CH0_IRQ_MODE.U = 0x2;
	GTM_ATOM2_CH0_IRQ_EN.U = 0X3;/*CCU0TC and CCU1TC support*/
	SRC_GTMATOM20.U = (IFX_CPU1 << 11) | (IR_ENABLE << 10) | ATOM2_CH0_PRIO; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/
}
/**!
 * ATOM2_CH0 setup
 * @param period
 */
void Crank_Signal(uint32 period, float duty) {

	//ATOM2 CH0 settings (PWM Period)
	GTM_ATOM2_CH0_CTRL.B.CLK_SRC = 4; //Forward the Period Trigger to the next Channel
	GTM_ATOM2_CH0_CTRL.B.MODE = 0x2; //(SOMP)
	GTM_ATOM2_CH0_CTRL.B.SL = 0; //Low Level
	GTM_ATOM2_CH0_CTRL.B.TRIGOUT = 1; //Forward the Period Trigger to the next Channel
	//GTM_ATOM2_CH0_CTRL.B.OSM = 1;
	GTM_ATOM2_CH0_SR0.U = (uint32) period; //Period
	GTM_ATOM2_CH0_SR1.U = (uint32) (period * duty); //Duty Cycle

	//ATOM2 Global registers settings
	GTM_ATOM2_AGC_GLB_CTRL.B.UPEN_CTRL0 = 0x2;
	GTM_ATOM2_AGC_ENDIS_CTRL.B.ENDIS_CTRL0 = 0x2;
	GTM_ATOM2_AGC_OUTEN_CTRL.B.OUTEN_CTRL0 = 0x2;
	GTM_ATOM2_AGC_FUPD_CTRL.B.FUPD_CTRL0 = 0x2;
	GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT0 = 0x2;
	GTM_ATOM2_AGC_GLB_CTRL.B.HOST_TRIG = 1;

	Gtm_Init_IR_ATOM2CH0();
	//Setup the GTM Port Output (P00.9)
	GTM_INOUTSEL_T_OUTSEL1.B.SEL2 = 0x2;
	P00_IOCR8.B.PC9 = 0x11;

}

#ifdef CAM_PULSE
	uint8 Start_array[CAM_NUM] = {0,};
	uint32 Real_Tooth_Num = START_CRANK_NUM;

	CAM_style CAM[CAM_NUM][CAM_EDGE_NUM]={{{CAM_LEVEL_HIGH,47},{CAM_LEVEL_LOW,107}},{{CAM_LEVEL_HIGH,47},{CAM_LEVEL_LOW,107}}};

	void CAM_Signal(void ) {

		//ATOM2 CH1 settings
		GTM_ATOM2_CH1_CTRL_SOMI.B.MODE =0;/*Set to SOMI MODE*/
		GTM_ATOM2_CH1_CTRL_SOMI.B.ARU_EN =0;/*Not use ARU*/
		GTM_ATOM2_CH1_CTRL_SOMI.B.ACB0 = 0;/*Set output to ~*/
		GTM_ATOM2_CH1_CTRL_SOMI.B.SL =0;/*Initial signal level after channel is low*/

		//ATOM2 Global registers settings
		GTM_ATOM2_AGC_GLB_CTRL.B.UPEN_CTRL1 = 0x2;
		GTM_ATOM2_AGC_ENDIS_CTRL.B.ENDIS_CTRL1 = 0x2;
		GTM_ATOM2_AGC_OUTEN_CTRL.B.OUTEN_CTRL1 = 0x2;
		GTM_ATOM2_AGC_OUTEN_STAT.B.OUTEN_STAT1 =0x2;
		GTM_ATOM2_AGC_FUPD_CTRL.B.FUPD_CTRL1 = 0x2;
		GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT1 = 0x2;
		/*Test port pin setting*/
		GTM_TOUTSEL5.B.SEL15 = 0x2;
		P11_IOCR0.B.PC2 = 0x11;

		//ATOM2 CH2 settings
		GTM_ATOM2_CH2_CTRL_SOMI.B.MODE =0;/*Set to SOMI MODE*/
		GTM_ATOM2_CH2_CTRL_SOMI.B.ARU_EN =0;/*Not use ARU*/
		GTM_ATOM2_CH2_CTRL_SOMI.B.ACB0 = 0;/*Set output to ~*/
		GTM_ATOM2_CH2_CTRL_SOMI.B.SL =0;/*Initial signal level after channel is low*/

		//ATOM2 CH2 Global registers settings
		GTM_ATOM2_AGC_GLB_CTRL.B.UPEN_CTRL2 = 0x2;
		GTM_ATOM2_AGC_ENDIS_CTRL.B.ENDIS_CTRL2 = 0x2;
		GTM_ATOM2_AGC_OUTEN_CTRL.B.OUTEN_CTRL2 = 0x2;
		GTM_ATOM2_AGC_OUTEN_STAT.B.OUTEN_STAT2 =0x2;
		GTM_ATOM2_AGC_FUPD_CTRL.B.FUPD_CTRL2 = 0x2;
		GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT2 = 0x2;

		/*Test port pin setting*/
		GTM_TOUTSEL6.B.SEL0 = 0x2;
		P11_IOCR0.B.PC3 = 0x11;

		/*CAM Starting point setting*/
		CAM_Start_point();

	}

	void CAM_Start_point(void )
	{

		for(uint8 j=0;j <CAM_NUM;j++)
		{
			for(uint8 i =0; i<CAM_EDGE_NUM;i++)
			{
				if(CAM[j][i].edge>= START_CRANK_NUM)
				{
					Start_array[j] = i;
					break;
				}
			}
		}
	}
#endif

#ifdef STOP_MODE
	static void Crank_Signal_Brake(void )
	{

		GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT0 = 0x1;
	}
#endif

uint8 cnt_ATOM2_CH0 = 0;

#ifdef CRANK_PULSE
	uint32 Tooth_Num =START_CRANK_NUM;
#endif

#ifdef STOP_MODE
	boolean Brake = FALSE;
	boolean Mode_Stop =  TRUE;
#endif

/*!
 * Interrupt Service Routine
 */
	uint32 Tbu_value[1000]={0,};
	uint32 Tbu_count;

IFX_INTERRUPT(ATOM2_CH0_Isr, 0, ATOM2_CH0_PRIO)
{
#ifdef CRANK_PULSE
		switch(Tooth_Num)
		{
			case BEFORE_GAP:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{
					Tooth_Num=Tooth_Num+GAP_TOTAL_TOOTH;

					#ifdef CAM_PULSE
					Real_Tooth_Num = Real_Tooth_Num+GAP_TOTAL_TOOTH;
					#endif

					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1; //Clean Interrupt Notification Bit
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
#ifdef SPEED
					if(Real_Tooth_Num<73)
					{
#endif
					GTM_ATOM2_CH0_SR0.U = (uint32) (GAP_PERIOD_SETUP); //Period
					GTM_ATOM2_CH0_SR1.U = (uint32) (GAP_DUTY_SETUP); //Duty Cycle
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
#ifdef SPEED
					}
					else
					{
					GTM_ATOM2_CH0_SR0.U = (uint32) (GAP_PERIOD_SETUP)*2; //Period
					GTM_ATOM2_CH0_SR1.U = (uint32) (GAP_DUTY_SETUP)*2; //Duty Cycle
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
					}
#endif
				}
				break;
			case GAP:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{

					#ifdef CAM_PULSE
					for(uint8 i = 0; i<CAM_NUM;i++)
					{
						if(Real_Tooth_Num == CAM[i][Start_array[i]].edge )
						{
							switch(i){
								case 0:
										if(CAM[i][Start_array[i]].level == CAM_LEVEL_HIGH)
										{
											GTM_ATOM2_CH1_CTRL_SOMI.B.ACB0 = 1;
										}
										else
										{
											GTM_ATOM2_CH1_CTRL_SOMI.B.ACB0 = 0;
										}
									break;
								case 1:

										if(CAM[i][Start_array[i]].level == CAM_LEVEL_HIGH)
										{
											GTM_ATOM2_CH2_CTRL_SOMI.B.ACB0 = 1;
										}
										else
										{
											GTM_ATOM2_CH2_CTRL_SOMI.B.ACB0 = 0;
										}
										break;
								default:
									break;
								}
							if(Start_array[i]<(CAM_EDGE_NUM-1))
							{
								Start_array[i]++;
							}
							else
							{
								Start_array[i] = 0;
							}
						}
					}

					if(Real_Tooth_Num == (uint8)REAL_TOTAL_TOOTH_NUM)
					{
						Real_Tooth_Num =1;
					}
					else
					{
						Real_Tooth_Num++;
					}
					#endif

					Tooth_Num=1;


					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1;
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
#ifdef SPEED
					if(Real_Tooth_Num<73)
					{
#endif
					GTM_ATOM2_CH0_SR0.U = (uint32) (PERIOD_SETUP); //Period
					GTM_ATOM2_CH0_SR1.U = (uint32) ((uint32)PERIOD_SETUP*(float)DUTY_SETUP); //Duty Cycle
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
#ifdef SPEED
					}
					else
					{
						GTM_ATOM2_CH0_SR0.U = (uint32) (PERIOD_SETUP)*2; //Period
						GTM_ATOM2_CH0_SR1.U = (uint32) ((uint32)PERIOD_SETUP*(float)DUTY_SETUP)*2; //Duty Cycle
						GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
					}
#endif
				}
				break;
			default:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1; //Clean Interrupt Notification Bit

					if(Real_Tooth_Num==62)
					{
						if(Tbu_count<1000)
						{
						Tbu_value[Tbu_count]=GTM_TBU_CH1_BASE.U;
						Tbu_count++;
						}
					}

					#ifdef CAM_PULSE
					for(uint8 i = 0; i<CAM_NUM;i++)
					{
						if(Real_Tooth_Num == CAM[i][Start_array[i]].edge )
						{
							switch(i){
							case 0:
									if(CAM[i][Start_array[i]].level == CAM_LEVEL_HIGH)
									{
										GTM_ATOM2_CH1_CTRL_SOMI.B.ACB0 = 1;
									}
									else
									{
										GTM_ATOM2_CH1_CTRL_SOMI.B.ACB0 = 0;
									}
								break;
							case 1:

									if(CAM[i][Start_array[i]].level == CAM_LEVEL_HIGH)
									{
										GTM_ATOM2_CH2_CTRL_SOMI.B.ACB0 = 1;
									}
									else
									{
										GTM_ATOM2_CH2_CTRL_SOMI.B.ACB0 = 0;
									}
									break;
							default:
								break;
							}

							if(Start_array[i]<(CAM_EDGE_NUM-1))
							{
								Start_array[i]++;
							}
							else
							{
								Start_array[i] = 0;
							}
						}
					}

					if(Real_Tooth_Num == (uint8)REAL_TOTAL_TOOTH_NUM)
					{
						Real_Tooth_Num =1;
					}
					else
					{
						Real_Tooth_Num++;
					}
					#endif

					Tooth_Num++;

					if(Tooth_Num%10==1)
					{
						//port_2_3();
					}

					#ifdef STOP_MODE
					if(cnt_ATOM2_CH0 == (uint8)PULSE_END)
					{
						Brake = TRUE;
					}
					#endif
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
					#ifdef STOP_MODE
					if(Brake == TRUE)
					{
						Crank_Signal_Brake();
					}
					#endif
				}
			break;
		}
#else //SIMPLE_PULSE
	if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
		{
			cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
			GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1; //Clean Interrupt Notification Bit
			#ifdef STOP_MODE
			if(cnt_ATOM2_CH0 == (uint8)PULSE_END)
			{
				Brake = TRUE;
			}
			#endif
		}
		else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
		{
			GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
			#ifdef STOP_MODE
			if(Brake == TRUE)
			{
				Crank_Signal_Brake();
			}
			#endif
		}
#endif


#if TEST
int cnt_TIM1CH0 = 0;
IFX_INTERRUPT(TIM1_CH0_Isr, 0, TIM1_CH0_PRIO)
{
	if (GTM_TIM1_CH0_IRQ_NOTIFY.B.NEWVAL)
	{
		cnt_TIM1CH0 = cnt_TIM1CH0+1;
		GTM_TIM1_CH0_IRQ_NOTIFY.B.NEWVAL = 1; //Clean Interrupt Notification Bit
		if (GTM_TIM1_CH0_CTRL.B.TIM_MODE == TIEM)
		{
			if(cnt_TIM1CH0==100)
			{
				Crank_Signal_Brake();
			}
		}
	}
}
#endif
}



