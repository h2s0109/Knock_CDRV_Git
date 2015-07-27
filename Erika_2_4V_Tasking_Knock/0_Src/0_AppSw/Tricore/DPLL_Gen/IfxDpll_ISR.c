
#include "IfxVrs_Driver.h"
#include "Ifx_Utility.h"
#include <IfxGtm_reg.h>
#include "IfxVrs_Cfg.h"

#include "knocheck_pub.h"
//#include "Test_switch.h"


uint32 sr1, sr0;
IFX_INTERRUPT (First_Input_Tim00_Isr, 0, TIM00_INT_NUM)
{

    if (GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL)
    {
		GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = IRQ_DISABLE;
		GTM_TIM0_CH0_IRQ_NOTIFY.B.NEWVAL = IRQ_ACK;

		/* Ignore Time setting */
		if (vrsAttributes.waitStart > 0)
		{
			GTM_ATOM4_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = IRQ_ENABLE;
			GTM_ATOM4_CH0_CM0.U = (uint32) (WAIT_START + GTM_TBU_CH0_BASE.U) & 0x0FFFFFFF;
		}

    }

}

IFX_INTERRUPT (IfxVrs_atomMngtIsr, 0, ATOM40_INT_NUM)
{
    /*Ignore Time End */
    if ((GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU0TC))
    {                           //Time Event CH1
        sr0 = GTM_ATOM4_CH0_SR0.U;
        GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU0TC = IRQ_ACK;
        GTM_ATOM4_CH0_IRQ_EN.B.CCU0TC_IRQ_EN = IRQ_DISABLE;
        Timeout_Detect_Mode();
        if (vrsAttributes.currentState != IfxVrs_FlywheelStat_sync)
        {
            GTM_DPLL_CTRL_1.B.DEN = 0x1;   //Start DPLL
            /* Ignore Tooth setting */
            GTM_DPLL_NTI_CNT.B.NTI_CNT = vrsAttributes.nIgnoreTeeh;

        }
    }

    /*Manage the setAngleEvent */
    if (GTM_ATOM4_CH1_IRQ_NOTIFY.B.CCU1TC)
    {                           //Angle Event CH1
        GTM_ATOM4_CH1_IRQ_NOTIFY.B.CCU1TC = 0x1;
        sr1 = GTM_ATOM4_CH1_SR1.U;
    }

    /*Simulate Gap Teeth */
    if ((GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU1TC))
    {                           //Angle Event CH0
        sr1 = GTM_ATOM4_CH0_SR1.U;

        GTM_ATOM4_CH0_CM1.U = (uint32) (IFX_VRS_TOOTH_STEPS + sr1);
        GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;   //Enable Check

        if (vrsAttributes.toothInGap < IFX_VRS_GAPTEETH)
        {
            GTM_ATOM4_CH0_CM1.B.CM1 = sr1 + (IFX_VRS_TOOTH_STEPS);  //6 degrees event

            vrsAttributes.toothInGap++;
        }
        else
        {
            simulateGap = 0;
            vrsAttributes.toothInGap = 0;
        }
        GTM_ATOM4_CH0_IRQ_NOTIFY.B.CCU1TC = 1;
    }
}

/*Ignore Tooth End */
IFX_INTERRUPT (IfxVrs_cdtiIsr, 0, CDTI_INT_NUM)
{
	//port_2_2();
    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_noSync)
    {
        vrsAttributes.currentState = IfxVrs_FlywheelStat_preSync;
        vrsAttributes.synchTeeth += 1;

        GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = IRQ_ENABLE;

        /*If there is no presync tooth*/
        if (vrsAttributes.synchTeeth >= vrsAttributes.nMinPresyncTeeth)
        {
            SRC_GTMDPLL7.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;   //Enable MTI Slope Interrupt
            SRC_GTMDPLL13.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
        }

        SRC_GTMDPLL23.U = (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | CDTI_INT_NUM;
        SRC_GTMDPLL9.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | TASI_INT_NUM;  //Active Slope Interrupt
    }
}

