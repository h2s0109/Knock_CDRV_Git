/****************************************************
 * \copyright Copyright (C) Infineon Technologies (2014)
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * IFX License for more details (IFX_License.txt).
 *
 * This file may be used, copied, and distributed, with or without
 * modification, provided that all copyright notices are retained;
 * that all modifications to this file are prominently noted in the
 * modified file; and that this paragraph is not modified.
 *
 * *
 *****************************************************/
/*!
 *  \file
 *      IfxVrs_Driver.c
 *  \date
 *      21-March-2014 5:11:54 PM
 *  \author
 *      Alfredo Baratta (alfredo.baratta@infineon.com)
 *
 *  \brief
 *      Basic Implementation of the VRS - Crankshaft Management Driver for AURIX/GTM (C Step)
 *  \version
 *      2.01
 *
 *****************************************************/

/** @defgroup VRS VRS Crankshaft Management Driver */
/*  @{ */

#include "IfxVrs_Driver.h"
#include "Ifx_Utility.h"
#include <IfxGtm_reg.h>
#include "IfxVrs_Cfg.h"

#define CRKHAL_u32_1MIN_IN_RES_100NS      (IFX_VRS_NTEETH* 10000000)    /* 600000000 */
#define CRKHAL_u32_RPM_CONV               (600000000/IFX_VRS_NTEETH)

/*****************************************************************************
 Function prototypes
 ******************************************************************************/

/**************************************************************************************************************/


/**
 * Enable Time Base Unit
 * @li TBU_CH0 => Time based  (res. CMU_CLK0)
 * @li TBU_CH1 => Angle based (res. CMU_CLK0)
 * @return None
 */
static void IfxVrs_enableTbu (void);

/**!
 * Write the expected Crankshaft profile (TRIGGER) into the ADT_T[i] DPLL memory region
 * @param nbOfTeeth Number of Teeth
 * @param nbOfMissingTeeth Number of missing Teeth
 *
 * @note The parameters ar not used, Inside the function only the 60-2 crank profile is managed
 * (TODO:Generalize the function)
 */
static IfxVrsPhs_setCrankProfileHook IfxVrs_initCrankProfile (void);

/*  @} */

/******************************************************************************
 Variables
 ******************************************************************************/



/*****************************************************************************
 Public Functions Definition
 ******************************************************************************/


/**!
 * VRS Driver initialization
 * @param config
 * @param diagnosticMode
 * @return
 */
IfxVrs_Mstatus IfxVrs_init (void) {

    uint16 i = 0;
    vrsAttributes.isDpllLocked = IfxVrs_Bool_false;
    vrsAttributes.currentState = IfxVrs_FlywheelStat_noSync;
    vrsAttributes.currentMode = IfxVrs_FlywheelMode_engineStopped;
    vrsAttributes.startAngleSignalFlag = IfxVrs_Bool_false;
    vrsAttributes.waitStart = 0;
    vrsAttributes.eventOnToothEn = FALSE;
    vrsAttributes.toothEventProgrammed = 0;
    vrsAttributes.nMinPresyncTeeth = 0;
    vrsAttributes.missingTeethCnt = 0;
    vrsAttributes.tdcPeriod = 0;
    vrsAttributes.synchTeeth = 0;
    vrsAttributes.angleOffset = 0;
    vrsAttributes.tdcTeeth = 0;
    vrsAttributes.toothInGap = 0;
    vrsAttributes.firstTDCTooth = TOOTH_NOT_SET;
    vrsAttributes.dpllStatus = (Ifx_GTM_DPLL_STATUS *) 0xf0128014u;

    Dpll_Ini();
    IfxVrs_initCrankProfile ();


      firstGap = 0;


    // General ATOM4 setting
    GTM_ATOM4_CH0_CTRL.U = 0x00000001;  //SOMC

    GTM_ATOM4_CH0_IRQ_MODE.B.IRQ_MODE = 0x2;    //pulse notify
    //CH1 used for angles comparison

    GTM_ATOM4_CH0_CTRL.B.ACB = 0xB; //Compare CCU0 only TIME, Toggle on match

    GTM_ATOM4_AGC_OUTEN_CTRL.U = 0x0000AAAA;
    GTM_ATOM4_AGC_ENDIS_CTRL.U = 0x0000AAAA;
    GTM_ATOM4_AGC_ENDIS_STAT.U = 0x0000AAAA;
    GTM_ATOM4_AGC_FUPD_CTRL.B.FUPD_CTRL0 = 0x2;

    GTM_ATOM4_CH0_IRQ_EN.U = 0x0;   //ccu0/1
    Signal_Input_Ini();
    return (IfxVrs_Mstatus_ok);

}

