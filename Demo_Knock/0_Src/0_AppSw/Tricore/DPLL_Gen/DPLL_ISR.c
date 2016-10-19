/*
 * DPLL_ISR.C
 *
 *  Created on: 2016. 6. 30.
 *      Author: hansteve
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "DPLL_Driver.h"
#include "knocheck_pub.h"
//#include "Test_switch.h"
/*******************************************************************************
**                      Private macros                                        **
*******************************************************************************/

/*******************************************************************************
**                       Global variables/constants                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Function definitions                          **
*******************************************************************************/
IFX_INTERRUPT (FirstSigInput_ISR, 0, TIM00_INT_NUM)
{

    if (GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL)
    {
		GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = IRQ_DISABLE;
		GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = IRQ_ACK;

		/* Ignore Time setting */
		if (CrankState.waitStart > 0)
		{
			GTM_ATOM4_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = IRQ_ENABLE;
			GTM_ATOM4_CH0_CM0.U = (uint32) (WAIT_START + GTM_TBU_CH0_BASE.U) & 0x0FFFFFFF;
		}

    }

}

IFX_INTERRUPT (Auxiliary_ISR, 0, ATOM40_INT_NUM)
{
	uint32 tempsr1;
    /*Ignore Time End */
    if ((GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU0TC))
    {                           //Time Event CH1
        GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU0TC = IRQ_ACK;
        GTM_ATOM4_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = IRQ_DISABLE;
        Timeout_Detect_Mode();
        if (CrankState.currentState != FULL_SYNC)
        {
            GTM_DPLL_CTRL_1.B.DEN = 0x1;   //Start DPLL
            /* Ignore Tooth setting */
            GTM_DPLL_NTI_CNT.B.NTI_CNT = CrankState.nIgnoreTeeh;

        }
    }

    /*Manage the setAngleEvent */
    if (GTM_ATOM4_CH1_IRQ_NOTIFY.B.CCU1TC)
    {                           //Angle Event CH1
        GTM_ATOM4_CH1_IRQ_NOTIFY.B.CCU1TC = 0x1;
    }

    /*Simulate Gap Teeth */
    if ((GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU1TC))
    {                           //Angle Event CH0
    	tempsr1 = GTM_ATOM4_CH0_SR1.U;

        GTM_ATOM4_CH0_CM1.U = (uint32) (GENERATED_PLL + tempsr1);
        GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;   //Enable Check

        if (CrankState.toothInGap < IFX_VRS_GAPTEETH)
        {
            GTM_ATOM4_CH0_CM1.B.CM1 = tempsr1 + (GENERATED_PLL);  //6 degrees event

            CrankState.toothInGap++;
        }
        else
        {
            simulateGap = 0;
            CrankState.toothInGap = 0;
        }
        GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
    }
}

/*Ignore Tooth End */
IFX_INTERRUPT (CDT_ISR, 0, CDTI_INT_NUM)
{
	//port_2_2();
    if (CrankState.currentState == NO_SYNC)
    {
        CrankState.currentState = PRE_SYNC;
        CrankState.synchTeeth += 1;

        GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = IRQ_ENABLE;

        /*If there is no presync tooth*/
        if (CrankState.synchTeeth >= CrankState.nMinPresyncTeeth)
        {
            SRC_GTMDPLL7.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;   //Enable MTI Slope Interrupt
            SRC_GTMDPLL13.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
        }

        SRC_GTMDPLL23.U = (CPU2_IR << 11) | (DISABLED_INT << 10) | CDTI_INT_NUM;
        SRC_GTMDPLL9.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | TASI_INT_NUM;  //Active Slope Interrupt
    }
}