uint16 first =0;
IFX_INTERRUPT (IfxVrs_tasIsr, 0, TASI_INT_NUM)
{


		GTM_DPLL_IRQ_NOTIFY.B.TASI = IRQ_ACK;

        switch (vrsAttributes.currentState)
        {
			case IfxVrs_FlywheelStat_noSync:
				break;

			case IfxVrs_FlywheelStat_sync:/*After Sync*/
				/* you can select whether come to here or not*/
				   GTM_DPLL_CTRL_1.B.SGE1 = 1;
				vrsAttributes.currentMode = IfxVrs_FlywheelMode_run;
				uint32 volatile aptVal = (uint32) (GTM_DPLL_APT_2C.B.APT_2C);
				break;
			case IfxVrs_FlywheelStat_preSync:/*Before Gap*/
				vrsAttributes.synchTeeth += 1;
				if (vrsAttributes.synchTeeth >= vrsAttributes.nMinPresyncTeeth)
				{
					SRC_GTMDPLL7.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;   //Enable MTI Slope Interrupt
					SRC_GTMDPLL13.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
				}
				break;
			case IfxVrs_FlywheelStat_waitSync:/*After Gap*/
				//port_0_10();
				GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = IRQ_ENABLE;
				GTM_TBU_CHEN.B.ENDIS_CH1 = 1;   //Disable Angle Counter
				GTM_TBU_CH1_BASE.U = 0x00000000;
				GTM_TBU_CHEN.B.ENDIS_CH1 = 2;   //Enable Angle Counter
				GTM_DPLL_IRQ_EN.B.TE0I_IRQ_EN = 1;
				GTM_DPLL_IRQ_EN.B.PWI_IRQ_EN = 1;

				GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;
				GTM_ATOM4_CH0_CTRL.B.ACB = 0xF; //Compare CCU1 only ANGLE, Toggle on match

				vrsAttributes.currentState = IfxVrs_FlywheelStat_sync;
				vrsAttributes.currentMode = IfxVrs_FlywheelMode_run;
				break;
        }
}

IFX_INTERRUPT (IfxVrs_mtiIsr, 0, MTI_INT_NUM)
{
    uint32 volatile aptVal = (uint32) (GTM_DPLL_APT_2C.B.APT_2C);
    GTM_DPLL_IRQ_NOTIFY.B.MTI = 1;
    switch (vrsAttributes.currentState)
    {
    case IfxVrs_FlywheelStat_preSync:

        if (vrsAttributes.startAngleSignalFlag != IfxVrs_Bool_true)
        {
            //Normal Case
            GTM_DPLL_APT_2C.B.APT_2C = 0;   //Synchronization
            GTM_TBU_CHEN.B.ENDIS_CH1 = 1;   //Disable Angle Counter
            GTM_TBU_CH1_BASE.U = 0x00000000;
            GTM_TBU_CHEN.B.ENDIS_CH1 = 2;   //Enable Angle Counter


            vrsAttributes.currentState = IfxVrs_FlywheelStat_waitSync;

        }
        else
        {
        }
        break;
    case IfxVrs_FlywheelStat_sync:
        if ((aptVal == VRS_FIRST_APT_2C_TOOTH_1st_ROT) || (aptVal == VRS_FIRST_APT_2C_TOOTH_2nd_ROT))
        {

        }
        else
        {
            if ((vrsAttributes.isDpllLocked == IfxVrs_Bool_true) && (GTM_DPLL_STATUS.B.ITN))
            {
                vrsAttributes.missingTeethCnt++;
                if (vrsAttributes.missingTeethCnt <= 1)
                {

                    GTM_DPLL_APT_2C.B.APT_2C += 1;
                    if (GTM_DPLL_APT_2C.B.APT_2C == (2 * IFX_VRS_REAL_TEETH_HALF_SCALE))
                    {
                        GTM_DPLL_APT_2C.B.APT_2C = 0;
                    }
                    GTM_DPLL_MPVAL1.U = IFX_VRS_TOOTH_STEPS;
                    GTM_DPLL_CTRL_1.B.PCM1 = (1U);
                }
                else
                {
                    //Too many missing teeth in this rotation
                    vrsAttributes.currentState = IfxVrs_FlywheelStat_noSync;
                    vrsAttributes.missingTeethCnt = 0;
                    GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
                }
            }
        }

        break;
    default:
        break;
    }

}

