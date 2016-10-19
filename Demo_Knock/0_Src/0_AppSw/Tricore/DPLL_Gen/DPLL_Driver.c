/*
 * DPLL_Driver.c
 *
 *  Created on: 2016. 6. 30.
 *      Author: hansteve
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "DPLL_Driver.h"
#include "Utility.h"

/*******************************************************************************
**                      Private macros                                        **
*******************************************************************************/

/*******************************************************************************
**                       Global variables/constants                           **
*******************************************************************************/
boolean SignalGenComplete = FALSE;
CrankState_struct CrankState;

volatile uint32 *pmem_TSF_T = (uint32 *) 0xf012c400u;
volatile uint32 *pmem_RDT_T = (uint32 *) 0xf012c000u;
volatile uint32 *DT_Ti = (uint32 *) 0xf012cc00u;

uint32 *pmem_ADT_Tx = &GTM_DPLL_ADT_T;
uint8 simulateGap = 0;
/*******************************************************************************
**                      Global Function definitions                          **
*******************************************************************************/

void DPLL_Clock_Init(void)
{
    if ((GTM_CMU_CLK_EN.B.EN_CLK0 == CLK_ENABLED)||(GTM_CMU_CLK_EN.B.EN_CLK1 == CLK_ENABLED))
    {
        GTM_CMU_CLK_EN.B.EN_CLK0 = CLK_DIS;   //Disable it first
        GTM_CMU_CLK_EN.B.EN_CLK1 = CLK_DIS;   //Disable it first
        GTM_TBU_CHEN.B.ENDIS_CH0 = CLK_DIS;   //Disable TBU_CH0 counter
        GTM_TBU_CHEN.B.ENDIS_CH1 = CLK_DIS;   //Disable TBU_CH1 counter
    }

    GTM_CMU_CLK_0_CTRL.B.CLK_CNT = CLK_CNT_100ns;   /* 10ns x (9+1) => 100ns period */
    GTM_CMU_CLK_1_CTRL.B.CLK_CNT = CLK_CNT_1ms; /* 10ns x (99999+1) => 1ms resolution, Used for Crank Timeout */

    GTM_CMU_CLK_EN.B.EN_CLK0 = CLK_EN;   //enable clk_0
    GTM_CMU_CLK_EN.B.EN_CLK1 = CLK_EN;   //enable clk1
}

void DPLL_Init (void)
{
    // configure DPLL
    GTM_DPLL_CTRL_0.B.TEN =1;
    GTM_DPLL_CTRL_0.B.IFP =0;
    GTM_DPLL_CTRL_0.B.TNU =VRS_DPLL_TNU;

    GTM_DPLL_CTRL_0.B.MLT = (GENERATED_PLL - 1) & 0x0000007FF;
    GTM_DPLL_CTRL_1.B.TSL = ACTIVE_FALLING;  //(High-low slope)
    GTM_DPLL_CTRL_1.B.SYN_NT = 2;
    GTM_DPLL_CTRL_1.B.PIT = 1;//TIME RELATED

    GTM_DPLL_MLS1.U = (uint32) 0x0; /* number of pulses between two state events */
    GTM_DPLL_OSW.U = (uint32) 0x100;    /* Update RAM size,256 entries */

    GTM_DPLL_NTI_CNT.U = (uint32) 0;
    GTM_DPLL_MPVAL1.U = (uint32) 0x00000000;    //Very Important
    GTM_DPLL_PVT.U = (uint32) 25;   //Plausability_val

    GTM_DPLL_TOV.U = IFX_VRS_TS_T_CHECK_GAP;    //TS_T_Check TOV value
    GTM_DPLL_THMI.U = 0;        // Trigger Hold Time min */
    GTM_DPLL_THMA.U = (uint32) 2000;    // Trigger Hold Time max */
    GTM_DPLL_TLR.U = (uint32) 0;    //Disable TLR Check

    GTM_DPLL_IRQ_EN.U = (uint32) 0x07FFFFFF;    /* all interrupts */
    GTM_DPLL_IRQ_EN.B.CDSI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TISI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TAXI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.PWI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = 1;
    GTM_DPLL_CTRL_1.B.DEN = 0;
    GTM_DPLL_CTRL_1.B.SSL = 0;
}

