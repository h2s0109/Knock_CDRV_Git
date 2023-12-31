/**
 * \file IfxGtm_Atom_PwmHl.c
 * \brief _ATOM PWMHL details
 *
 * \version iLLD_1_0_0_3_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxGtm_Atom_PwmHl.h"
#include "Ifx_Assert.h"
#include "stddef.h"

/** \addtogroup IfxLld_Gtm_Atom_PwmHl_PwmHl_StdIf_Functions
 * \{ */
/******************************************************************************/
/*------------------------Inline Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief Inverts the active state
 * \param activeState Active state
 * \return State
 */
IFX_INLINE Ifx_ActiveState IfxGtm_Atom_PwmHl_invertActiveState(Ifx_ActiveState activeState);

/******************************************************************************/
/*-----------------------Private Function Prototypes--------------------------*/
/******************************************************************************/

/** \brief Updates the x output duty cycle in center aligned and center aligned inverted modes
 * \param driver GTM ATOM PWM driver
 * \param tOn T on
 * \return None
 */
static void IfxGtm_Atom_PwmHl_updateCenterAligned(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn);

/** \brief Updates the x output duty cycle in edge aligned modes (left and right aligned)
 * \param driver GTM ATOM PWM driver
 * \param tOn T on
 * \return None
 */
static void IfxGtm_Atom_PwmHl_updateEdgeAligned(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn);

/** \brief Set the outputs to inactive
 * \param driver GTM ATOM PWM driver
 * \param tOn T on
 * \return None
 */
static void IfxGtm_Atom_PwmHl_updateOff(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn);

/** \} */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

static const IfxGtm_Atom_PwmHl_Mode IfxGtm_Atom_PwmHl_modes[5] = {
    {FALSE, &IfxGtm_Atom_PwmHl_updateCenterAligned}, // Ifx_Pwm_Mode_centerAligned
    {TRUE,  &IfxGtm_Atom_PwmHl_updateCenterAligned}, // Ifx_Pwm_Mode_centerAlignedInverted
    {FALSE, &IfxGtm_Atom_PwmHl_updateEdgeAligned  }, // Ifx_Pwm_Mode_leftAligned
    {TRUE,  &IfxGtm_Atom_PwmHl_updateEdgeAligned  }, // Ifx_Pwm_Mode_rightAligned
    {FALSE, &IfxGtm_Atom_PwmHl_updateOff          }  // Ifx_Pwm_Mode_off
};

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE Ifx_ActiveState IfxGtm_Atom_PwmHl_invertActiveState(Ifx_ActiveState activeState)
{
    return activeState == Ifx_ActiveState_low ? Ifx_ActiveState_high : Ifx_ActiveState_low;
}


/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

float32 IfxGtm_Atom_PwmHl_getDeadtime(IfxGtm_Atom_PwmHl *driver)
{
    return IfxStdIf_Timer_tickToS(driver->timer->base.clockFreq, driver->base.deadtime);
}


float32 IfxGtm_Atom_PwmHl_getMinPulse(IfxGtm_Atom_PwmHl *driver)
{
    return IfxStdIf_Timer_tickToS(driver->timer->base.clockFreq, driver->base.minPulse - driver->base.deadtime);
}


Ifx_Pwm_Mode IfxGtm_Atom_PwmHl_getMode(IfxGtm_Atom_PwmHl *driver)
{
    return driver->base.mode;
}