IFX_INTERRUPT (IfxVrs_gliIsr, 0, GLI_INT_NUM)
{
    GTM_DPLL_IRQ_NOTIFY.B.GL1I = 1;
    SRC_GTMDPLL13.U = (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | GLI_INT_NUM; /* 0<11 => ON IFXVRS_CPU2, 1<<10 => Enable *///GL
    SRC_GTMDPLL14.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | LLI_INT_NUM;
   GTM_DPLL_NUTC.U = (1 << 31)|(4 << 19) |(1 << 29) | (1 << 10) | 120; //NUTE FULL_SCALE, 120

    GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 1;

    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {
        GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 1;
        vrsAttributes.isDpllLocked = IfxVrs_Bool_true;
    }
}

IFX_INTERRUPT (IfxVrs_te0Isr, 0, TE0_INT_NUM)
{
	 P00_OMSR.B.PS10=1;
	 P00_OMCR.B.PCL10=1;
}

IFX_INTERRUPT (IfxVrs_te1Isr, 0, TE1_INT_NUM)
{
		P00_OMSR.B.PS10=1;
		 P00_OMCR.B.PCL10=1;
    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {
        uint32 currentAngle = (GTM_TBU_CH1_BASE.U & 0x00FFFFFF);
        GTM_DPLL_PVT.U = (uint32) IFX_VRS_PVT_CHECK_FIRST_TOOTH;    //Plausability_val
        GTM_DPLL_IRQ_NOTIFY.B.TE1I = 0x1;
        //Start "missing tooth" simulation (gap teeth)
        vrsAttributes.toothInGap = 0;
        vrsAttributes.missingTeethCnt = 0;
        /*Check if an event has been programmed inside one of the teeth in the GAp */
        if (simulateGap == 1)
        {
            GTM_ATOM4_CH0_CM1.U = (uint32) (IFX_VRS_TOOTH_STEPS + currentAngle);
            GTM_ATOM4_CH0_IRQ_EN.B.CCU1TC_IRQ_EN = 1;   //Enable Check
        }
    }
}


IFX_INTERRUPT (IfxVrs_te2Isr, 0, TE2_INT_NUM)
{
    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {

        GTM_DPLL_PVT.U = (uint32) IFX_VRS_PVT_CHECK_NORMAL_TEETH;
        GTM_DPLL_IRQ_NOTIFY.B.TE2I = 0x1;

    }
}
uint32 currentAngle;
uint32 Testcount2;
IFX_INTERRUPT (list, 0, TE3_INT_NUM)
{

	P02_OMSR.B.PS3=1;
    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {
        if ((vrsAttributes.eventOnToothEn == TRUE) && (simulateGap == 0))
        {
        	vrsAttributes.eventOnToothEn = FALSE;
            if (vrsAttributes.toothEventProgrammed != 0)
                pmem_ADT_Tx[(uint8) vrsAttributes.toothEventProgrammed] &= 0xF7FFF; /* TE3I Disable */
            GTM_DPLL_IRQ_NOTIFY.B.TE3I = 0x1;

            Sync_tooth=Sync_tooth+30;
            if(6*Sync_tooth>720)
			{
				Sync_tooth=DEFAULT_SYNC;
			}
            IfxVrs_setEventOnTooth(Sync_tooth);

           Ifx_DSADC *psDSADC = &MODULE_DSADC;

    	   currentAngle = (GTM_TBU_CH1_BASE.U & 0x00FFFFFF);
    	   volatile uint32 currentAngle_temp;
    	   volatile uint32 currentAngle_temp2;
    	   currentAngle_temp = GTM_ATOM2_CH5_SR0.U;					/*To update value, Should be read*/
    	   currentAngle_temp2 = GTM_ATOM2_CH5_SR1.U;				/*To update value, Should be read*/
    	   GTM_ATOM2_CH5_CM0.U =currentAngle+273*4;					/*update*/
    	   GTM_ATOM2_CH5_CM1.U =currentAngle+273*10;

			#ifdef SETCTRL_TEST
    	   		SetCtrl_selection();
			#endif /*End of SETCTRL_TEST*/

			#ifdef TOTAL_FILTER_TEST
    	   		SetCtrlData_Total_Filter_Test();
			#endif /*End of SETCTRL_TEST*/

			#ifdef CHANGE_FILTER_DOUBLEBUF_TEST
				Filter_Selection();
				SetCtrlData_Test();
			#endif
			#ifdef CHANNEL_FILTER_SELECT_TEST
				Filter_Selection();
				SetCtrlData_Test();
			#endif
    	   Testcount2++;
        }
        P02_OMCR.B.PCL3=1;
    }
    /*Very Important: Update to rotation Angle Offest */
}


IFX_INTERRUPT (IfxVrs_te4Isr, 0, TE4_INT_NUM)
{
    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {
        GTM_DPLL_IRQ_NOTIFY.B.TE4I = 0x1;
    }
}

IFX_INTERRUPT (IfxVrs_lliIsr, 0, LLI_INT_NUM)
{
    GTM_DPLL_IRQ_NOTIFY.B.LL1I = 1;

    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {

        if (vrsAttributes.missingTeethCnt == 0)
        {
            //More teeth than expected?
            if (GTM_DPLL_NUTC.B.SYN_T == (IFX_VRS_GAPTEETH + 1))
            {                   //A Gap was expected (tooth in Hole)
                vrsAttributes.currentState = IfxVrs_FlywheelStat_noSync;
                GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = 1;
            }
        }
        //if gap was expected and we got a normal one
        SRC_GTMDPLL7.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | MTI_INT_NUM;
        SRC_GTMDPLL13.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10) | GLI_INT_NUM;
    }

    vrsAttributes.isDpllLocked = IfxVrs_Bool_false;
}

IFX_INTERRUPT (IfxVrs_pwiIsr, 0, PWI_INT_NUM)
{
    GTM_DPLL_PVT.U = (uint32) 25;
    GTM_DPLL_IRQ_NOTIFY.B.PWI = 1;

}