void DPLL_IR_Init(void)
{
	GTM_DPLL_IRQ_EN.U = (uint32) 0x06fffBd7; //ALL DPLL IR ENABLE
    SRC_GTMTIM00.U	= (CPU2_IR << 11) | (ENABLED_INT << 10)  | TIM00_INT_NUM; 	/* 0<11 => ON CPU2_IR, 1<<10 => Enable */
    SRC_GTMATOM40.U	= (CPU2_IR << 11) | (ENABLED_INT << 10)  | ATOM40_INT_NUM;  /* ATOM4_CH0/CH1, Enabled */

    SRC_GTMDPLL7.U	= (CPU2_IR << 11) | (DISABLED_INT << 10) | MTI_INT_NUM;  	/* 0<11 => ON CPU2_IR, 0<<10 => Disabled *///MTI
    SRC_GTMDPLL9.U	= (CPU2_IR << 11) | (DISABLED_INT << 10) | TASI_INT_NUM;	/* TAS 0<11 => ON CPU2_IR, 1<<10 => Enable -TRIGGER active slope detected while NTI_CNT is zero */
    SRC_GTMDPLL13.U = (CPU2_IR << 11) | (DISABLED_INT << 10) | GLI_INT_NUM;		/* 0<11 => ON CPU2_IR, 1<<10 => Enable *///GL
    SRC_GTMDPLL18.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | TE0_INT_NUM;		/* TE0 used for a EventOnTooth */
    SRC_GTMDPLL19.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | TE1_INT_NUM;		/* TE1 */
    SRC_GTMDPLL20.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | TE2_INT_NUM;		/* TE2 */
    SRC_GTMDPLL21.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | TE3_INT_NUM;		/* TE3 */
    SRC_GTMDPLL22.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | TE4_INT_NUM;		/* TE4 */
    SRC_GTMDPLL23.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | CDTI_INT_NUM;	/* CDTI */

    SRC_GTMDPLL25.U = (CPU2_IR << 11) | (DISABLED_INT << 10) | TORI_INT_NUM;	/*Trigger out of range interrupt */
    SRC_GTMDPLL14.U = (CPU2_IR << 11) | (DISABLED_INT << 10) | LLI_INT_NUM;  	/* Lost Lock Interrupt */
    SRC_GTMDPLL10.U = (CPU2_IR << 11) | (ENABLED_INT << 10)  | PWI_INT_NUM;  	/* Lost Lock Interrupt */
}

void CrankInput_Init (void)
{
	 /*Configure TIM0_CH0 (TRIGGER = Crankshaft) */
	GTM_TIM0_CH0_FLT_RE.U = (uint32) 0x00000040;    /* Acceptance time for rising edge */
	GTM_TIM0_CH0_FLT_FE.U = (uint32) 0x00000050;    /* Acceptance time for falling edge */
	GTM_TIM0_CH0_CTRL.B.ISL = 1;
	GTM_TIM0_CH0_CTRL.B.DSL = 0;
	GTM_TIM0_CH0_CTRL.B.TIM_MODE = TIEM;
	GTM_TIM0_CH0_CTRL.B.FLT_CTR_RE =1;
	GTM_TIM0_CH0_IRQ_MODE.B.IRQ_MODE = PULSE_NOTIFY;
	GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = IRQ_ENABLE;
	GTM_TIM0INSEL.U = 0x1;     						 /*ATOM2_CH0 ===> TIM0_CH0 (TOUT/TIN18) - TRIGGER (P00.09)*/
	GTM_TIM0_CH0_CTRL.B.TIM_EN = 1;
}