boolean IfxGtm_Atom_PwmHl_init(IfxGtm_Atom_PwmHl *driver, const IfxGtm_Atom_PwmHl_Config *config)
{
    boolean            result       = TRUE;
    uint16             channelMask;
    uint16             channelsMask = 0;
    uint32             channelIndex;

    IfxGtm_Atom_Timer *timer = config->timer;

    /* driver.base must be at offset 0 to be compatible with the standard interface PwmHl */
    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, offsetof(IfxGtm_Atom_PwmHl, base) == 0);

    driver->base.mode             = Ifx_Pwm_Mode_init;
    driver->timer                 = timer;
    driver->base.setMode          = 0;
    driver->base.inverted         = FALSE;
    driver->base.ccxActiveState   = config->base.ccxActiveState;
    driver->base.coutxActiveState = config->base.coutxActiveState;
    driver->base.channelCount     = config->base.channelCount;

    IfxGtm_Atom_PwmHl_setDeadtime(driver, config->base.deadtime);
    IfxGtm_Atom_PwmHl_setMinPulse(driver, config->base.minPulse);

    driver->atom = &(timer->gtm->ATOM[config->atom]);
    /* Only one AGC */
    driver->agc  = (Ifx_GTM_ATOM_AGC *)&driver->atom->AGC.GLB_CTRL;

    IFX_ASSERT(IFX_VERBOSE_LEVEL_ERROR, config->base.channelCount <= IFXGTM_ATOM_PWMHL_NUM_CHANNELS);

    IfxGtm_Cmu_Clk clock = IfxGtm_Atom_Ch_getClockSource(timer->atom, timer->timerChannel);

    for (channelIndex = 0; channelIndex < config->base.channelCount; channelIndex++)
    {
        IfxGtm_Atom_Ch channel;
        /* CCX */
        channel                   = config->ccx[channelIndex]->channel;
        driver->ccx[channelIndex] = channel;
        channelMask               = 1 << channel;
        channelsMask             |= channelMask;

        /* Initialize the timer part */
        IfxGtm_Atom_Ch_configurePwmMode(driver->atom, channel, clock,
            driver->base.inverted ? config->base.ccxActiveState :
            IfxGtm_Atom_PwmHl_invertActiveState(config->base.ccxActiveState),
            IfxGtm_Atom_Ch_ResetEvent_onTrigger, IfxGtm_Atom_Ch_OutputTrigger_forward);

        /* Initialize the port */
        IfxGtm_PinMap_setAtomTout(config->ccx[channelIndex],
            config->base.outputMode, config->base.outputDriver);

        /* COUTX */
        channel                     = config->coutx[channelIndex]->channel;
        driver->coutx[channelIndex] = channel;
        channelMask                 = 1 << channel;
        channelsMask               |= channelMask;

        /* Initialize the timer part */
        IfxGtm_Atom_Ch_configurePwmMode(driver->atom, channel, clock,
            driver->base.inverted ?
            IfxGtm_Atom_PwmHl_invertActiveState(config->base.coutxActiveState)
            : config->base.coutxActiveState,
            IfxGtm_Atom_Ch_ResetEvent_onTrigger, IfxGtm_Atom_Ch_OutputTrigger_forward);

        /* Initialize the port */
        IfxGtm_PinMap_setAtomTout(config->coutx[channelIndex],
            config->base.outputMode, config->base.outputDriver);
    }

    IfxGtm_Atom_Agc_enableChannelsOutput(driver->agc, channelsMask, 0, TRUE);
    IfxGtm_Atom_Agc_enableChannels(driver->agc, channelsMask, 0, TRUE);

    IfxGtm_Atom_PwmHl_setMode(driver, Ifx_Pwm_Mode_off);

    Ifx_TimerValue tOn[IFXGTM_ATOM_PWMHL_NUM_CHANNELS] = {0};
    IfxGtm_Atom_PwmHl_updateOff(driver, tOn); /* tOn do not need defined values */

    /* Transfer the shadow registers */
    IfxGtm_Atom_Agc_setChannelsForceUpdate(driver->agc, channelsMask, 0, 0, 0);
    IfxGtm_Atom_Agc_trigger(driver->agc);
    IfxGtm_Atom_Agc_setChannelsForceUpdate(driver->agc, 0, channelsMask, 0, 0);

    /* Enable timer to update the channels */

    for (channelIndex = 0; channelIndex < driver->base.channelCount; channelIndex++)
    {
        IfxGtm_Atom_Timer_addToChannelMask(timer, 1 << driver->ccx[channelIndex]);
        IfxGtm_Atom_Timer_addToChannelMask(timer, 1 << driver->coutx[channelIndex]);
    }

    return result;
}


void IfxGtm_Atom_PwmHl_initConfig(IfxGtm_Atom_PwmHl_Config *config)
{
    IfxStdIf_PwmHl_initConfig(&config->base);
    config->timer = NULL_PTR;
    config->atom  = IfxGtm_Atom_0;
    config->ccx   = NULL_PTR;
    config->coutx = NULL_PTR;
}


boolean IfxGtm_Atom_PwmHl_setDeadtime(IfxGtm_Atom_PwmHl *driver, float32 deadtime)
{
    Ifx_TimerValue value = IfxStdIf_Timer_sToTick(driver->timer->base.clockFreq, deadtime);

    /* FIXME warn if dead time in out of range: > 1/2 period, ... */
    driver->base.deadtime = value;

    return TRUE;
}