IFX_INTERRUPT (TAS_ISR, 0, TASI_INT_NUM)
{
		GTM_DPLL_IRQ_NOTIFY.B.TASI = IRQ_ACK;

        switch (CrankState.currentState)
        {
			case NO_SYNC:
				break;

			case FULL_SYNC:/*After Sync*/
				/* you can select whether come to here or not*/
				   GTM_DPLL_CTRL_1.B.SGE1 = 1;
				CrankState.currentMode = ENGINE_RUN;
				uint32 volatile aptVal = (uint32) (GTM_DPLL_APT_2C.B.APT_2C);
				break;
			case PRE_SYNC:/*Before Gap*/
				CrankState.synchTeeth += 1;
				if (CrankState.synchTeeth >= CrankState.nMinPresyncTeeth)
				{
					SRC_GTMDPLL7.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;   //Enable MTI Slope Interrupt
					SRC_GTMDPLL13.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
				}
				break;
			case WAIT_SYNC:/*After Gap*/
				GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = IRQ_ENABLE;
				GTM_TBU_CHEN.B.ENDIS_CH1 = 1;   //Disable Angle Counter
				GTM_TBU_CH1_BASE.U = 0x00000000;
				GTM_TBU_CHEN.B.ENDIS_CH1 = 2;   //Enable Angle Counter
				GTM_DPLL_IRQ_EN.B.TE0I_IRQ_EN = 1;
				GTM_DPLL_IRQ_EN.B.PWI_IRQ_EN = 1;

				GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;
				GTM_ATOM4_CH0_CTRL.B.ACB = 0xF; //Compare CCU1 only ANGLE, Toggle on match

				CrankState.currentState = FULL_SYNC;
				CrankState.currentMode = ENGINE_RUN;
				break;
        }
}

IFX_INTERRUPT (MTI_ISR, 0, MTI_INT_NUM)
{
    uint32 volatile aptVal = (uint32) (GTM_DPLL_APT_2C.B.APT_2C);
    GTM_DPLL_IRQ_NOTIFY.B.MTI = 1;
    switch (CrankState.currentState)
    {
    case PRE_SYNC:

        if (CrankState.startAngleSignalFlag != TRUE)
        {
            //Normal Case
            GTM_DPLL_APT_2C.B.APT_2C = 0;   //Synchronization
            GTM_TBU_CHEN.B.ENDIS_CH1 = 1;   //Disable Angle Counter
            GTM_TBU_CH1_BASE.U = 0x00000000;
            GTM_TBU_CHEN.B.ENDIS_CH1 = 2;   //Enable Angle Counter
            CrankState.currentState = WAIT_SYNC;
        }
        else
        {
        }
        break;
    case FULL_SYNC:
        if ((aptVal == VRS_FIRST_APT_2C_TOOTH_1st_ROT) || (aptVal == VRS_FIRST_APT_2C_TOOTH_2nd_ROT))
        {

        }
        else
        {
            if ((CrankState.isDpllLocked == TRUE) && (GTM_DPLL_STATUS.B.ITN))
            {
                CrankState.missingTeethCnt++;
                if (CrankState.missingTeethCnt <= 1)
                {

                    GTM_DPLL_APT_2C.B.APT_2C += 1;
                    if (GTM_DPLL_APT_2C.B.APT_2C == (2 * IFX_VRS_REAL_TEETH_HALF_SCALE))
                    {
                        GTM_DPLL_APT_2C.B.APT_2C = 0;
                    }
                    GTM_DPLL_MPVAL1.U = GENERATED_PLL;
                    GTM_DPLL_CTRL_1.B.PCM1 = (1U);
                }
                else
                {
                    //Too many missing teeth in this rotation
                    CrankState.currentState = NO_SYNC;
                    CrankState.missingTeethCnt = 0;
                    GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
                }
            }
        }

        break;
    default:
        break;
    }

}

IFX_INTERRUPT (GLI_ISR, 0, GLI_INT_NUM)
{
    GTM_DPLL_IRQ_NOTIFY.B.GL1I = 1;
    SRC_GTMDPLL13.U = (CPU2_IR << 11) | (DISABLED_INT << 10) | GLI_INT_NUM; /* 0<11 => ON CPU2_IR, 1<<10 => Enable *///GL
    SRC_GTMDPLL14.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | LLI_INT_NUM;
   GTM_DPLL_NUTC.U = (1 << 31)|(4 << 19) |(1 << 29) | (1 << 10) | 120; //NUTE FULL_SCALE, 120

    GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 1;

    if (CrankState.currentState == FULL_SYNC)
    {
        GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 1;
        CrankState.isDpllLocked = TRUE;
    }
}