void CrankState_Init (void)
{
    CrankState.isDpllLocked 			= FALSE;
    CrankState.currentState 			= NO_SYNC;
    CrankState.currentMode 				= ENGINE_STOPPED;
    CrankState.startAngleSignalFlag 	= FALSE;
    CrankState.waitStart 				= 0;
    CrankState.eventOnToothEn 			= FALSE;
    CrankState.toothEventProgrammed 	= 0;
    CrankState.nMinPresyncTeeth 		= 0;
    CrankState.missingTeethCnt 			= 0;
    CrankState.synchTeeth 				= 0;
    CrankState.toothInGap 				= 0;
    CrankState.firstTDCTooth 			= TOOTH_NOT_SET;
    CrankState.dpllStatus 				= (Ifx_GTM_DPLL_STATUS *) 0xf0128014u;
}

void Auxiliary_ATOM40_Init(void)
{
    // General ATOM4 setting
    GTM_ATOM4_CH0_CTRL.B.MODE = SOMC;  //SOMC
    GTM_ATOM4_CH0_CTRL.B.SL = 0;

    //CH1 used for angles comparison

    GTM_ATOM4_CH0_CTRL.B.ACB = 0xB; //Compare CCU0 only TIME, Toggle on match

    GTM_ATOM4_AGC_OUTEN_CTRL.B.OUTEN_CTRL0 = 0x2;
    GTM_ATOM4_AGC_ENDIS_CTRL.B.ENDIS_CTRL0 = 0x2;
    GTM_ATOM4_AGC_ENDIS_STAT.B.ENDIS_STAT0 = 0x2;
    GTM_ATOM4_AGC_FUPD_CTRL.B.FUPD_CTRL0 = 0x2;
    GTM_ATOM4_CH0_IRQ_MODE.B.IRQ_MODE = 0x2;    //pulse notify
    GTM_ATOM4_CH0_IRQ_EN.U = 0x0;   //ccu0/1
}

void Timeout_Detect_Mode(void)
{
	GTM_TIM0_CH0_CTRL.B.TOCTRL = FALLING_EDGE;
	GTM_TIM0_CH0_TDUV.U = ENGINE_STOP_TIMEOUT | CMU_CLK1 << 28;
	GTM_TIM0_CH0_IRQ_EN.B.TODET_IRQ_EN = IRQ_ENABLE;
}

void Crank_Start (void)
{
    GTM_IRQ_MODE.B.IRQ_MODE = 0x2;

    GTM_DPLL_IRQ_MODE.B.IRQ_MODE = 0x2;

    GTM_TBU_CH1_CTRL.B.CH_MODE = LOW_LES;
    /*Time Timer start from here*/
    GTM_TBU_CHEN.B.ENDIS_CH0 = CLK_EN;   /* enable TBU channels 0 and 1 */
    /*Angle Timer start from here*/
    GTM_TBU_CHEN.B.ENDIS_CH1 = CLK_EN;   /* enable TBU channels 0 and 1 */

}

void CrankProfile_Init (void)
{
    uint8 count;
    uint8 noCycle = 0;
    uint8 relTeethOffset = 0;

    /* Configuration for - Rotation */

    for (count = 0; count < 120; count++)
    {
        pmem_ADT_Tx[count] = 0;
    }

    for (noCycle = 0; noCycle < 2; noCycle++)
    {
        /* NormalTeeth (NT = 1) value from 0 - 54th Teeth */
        /* Load Normal teeth value from teeth 58 to 112 */
        for (count = relTeethOffset; count < (IFX_VRS_LAST_TOOTH_MIN_1 + relTeethOffset); count += 1)
        {
            pmem_ADT_Tx[count] = IFX_VRS_NORMAL_TOOTH_LEN;
        }
        /* NormalTeeth + TE0I set = RATIO3 check value for 55, 113 Teeth */
        pmem_ADT_Tx[IFX_VRS_LAST_TOOTH_MIN_1 + relTeethOffset] = (IFX_VRS_NORMAL_TOOTH_LEN | IFX_VRS_TE0I_ENABLE);  /* TE0_IRQ=> ratio3 check       */
        /* NormalTeeth + TE1I set = RATIO12bis check value for 56, 114 Teeth */
        pmem_ADT_Tx[IFX_VRS_LAST_TOOTH + relTeethOffset] = (IFX_VRS_NORMAL_TOOTH_LEN | IFX_VRS_TE1I_ENABLE);    /* TE1_IRQ=> ratio1,2 bis check */
        /* 3Teeth + TE2I set = RATIO12 check value for 57, 115 Teeth */
        pmem_ADT_Tx[IFX_VRS_FIRST_TOOTH + relTeethOffset] = (IFX_VRS_GAP_TOOTH_LEN | IFX_VRS_TE2I_ENABLE);  /* TE2_IRQ=> ratio1,2 check     */
        /* for 2nd Rotation */
        relTeethOffset = IFX_VRS_REAL_TEETH_HALF_SCALE;
    }
    /* NormalTeeth + TE3I set for 0th Teeth */
    //pmem_ADT_Tx[8] = (IFX_VRS_NORMAL_TOOTH_LEN | IFX_VRS_TE3I_ENABLE);    /* TE3_IRQ=> update offset      */
}