boolean IfxGtm_Atom_PwmHl_setMinPulse(IfxGtm_Atom_PwmHl *driver, float32 minPulse)
{
    Ifx_TimerValue value = IfxStdIf_Timer_sToTick(driver->timer->base.clockFreq, minPulse);

    driver->base.minPulse = value + driver->base.deadtime;
    driver->base.maxPulse = driver->timer->base.period - driver->base.minPulse;

    return TRUE;
}


boolean IfxGtm_Atom_PwmHl_setMode(IfxGtm_Atom_PwmHl *driver, Ifx_Pwm_Mode mode)
{
    boolean                 result = TRUE;
    IfxGtm_Atom_PwmHl_Base *base   = &driver->base;

    if (base->mode != mode)
    {
        if (mode > Ifx_Pwm_Mode_off)
        {
            mode   = Ifx_Pwm_Mode_off;
            result = FALSE;
        }

        base->mode     = mode;
        driver->update = IfxGtm_Atom_PwmHl_modes[mode].update;

        if (base->mode != Ifx_Pwm_Mode_off)
        {
            base->inverted = IfxGtm_Atom_PwmHl_modes[mode].inverted;
        }
        else
        {   /* Keep previous inverted for off mode */
        }

        if (base->inverted)
        {
            driver->ccxTemp   = driver->coutx;
            driver->coutxTemp = driver->ccx;
        }
        else
        {
            driver->ccxTemp   = driver->ccx;
            driver->coutxTemp = driver->coutx;
        }

        {   /* Workaround to enable the signal inversion required for center aligned inverted
             * and right aligned modes */
            /** \note Changing signal level may produce short circuit at the power stage,
             * in which case the inverter must be disable during this action.*/

            /* Ifx_Pwm_Mode_centerAligned and Ifx_Pwm_Mode_LeftAligned use inverted=FALSE */
            /* Ifx_Pwm_Mode_centerAlignedInverted and Ifx_Pwm_Mode_RightAligned use inverted=TRUE */
            uint32 channelIndex;

            for (channelIndex = 0; channelIndex < driver->base.channelCount; channelIndex++)
            {
                IfxGtm_Atom_Ch channel;

                channel = driver->ccx[channelIndex];
                IfxGtm_Atom_Ch_setSignalLevel(driver->atom, channel, base->inverted
                    ? base->ccxActiveState
                    : IfxGtm_Atom_PwmHl_invertActiveState(driver->base.ccxActiveState));

                channel = driver->coutx[channelIndex];
                IfxGtm_Atom_Ch_setSignalLevel(driver->atom, channel, base->inverted
                    ? IfxGtm_Atom_PwmHl_invertActiveState(driver->base.coutxActiveState)
                    : base->coutxActiveState);
            }
        }
    }

    return result;
}


void IfxGtm_Atom_PwmHl_setOnTime(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn)
{
    driver->update(driver, tOn);
}


void IfxGtm_Atom_PwmHl_setupChannels(IfxGtm_Atom_PwmHl *driver, boolean *activeCh, boolean *stuckSt)
{
    /* FIXME TODO */
}


boolean IfxGtm_Atom_PwmHl_stdIfPwmHlInit(IfxStdIf_PwmHl *stdif, IfxGtm_Atom_PwmHl *driver)
{
    /* *INDENT-OFF* Note: this file was indented manually by the author. */
    /* Set the API link */
    stdif->driver               = driver;
    stdif->setDeadtime          = (IfxStdIf_PwmHl_SetDeadtime  )&IfxGtm_Atom_PwmHl_setDeadtime;
    stdif->getDeadtime          = (IfxStdIf_PwmHl_GetDeadtime  )&IfxGtm_Atom_PwmHl_getDeadtime;
    stdif->setMinPulse          = (IfxStdIf_PwmHl_SetMinPulse  )&IfxGtm_Atom_PwmHl_setMinPulse;
    stdif->getMinPulse          = (IfxStdIf_PwmHl_GetMinPulse  )&IfxGtm_Atom_PwmHl_getMinPulse;
    stdif->getMode              = (IfxStdIf_PwmHl_GetMode      )&IfxGtm_Atom_PwmHl_getMode;
    stdif->setMode              = (IfxStdIf_PwmHl_SetMode      )&IfxGtm_Atom_PwmHl_setMode;
    stdif->setOnTime            = (IfxStdIf_PwmHl_SetOnTime    )&IfxGtm_Atom_PwmHl_setOnTime;
    stdif->setupChannels        = (IfxStdIf_PwmHl_SetupChannels)&IfxGtm_Atom_PwmHl_setupChannels;
    IfxGtm_Atom_Timer_stdIfTimerInit(&stdif->timer, driver->timer);
    /* *INDENT-ON* */

    return TRUE;
}


