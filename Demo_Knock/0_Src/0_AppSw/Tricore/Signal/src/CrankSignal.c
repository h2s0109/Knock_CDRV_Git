/*
 * TEMPLATE.C
 *
 *  Created on: 2016. 6. 30.
 *      Author: hansteve
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/*******************************************************************************
**                      Private macros                                        **
*******************************************************************************/

/*******************************************************************************
**                       Global variables/constants                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Function definitions                          **
*******************************************************************************/

#include "CrankSignal_priv.h"

static void CrankValue_Init(simulator_st* pCrankValue)
{
	simulator_st *CrankValue = pCrankValue;
	CrankValue->Period 				= PERIOD_SETUP;
	CrankValue->PeriodMax 			= PERIOD_MAX;
	CrankValue->PeriodMin 			= PERIOD_MIN;
	CrankValue->Period_DecreasInc 	= DECREASE_INCREMENT ;
	CrankValue->Period_IncreasInc 	= INCREASE_INCREMENT ;
	CrankValue->Dutypercent 		= DUTY_SETUP;
	CrankValue->Duty				= (uint32)((uint32)PERIOD_SETUP*(float)DUTY_SETUP);
	CrankValue->GapPeriod 			= (PERIOD_SETUP)*3;
	CrankValue->GapDuty 			= (uint32) (PERIOD_SETUP)*2+(uint32) ((uint32)PERIOD_SETUP*(float)DUTY_SETUP);
	CrankValue->CurrentPhase		= INCREASE;
	CrankValue->Pattern				= NORMAL;
	CrankValue->CurrToothNum1Rev 	= START_CRANK_NUM;
	CrankValue->CurrToothNum2Rev 	= START_CRANK_NUM;
}
/*CMU value setting for Cranksignal generation*/
static void CrankSignal_ClockSetup(void)
{
	if (GTM_CMU_CLK_EN.B.EN_CLK4==CLK_ENABLED)
	{
				GTM_CMU_CLK_EN.B.EN_CLK4=  1; //Disable it first
	}

	if (GTM_CMU_CLK_EN.U==0x00000000) {
		GTM_CMU_GCLK_NUM.U = 0x00000001;
		GTM_CMU_GCLK_DEN.U = 0x00000001;
	}
	//2. TCMU_CLK[x]=(CLK_CNT[x]+1)*tGTM_CMU_GCLK
	GTM_CMU_CLK_4_CTRL.U = 0x00000009; /* ==> (9+1)*tGTM_CMU_GCLK   => 10x10ns (100ns)  */
	//3. Enable Cmu and Fx Clocks
	GTM_CMU_CLK_EN.B.EN_CLK4 = CLK_EN;
	GTM_CMU_CLK_EN.B.EN_FXCLK = CLK_EN; //All the FXCLK will be enabled
}

static void CrankSignalHw_Init(simulator_st* pCrankValue)
{
	simulator_st *CrankValue = pCrankValue;
	//ATOM2 CH0 settings (PWM Period)
	GTM_ATOM2_CH0_CTRL.B.CLK_SRC = 4; //Forward the Period Trigger to the next Channel
	GTM_ATOM2_CH0_CTRL.B.MODE = 0x2; //(SOMP)
	GTM_ATOM2_CH0_CTRL.B.SL = 0; //Low Level
	GTM_ATOM2_CH0_CTRL.B.TRIGOUT = 1; //Forward the Period Trigger to the next Channel
	//GTM_ATOM2_CH0_CTRL.B.OSM = 1;
	GTM_ATOM2_CH0_SR0.U = CrankValue->Period; //Period
	GTM_ATOM2_CH0_SR1.U = CrankValue->Duty;//Duty Cycle

	//ATOM2 Global registers settings
	GTM_ATOM2_AGC_GLB_CTRL.B.UPEN_CTRL0 = 0x2;
	GTM_ATOM2_AGC_ENDIS_CTRL.B.ENDIS_CTRL0 = 0x2;
	GTM_ATOM2_AGC_OUTEN_CTRL.B.OUTEN_CTRL0 = 0x2;
	GTM_ATOM2_AGC_FUPD_CTRL.B.FUPD_CTRL0 = 0x2;
	GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT0 = 0x2;
	GTM_ATOM2_AGC_GLB_CTRL.B.HOST_TRIG = 1;

	GTM_IRQ_MODE.B.IRQ_MODE = 0x2; //Pulse,Notify
	GTM_ATOM2_CH0_IRQ_MODE.U = 0x2;
	GTM_ATOM2_CH0_IRQ_EN.U = 0X3;/*CCU0TC and CCU1TC support*/
	SRC_GTMATOM20.U = (CPU1_IR << 11) | (ENABLED_INT << 10) | ATOM2_CH0_PRIO; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/

	//Setup the GTM Port Output (P00.9)
	GTM_INOUTSEL_T_OUTSEL1.B.SEL2 = 0x2;
	P00_IOCR8.B.PC9 = 0x11;
}