CrankSyncState Get_CrankState(void)
{
    return (CrankState.currentState);
}

void Set_EventOnTooth (CrankTeethNum toothNo)
{

    uint8 prevValue = CrankState.toothEventProgrammed;


    if (CrankState.currentState == FULL_SYNC)
    {
        if ((toothNo != VRS_FIRST_TOOTH_IN_HOLE_1st_ROT)
            && (toothNo != VRS_SECOND_TOOTH_IN_HOLE_1st_ROT)
            && (toothNo != VRS_FIRST_TOOTH_IN_HOLE_2nd_ROT) && (toothNo != VRS_SECOND_TOOTH_IN_HOLE_2nd_ROT))
        {

            if ((CrankState.eventOnToothEn == TRUE) && (prevValue != 0))
            {
                pmem_ADT_Tx[prevValue] &= 0xF7FFF;  /* Disable the previous */
            }

            if (toothNo >= IFX_VRS_NTEETH)
            {
                pmem_ADT_Tx[(uint8) (toothNo - IFX_VRS_GAPTEETH)] |= 0x8000;    /* TE3I Enabled */
            }
            else
            {
                pmem_ADT_Tx[(uint8) (toothNo)] |= 0x8000;   /* TE3I Enabled */
            }

        }
        else
        {
            simulateGap = 1;
        }
        CrankState.toothEventProgrammed = toothNo;
        CrankState.eventOnToothEn = TRUE;
    }
}

void Set_WaitEndTime (IfxVrs_Period waitTime)
{
    CrankState.waitStart = waitTime;
}
void Set_IgnoreTooth(CrankTeethNum teethNumbers)
{
   CrankState.nIgnoreTeeh = teethNumbers;
}

void Set_NMinPresyncTeeth (CrankTeethNum teethNumbers)
{
    CrankTeethNum oldValue = CrankState.nMinPresyncTeeth;
    if (teethNumbers >= IFX_VRS_MAX_NTEETH)
    {
    	CrankState.nMinPresyncTeeth = 2;
    }
    else
    {
        CrankState.nMinPresyncTeeth = teethNumbers;
    }

}

/* Calculate the Istantaneous RPM using last Tooth duration */
uint32 Get_InstantRpm (void)
{
    uint32 lastToothDur = GTM_DPLL_DT_T_ACT.U;
    uint32 speedRpm = (CRKHAL_u32_RPM_CONV / lastToothDur);
    return (speedRpm);
}

void Reset_Procedure(void)
{
	GTM_CTRL.B.RF_PROT = 0;     //SW RST (global), SW interrupt FORCINT, and SW RAM reset functionality is enabled
	GTM_RST.B.RST = 1;
	GTM_DPLL_CTRL_1.B.SWR =1;
	GTM_CTRL.B.RF_PROT = 1;     //SW RST (global), SW interrupt FORCINT, and SW RAM reset functionality is enabled

	/*DPLL Ram initialize state check*/
	while(((GTM_DPLL_RAM_INI.B.INIT_1B == TRUE)||(GTM_DPLL_RAM_INI.B.INIT_1B == TRUE)))
	{

	}
}