/*!
 * @fn void IfxVrs_start(void)
 * To Start the VRS driver execution
 */
void IfxVrs_start (void)
{
    GTM_IRQ_MODE.U |= 0x2;
    GTM_DPLL_IRQ_MODE.B.IRQ_MODE = 0x2;
    IfxVrs_enableTbu ();

}



/**!
 * fn IfxVrs_FlywheelStat IfxVrs_getFlywheelStat(void)
 * @return the current Flywheel Status
 */
IfxVrs_FlywheelStat IfxVrs_getFlywheelStat (void)
{
    return (vrsAttributes.currentState);
}

/*!
 * @fn IfxVrs_getAngleOffset
 * Returne the Angle Offset used for the relative angle calculation
 * @return None
 */
/*!
 * @fn IfxVrs_setAngleOffset(uint32 angleValue)
 * Setup of the Angle Offset used for the relative angle calculation
 * @param angleValue Angle offset
 * @return None
 */
uint32 IfxVrs_setAngleOffset (uint32 angleValue)
{
    vrsAttributes.angleOffset = angleValue;
    //vrsAttributes.globalTeethOffset = angleValue;
    return (0);
}

/*!
 * @fn IfxVrs_getLastTs(void)
 * Return the last Crankshaft active edge Timestamp
 * @return
 */


uint32 IfxVrs_getLastTs (void)
{
    uint32 ptr = GTM_DPLL_APT.B.APT_2B;

    return (pmem_TSF_T[ptr] & (0x00FFFFFF));
}




/**************************************************************************************************/
/*!
 * @fn IfxVrs_FlywheelAngle IfxVrs_getAngle(IfxVrs_Mstatus *status)
 * Return the absolute angle (Internal Resolution [0...vrsAttributes.uTicksToT])
 * @param status
 * @return relativeaAngle
 */


/*!
 * @fn IfxVrs_Nteeth IfxVrs_setEventOnTooth(IfxVrs_Nteeth toothNo,
 IfxVrs_Mstatus *status)
 *
 * Set an Event on a specifi tooth (callBack function)
 * @param toothNo   Tooth number
 * @param status
 * @return previous tooth where the event was programmed
 */