static void IfxGtm_Atom_PwmHl_updateCenterAligned(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn)
{
    uint8          channelIndex;
    Ifx_TimerValue period;
    Ifx_TimerValue deadtime = driver->base.deadtime;

    period = driver->timer->base.period;

    for (channelIndex = 0; channelIndex < driver->base.channelCount; channelIndex++)
    {
        Ifx_TimerValue x; /* x=period*dutyCycle, x=OnTime+deadTime */
        Ifx_TimerValue cm0, cm1;
        x = tOn[channelIndex];

        if (driver->base.inverted != FALSE)
        {
            x = period - x;
        }
        else
        {}

        if ((x < driver->base.minPulse) || (x <= deadtime))
        {   /* For deadtime condition: avoid leading edge of top channel to occur after the trailing edge */
            x = 0;
        }
        else if (x > driver->base.maxPulse)
        {
            x = period;
        }
        else
        {}

        /* Special handling due to GTM issue */
        if (x == period)
        {   /* 100% duty cycle */
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex],
                period + 1 /* No compare event */,
                2 /* 1st compare event (issue: expected to be 1)*/ + deadtime);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex],
                period + 2 /* No compare event, issues has been seen with +1 */,
                2 /* 1st compare event (issue: expected to be 1)*/);
        }
        else if (x == 0)
        {
            cm0 = 1;
            cm1 = period + 2;
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex], cm0, cm1);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex], cm0 + deadtime, cm1);
        }
        else
        {                           /* x% duty cycle */
            cm1 = (period - x) / 2; // CM1
            cm0 = (period + x) / 2; // CM0
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex], cm0, cm1 + deadtime);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex], cm0 + deadtime, cm1);
        }
    }
}


static void IfxGtm_Atom_PwmHl_updateEdgeAligned(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn)
{
    uint8          channelIndex;
    Ifx_TimerValue period;
    Ifx_TimerValue deadtime = driver->base.deadtime;

    period = driver->timer->base.period;

    for (channelIndex = 0; channelIndex < driver->base.channelCount; channelIndex++)
    {
        Ifx_TimerValue x; /* x=period*dutyCycle, x=OnTime+deadTime */
        Ifx_TimerValue cm0, cm1;
        x = tOn[channelIndex];

        if (driver->base.inverted != FALSE)
        {
            x = period - x;
        }
        else
        {}

        if ((x < driver->base.minPulse) || (x <= deadtime))
        {   /* For deadtime condition: avoid leading edge of top channel to occur after the trailing edge */
            x = 0;
        }
        else if (x > driver->base.maxPulse)
        {
            x = period;
        }
        else
        {}

        /* Special handling due to GTM issue */
        if (x == period)
        {   /* 100% duty cycle */
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex],
                period + 1 /* No compare event */,
                2 /* 1st compare event (issue: expected to be 1)*/ + deadtime);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex],
                period + 2 /* No compare event, issues has been seen with +1 */,
                2 /* 1st compare event (issue: expected to be 1)*/);
        }
        else if (x == 0)
        {
            cm0 = 1;
            cm1 = period + 2;
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex], cm0, cm1);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex], cm0 + deadtime, cm1);
        }
        else
        {            /* x% duty cycle */
            cm1 = 2; // CM1, set to 2 due to a GTM issue. should be 1 according to spec
            cm0 = x; // CM0, set to x+2 due to a GTM issue. should be x+1 according to spec
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex], cm0, cm1 + deadtime);
            IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex], cm0 + deadtime, cm1);
        }
    }
}


static void IfxGtm_Atom_PwmHl_updateOff(IfxGtm_Atom_PwmHl *driver, Ifx_TimerValue *tOn)
{
    uint8          channelIndex;
    Ifx_TimerValue period;
    period = driver->timer->base.period;

    for (channelIndex = 0; channelIndex < driver->base.channelCount; channelIndex++)
    {
        IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->ccxTemp[channelIndex],
            2 /* 1 will keep the previous level*/, period + 2);
        IfxGtm_Atom_Ch_setCompareShadow(driver->atom, driver->coutxTemp[channelIndex], period + 1, 2);
    }
}