#ifdef STOP_MODE
	static void Crank_Signal_Brake(void )
	{

		GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT0 = 0x1;
	}
#endif

 void CrankSimulationGen(simulator_st *pCrankValue)
 {
		CrankValue_Init(pCrankValue);
		CrankSignal_ClockSetup(); 				  // CMU_CLK4 100Mhz (10ns)
		CrankSignalHw_Init(pCrankValue); //Generate a PWM on ATOM2_CH0 => P00.9
	#ifdef CAM_PULSE
		CamSignalHw_Init();
	#endif
 }

IFX_INTERRUPT(Cranksignal_ISR, 0, ATOM2_CH0_PRIO)
{
#ifdef CRANK_PULSE
		switch(CrankValue.CurrToothNum1Rev)
		{
			case BEFORE_GAP:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{
					CrankSignal_ToothCnt(BEFORE_GAP_POSITION);
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1; //Clean Interrupt Notification Bit
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
					CrankSignal_Pattern(BEFORE_GAP_POSITION);
				}
				break;
			case GAP:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{

					#ifdef CAM_PULSE
					CamSignal_Handle();
					#endif
					CrankSignal_ToothCnt(AFTER_GAP_POSITION);
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1;
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
					CrankSignal_Pattern(AFTER_GAP_POSITION);
				}
				break;
			default:
				if (GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC == TRUE )
				{
					CrankSignal_ToothCnt(NORMAL_POSITION);
					cnt_ATOM2_CH0 = cnt_ATOM2_CH0+1;
					GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU0TC = 1; //Clean Interrupt Notification Bit

					#ifdef CAM_PULSE
					CamSignal_Handle();
					#endif
					#ifdef STOP_MODE
					if(cnt_ATOM2_CH0 == (uint8)PULSE_END)
					{
						Brake = TRUE;
					}
					#endif
				}
				else if(GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC == TRUE )
				{
					CrankSignal_Pattern(NORMAL_POSITION);
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
}
/*Count the tooth number*/
static void CrankSignal_ToothCnt(position_enum Gap)
{
	switch(Gap)
	{
		case BEFORE_GAP_POSITION:
			CrankValue.CurrToothNum1Rev = CrankValue.CurrToothNum1Rev+GAP_TOTAL_TOOTH;
			CrankValue.CurrToothNum2Rev = CrankValue.CurrToothNum2Rev+GAP_TOTAL_TOOTH;
			break;

		case AFTER_GAP_POSITION:
		case NORMAL_POSITION:
			if(CrankValue.CurrToothNum2Rev == (uint8)REAL_TOTAL_TOOTH_NUM)
			{
				CrankValue.CurrToothNum2Rev = 1;
			}
			else
			{
				CrankValue.CurrToothNum2Rev++;
			}

			if(CrankValue.CurrToothNum1Rev == (uint8)TOTAL_TOOTH_NUM)
			{
				CrankValue.CurrToothNum1Rev = 1;
			}
			else
			{
				CrankValue.CurrToothNum1Rev++;
			}
			break;
	}
}

/*Handling Cranksignal pattern*/
static void CrankSignal_Pattern(position_enum Gap)
{
	switch(CrankValue.Pattern)
	{
		/*Fixed Rpm generation*/
		case NORMAL:
			if(Gap == BEFORE_GAP_POSITION)
			{
				PeriodDutyGap(&CrankValue);

			}
			else
			{
				PeriodDuty(&CrankValue);
				ConvPulse_Cal(&CrankValue);
			}
			GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
		break;
		/*Generate gradually increasing and decreasing Cranksignal*/
		case INCDEC:
			if(Gap == BEFORE_GAP_POSITION)
			{
				if(CrankValue.CurrentPhase == INCREASE)
				{

					if(CrankValue.Period <= CrankValue.PeriodMax)
					{
						CrankValue.Period += CrankValue.Period_IncreasInc ;
					}
					else
					{
						CrankValue.Period -= CrankValue.Period_DecreasInc ;
						CrankValue.CurrentPhase = DECREASE;
					}
				}
				else if(CrankValue.CurrentPhase == DECREASE)
				{

					if(CrankValue.Period >= PERIOD_MIN)
					{
						CrankValue.Period -= CrankValue.Period_DecreasInc ;

					}
					else
					{
						CrankValue.Period += CrankValue.Period_IncreasInc ;
						CrankValue.CurrentPhase = INCREASE;
					}

				}

				PeriodDutyGap(&CrankValue);
			}
			else
			{
				if(CrankValue.CurrentPhase == INCREASE)
				{

					if(CrankValue.Period <= CrankValue.PeriodMax)
					{
						CrankValue.Period += CrankValue.Period_IncreasInc ;
					}
					else
					{
						CrankValue.Period -= CrankValue.Period_DecreasInc ;
						CrankValue.CurrentPhase = DECREASE;
					}
				}
				else if(CrankValue.CurrentPhase == DECREASE)
				{

					if(CrankValue.Period >= PERIOD_MIN)
					{
						CrankValue.Period -= CrankValue.Period_DecreasInc ;
					}
					else
					{
						CrankValue.Period += CrankValue.Period_IncreasInc ;
						CrankValue.CurrentPhase = INCREASE;
					}
				}
				PeriodDuty(&CrankValue);
				ConvPulse_Cal(&CrankValue);
			}
			GTM_ATOM2_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
		break;
	}
}
/* Change the Period and Duty before Gap*/
/* Generate missing tooth*/
static void PeriodDutyGap(simulator_st *pCrankValue)
{
	simulator_st *tempCrankValue = pCrankValue;
	tempCrankValue->GapPeriod = tempCrankValue->Period*(uint32)GAP_TOTAL_TOOTH;
	tempCrankValue->GapDuty	  =	tempCrankValue->Period*(uint32)MISSING_TOOTH_NUM+(uint32)(tempCrankValue->Period*tempCrankValue->Dutypercent);
	GTM_ATOM2_CH0_SR0.U = tempCrankValue->GapPeriod ;	//Period
	GTM_ATOM2_CH0_SR1.U = tempCrankValue->GapDuty;		//Duty
}
/* Change the Period and Duty except before Gap*/
/* Generate Normal tooth*/
static void PeriodDuty(simulator_st *pCrankValue)
{
	simulator_st *tempCrankValue = pCrankValue;
	tempCrankValue->Duty	  =	(uint32)(tempCrankValue->Period*tempCrankValue->Dutypercent);
	GTM_ATOM2_CH0_SR0.U = 	tempCrankValue->Period;		//Period
	GTM_ATOM2_CH0_SR1.U =	tempCrankValue->Duty;		//Duty
}

#ifdef CAM_PULSE
	static void CamSignalHw_Init(void )
	{

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
		CamSignalStartpoint();

	}

	static void CamSignalStartpoint(void )
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

static void CamSignal_Handle(void)
{
	for(uint8 i = 0; i<CAM_NUM;i++)
	{
		if(CrankValue.CurrToothNum2Rev == CAM[i][Start_array[i]].edge )
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
}

static void ConvPulse_Cal(simulator_st *pCrankValue)
{
	simulator_st *tempCrankValue = pCrankValue;
	if(tempCrankValue->Period>10000)

	{
		caltime =38000;
	}
	else if (tempCrankValue->Period<=10000 && tempCrankValue->Period>8000)
	{
		caltime =30000;
	}
	else
	{
		caltime =15000;
	}
}