IfxVrs_Nteeth IfxVrs_setEventOnTooth (IfxVrs_Nteeth toothNo)
{

    uint8 prevValue = vrsAttributes.toothEventProgrammed;


    if (vrsAttributes.currentState == IfxVrs_FlywheelStat_sync)
    {
        if ((toothNo != VRS_FIRST_TOOTH_IN_HOLE_1st_ROT)
            && (toothNo != VRS_SECOND_TOOTH_IN_HOLE_1st_ROT)
            && (toothNo != VRS_FIRST_TOOTH_IN_HOLE_2nd_ROT) && (toothNo != VRS_SECOND_TOOTH_IN_HOLE_2nd_ROT))
        {

            if ((vrsAttributes.eventOnToothEn == TRUE) && (prevValue != 0))
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
        vrsAttributes.toothEventProgrammed = toothNo;
        vrsAttributes.eventOnToothEn = TRUE;
    }

    return (prevValue);
}



/*!
 * @fn IfxVrs_Period IfxVrs_toothPeriod(IfxVrs_Mstatus *status)
 * @param status
 * @return return the last tooth period
 *
 */

IfxVrs_Period IfxVrs_toothPeriod (IfxVrs_Mstatus * status)
{
    *status = IfxVrs_Mstatus_notOk;
    /*@AB:toDo: Consider Swon bit, to be updated */
    return ((IfxVrs_Period) GTM_DPLL_DT_T_ACT.U);
}

/*!
 * @fn IfxVrs_Mstatus IfxVrs_setWaitStart(IfxVrs_Period waitTime)
 * Time to wait before accepting 2占� tooth with Gen8 resolution
 * @param waitTime
 * @return
 */
IfxVrs_Mstatus IfxVrs_setWaitStart (IfxVrs_Period waitTime)
{
    vrsAttributes.waitStart = waitTime;
    return (IfxVrs_Mstatus_ok);
}



/*!
 *  Set the minimum number of teeth to stay in PRESYNC state of VRS state machine
 * @param teethNumbers Minimum number of teeth to stay in PRESYNC state of VRS state machine
 * @param status
 *  @li IfxVrs_Mstatus_ok -> The method has not been executed correctly
 *  @li IfxVrs_Mstatus_notOk -> Input parameter are not correct
 * @return
 * @retval  vrsNMinPresyncTeeth old value
 *
 */
IfxVrs_Nteeth IfxVrs_setNMinPresyncTeeth (IfxVrs_Nteeth teethNumbers)
{
    IfxVrs_Nteeth oldValue = vrsAttributes.nMinPresyncTeeth;
    if (teethNumbers >= IFX_VRS_MAX_NTEETH)
    {
    }
    else
    {
        vrsAttributes.nMinPresyncTeeth = teethNumbers;
    }
    return (oldValue);
}

void Set_Ignore_Tooth(IfxVrs_Nteeth teethNumbers)
{
   vrsAttributes.nIgnoreTeeh = teethNumbers;
}

static void IfxVrs_enableTbu (void)
{
    GTM_TBU_CH1_CTRL.B.CH_MODE = LOW_LES;
    /*Time Timer start from here*/
    GTM_TBU_CHEN.B.ENDIS_CH0 = CLK_EN;   /* enable TBU channels 0 and 1 */
    /*Angle Timer start from here*/
    GTM_TBU_CHEN.B.ENDIS_CH1 = CLK_EN;   /* enable TBU channels 0 and 1 */
}

/**************************************************************************************************/
static IfxVrsPhs_setCrankProfileHook IfxVrs_initCrankProfile (void)
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

    return (0);
}

/* Calculate the Average RPM using the TDC Period */
uint32 IfxVrs_getAvrRpm (void)
{
    IfxVrs_Mstatus status = (IfxVrs_Mstatus) IfxVrs_Mstatus_notOk;

    uint32 tdcPeriod = vrsAttributes.tdcPeriod;
    uint32 speedRpm = (CRKHAL_u32_RPM_CONV * vrsAttributes.tdcTeeth) / (tdcPeriod);
    return (speedRpm);
}

/* Calculate the Istantaneous RPM using last Tooth duration */
uint32 IfxVrs_getIstRpm (void)
{
    uint32 lastToothDur = GTM_DPLL_DT_T_ACT.U;
    uint32 speedRpm = (CRKHAL_u32_RPM_CONV / lastToothDur);
    return (speedRpm);
}



/**************************************************************************************************/
/** \brief Recovery resync
 *
 * NOTE: this is for Recovery mode only.
 * \param <angle> is the angle corresponding to a cam edge detected by the camshaft flywheel in recovery status.
 * \param <tooth> tooth.
 * \param <TP> Tooth Period.
 *
 * \return MS_OK =The method has been executed correctly.
 *   MS_NOT_OK =The method has not been executed correctly
 */
/**************************************************************************************************/

IfxVrs_Mstatus IfxVrs_recoveryResync (uint32 angle, uint32 TP)
{
    IfxVrs_Mstatus status = (IfxVrs_Mstatus) IfxVrs_Mstatus_notOk;

    vrsAttributes.uSimTP = TP;
    status = (IfxVrs_Mstatus) IfxVrs_Mstatus_ok;
    return ((IfxVrs_Mstatus) status);
}

/*******************************************************************************/