IFX_INTERRUPT (TE0_ISR, 0, TE0_INT_NUM)
{
	 P00_OMSR.B.PS10=1;
	 P00_OMCR.B.PCL10=1;
}

IFX_INTERRUPT (TE1_ISR, 0, TE1_INT_NUM)
{
		P00_OMSR.B.PS10=1;
		 P00_OMCR.B.PCL10=1;
    if (CrankState.currentState == FULL_SYNC)
    {
        uint32 currentAngle = (GTM_TBU_CH1_BASE.U & 0x00FFFFFF);
        GTM_DPLL_PVT.U = (uint32) IFX_VRS_PVT_CHECK_FIRST_TOOTH;    //Plausability_val
        GTM_DPLL_IRQ_NOTIFY.B.TE1I = 0x1;
        //Start "missing tooth" simulation (gap teeth)
        CrankState.toothInGap = 0;
        CrankState.missingTeethCnt = 0;
        /*Check if an event has been programmed inside one of the teeth in the GAp */
        if (simulateGap == 1)
        {
            GTM_ATOM4_CH0_CM1.U = (uint32) (GENERATED_PLL + currentAngle);
            GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;   //Enable Check
        }
    }
}

IFX_INTERRUPT (TE2_ISR, 0, TE2_INT_NUM)
{
    if (CrankState.currentState == FULL_SYNC)
    {

        GTM_DPLL_PVT.U = (uint32) IFX_VRS_PVT_CHECK_NORMAL_TEETH;
        GTM_DPLL_IRQ_NOTIFY.B.TE2I = 0x1;

    }
}

IFX_INTERRUPT (SyncPoint_TE3_ISR, 0, TE3_INT_NUM)
{

    if (CrankState.currentState == FULL_SYNC)
    {
        if ((CrankState.eventOnToothEn == TRUE) && (simulateGap == 0))
        {
        	CrankState.eventOnToothEn = FALSE;
            if (CrankState.toothEventProgrammed != 0)
                pmem_ADT_Tx[(uint8) CrankState.toothEventProgrammed] &= 0xF7FFF; /* TE3I Disable */
            GTM_DPLL_IRQ_NOTIFY.B.TE3I = 0x1;

            Sync_tooth=Sync_tooth+30;
            if(6*Sync_tooth>720)
			{
				Sync_tooth=DEFAULT_SYNC;
			}
            Set_EventOnTooth(Sync_tooth);
            KnoCheck_ASWControl();
        }

    }
    /*Very Important: Update to rotation Angle Offest */
}

IFX_INTERRUPT (TE4_ISR, 0, TE4_INT_NUM)
{
    if (CrankState.currentState == FULL_SYNC)
    {
        GTM_DPLL_IRQ_NOTIFY.B.TE4I = 0x1;
    }
}

IFX_INTERRUPT (LLI_ISR, 0, LLI_INT_NUM)
{
    GTM_DPLL_IRQ_NOTIFY.B.LL1I = 1;

    if (CrankState.currentState == FULL_SYNC)
    {

        if (CrankState.missingTeethCnt == 0)
        {
            //More teeth than expected?
            if (GTM_DPLL_NUTC.B.SYN_T == (IFX_VRS_GAPTEETH + 1))
            {                   //A Gap was expected (tooth in Hole)
                CrankState.currentState = NO_SYNC;
                GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
            }
        }
        //if gap was expected and we got a normal one
        SRC_GTMDPLL7.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;
        SRC_GTMDPLL13.U = (CPU2_IR << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
    }

    CrankState.isDpllLocked = FALSE;
}

IFX_INTERRUPT (PWI_ISR, 0, PWI_INT_NUM)
{
    GTM_DPLL_PVT.U = (uint32) 25;
    GTM_DPLL_IRQ_NOTIFY.B.PWI = 1;

}
