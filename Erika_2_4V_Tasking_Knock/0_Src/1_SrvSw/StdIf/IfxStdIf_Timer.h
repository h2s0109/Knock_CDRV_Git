/**
 * \file IfxStdIf_Timer.h
 * \brief Standard interface: Timer
 * \ingroup IfxStdIf
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
 *
 * \defgroup library_srvsw_stdif_timer Standard interface: Timer
 * \ingroup library_srvsw_stdif
 *
 * The standard interface timer (Timer) abstract the hardware used for timer feature. It provide, after proper initialization an hardware
 * independant way to interact with the timer functionallity like setting period, trigger points, ...
 *
 * The figure below shows the standard timer interface.
 *
 * \image html "Timer.png" "Standard timer interface"
 *
 * This interface defines the following features:
 * - Timer with configurable frequency
 * - Configurable counting direction
 * - Optional interrupt on timer event with configurable service request provider and priority
 * - Optional trigger signal with:
 *   - Optional interrupt on trigger event with configurable service request provider and priority
 *   - Configurable trigger point
 *   - Configurable trigger edge
 *   - trigger signal can be internal only or forwarded to a port pin
 */

#ifndef IFXSTDIF_TIMER_H_
#define IFXSTDIF_TIMER_H_ 1

#include "Ifx_Types.h"
#include "IfxSrc.h"     /* FIXME interface should not have dependencies to the HAL, an If type needs to be defined */
#include "IfxPort.h"   /* FIXME interface should not have dependencies to the HAL, an If type needs to be defined */ /*FIXME introduce generic IF for ports. ex: IfxPort_OutputMode */
#include "IfxStdIf.h"
/** \brief Output event enable / disable */
typedef enum
{
    IfxStdIf_Timer_Output_disabled,              /**< \brief Timer output is set to the inactive, no change in the output */
    IfxStdIf_Timer_Output_enabled                /**< \brief In case the trigger is enabled,
                                                  *          the output is set to active between time 0 to the trigger point,
                                                  *          and is set to inactive between the trigger point and the period.
                                                  *          In case the trigger is disabled and counting direction if IfxStdIf_Timer_CountDir_up, the output is toggled when the timer is reset
                                                  *          In case the trigger is disabled and counting direction if IfxStdIf_Timer_CountDir_down, the output is toggled when the timer is reloaded
                                                  *          In case the trigger is disabled and counting direction if IfxStdIf_Timer_CountDir_upAndDown, the output is toggled when the timer reach 0
                                                  *          In case the trigger is enabled the trigger is active before the trigger point and inactive after the trigger point.
                                                  */
} IfxStdIf_Timer_OutputEvent;

/** \brief Timer increment direction */
typedef enum
{
    IfxStdIf_Timer_CountDir_up,                      /**< \brief Timer is counting up */
    IfxStdIf_Timer_CountDir_upAndDown,               /**< \brief Timer is counting up and down */
    IfxStdIf_Timer_CountDir_down                     /**< \brief Timer is counting down */
} IfxStdIf_Timer_CountDir;

/** \brief Forward declaration */
typedef struct IfxStdIf_Timer_ IfxStdIf_Timer;

/** \brief Return the timer frequency in Hz
 * \param driver Pointer to the interface driver object
 * \return Return the timer frequency in Hz
 */
typedef float32 (*IfxStdIf_Timer_GetFrequency)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the timer period in ticks
 * \param driver Pointer to the interface driver object
 * \return Return the timer period in ticks
 */
typedef Ifx_TimerValue (*IfxStdIf_Timer_GetPeriod)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the timer resolution in seconds
 * \param driver Pointer to the interface driver object
 * \return Return the timer resolution in seconds
 */
typedef float32 (*IfxStdIf_Timer_GetResolution)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the timer trigger point
 * \param driver Pointer to the interface driver object
 * \return Return the timer timer point
 */
typedef Ifx_TimerValue (*IfxStdIf_Timer_GetTrigger)(IfxStdIf_InterfaceDriver driver);


/** \brief Set the timer frequency in Hz
 * \param driver Pointer to the interface driver object
 * \param frequency Requested timer frequency in Hz
 * \retval TRUE The requested frequency could be set
 * \retval FALSE The requested frequency is out of range
 */
typedef boolean (*IfxStdIf_Timer_SetFrequency)(IfxStdIf_InterfaceDriver driver, float32 frequency);

/** \brief Indicates the driver that the imput frequency has changed and that it should be taken in account
 * \param driver Pointer to the interface driver object
 * \return None
 */
typedef void (*IfxStdIf_Timer_UpdateInputFrequency)(IfxStdIf_InterfaceDriver driver);

/** \brief Apply requested updates.
 *
 * Such requested update are calls to:
 * - IfxStdIf_Timer_setSingleMode()
 * - IfxStdIf_Timer_setTrigger()
 * - IfxStdIf_Timer_setPeriod() or IfxStdIf_Timer_setFrequency()
 * as well as PWM generation related calls such as:
 * - IfxStdIf_PwmHl_setDeadtime()
 * - IfxStdIf_PwmHl_setMode()
 * - IfxStdIf_PwmHl_setOnTime()
 * - IfxStdIf_PwmHl_setupChannels()
 *
 * It is important to note, that only one call to Timer_applyUpdate() is allowed within
 * one timer period!!!
 * In order to ensure correct update synchronisation, disableUpdate() should have been called previously.
 *
 * \param driver Pointer to the interface driver object
 * \return None
 */
typedef void (*IfxStdIf_Timer_ApplyUpdate)(IfxStdIf_InterfaceDriver driver);

/** \brief Disable updates
 *
 * The following API action will not be taken in account before applyUpdate is called:
 * - IfxStdIf_Timer_setSingleMode()
 * - IfxStdIf_Timer_setTrigger()
 * - IfxStdIf_Timer_setPeriod() or IfxStdIf_Timer_setFrequency()
 * as well as PWM generation related calls such as:
 * - IfxStdIf_PwmHl_setDeadtime()
 * - IfxStdIf_PwmHl_setMode()
 * - IfxStdIf_PwmHl_setOnTime()
 * - IfxStdIf_PwmHl_setupChannels()
 *
 * \param driver Pointer to the interface driver object
 * \return None
 */
typedef void (*IfxStdIf_Timer_DisableUpdate)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the timer input frequency in Hz
 * \param driver Pointer to the interface driver object
 * \return Return the timer input frequency in Hz
 */
typedef float32 (*IfxStdIf_Timer_GetInputFrequency)(IfxStdIf_InterfaceDriver driver);

/** \brief Immediately start the timer.
 *
 * Please note that other mechanism may avoid the timer to tick, such as when the
 * global (input) clock to the timer block is inactive.
 * \param driver Pointer to the interface driver object
 * \return none
 */
typedef void (*IfxStdIf_Timer_Run)(IfxStdIf_InterfaceDriver driver);

/** \brief Set the timer period.
 *
 * Timer_applyUpdate() shall be invoked in order this to take effect.
 * \param driver Pointer to the interface driver object
 * \param period Period value in ticks
 * \retval TRUE The requested period could be set
 * \retval FALSE The requested period is out of range
 */
typedef boolean (*IfxStdIf_Timer_SetPeriod)(IfxStdIf_InterfaceDriver driver, Ifx_TimerValue period);

/** \brief Stop timer at the end of the period
 *
 * Timer_applyUpdate() shall be invoked in order this to take effect.
 * \param driver Pointer to the interface driver object
 * \param enabled If TRUE, enable the single mode, else disable the single mode (continuous mode)
 * \return none
 */
typedef void (*IfxStdIf_Timer_SetSingleMode)(IfxStdIf_InterfaceDriver driver, boolean enabled);

/** \brief Set trigger point.
 *
 * \see Timer_OutputEvent for the definition of the output level.
 * Active state is defined by Timer_Config::outputActiveState.
 *
 * Timer_applyUpdate() shall be invoked in order this to take effect.
 * \param driver Pointer to the interface driver object
 * \param triggerPoint Trigger point in ticks from the period start.
 * \return none
 */
typedef void (*IfxStdIf_Timer_SetTrigger)(IfxStdIf_InterfaceDriver driver, Ifx_TimerValue triggerPoint);

/** \brief Immediately stops the timer
 * \param driver Pointer to the interface driver object
 * \return none
 */
typedef void (*IfxStdIf_Timer_Stop)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the timer interrupt flag and reset it
 * \param driver Pointer to the interface driver object
 * \return Return the timer interrupt flag
 */
typedef boolean (*IfxStdIf_Timer_AckTimerIrq)(IfxStdIf_InterfaceDriver driver);

/** \brief Return the trigger interrupt flag and reset it
 * \param driver Pointer to the interface driver object
 * \return Return the trigger interrupt flag
 */
typedef boolean (*IfxStdIf_Timer_AckTriggerIrq)(IfxStdIf_InterfaceDriver driver);

/** \brief Standard interface object
 */
struct IfxStdIf_Timer_
{
    IfxStdIf_InterfaceDriver            driver;                  /**< \brief Interface driver object                  */
    IfxStdIf_Timer_GetFrequency         getFrequency;            /**< \brief \see IfxStdIf_Timer_GetFrequency         */
    IfxStdIf_Timer_GetPeriod            getPeriod;               /**< \brief \see IfxStdIf_Timer_GetPeriod            */
    IfxStdIf_Timer_GetResolution        getResolution;           /**< \brief \see IfxStdIf_Timer_GetResolution        */
    IfxStdIf_Timer_GetTrigger           getTrigger;              /**< \brief \see IfxStdIf_Timer_GetTrigger           */
    IfxStdIf_Timer_SetFrequency         setFrequency;            /**< \brief \see IfxStdIf_Timer_SetFrequency         */
    IfxStdIf_Timer_UpdateInputFrequency updateInputFrequency;    /**< \brief \see IfxStdIf_Timer_UpdateInputFrequency */
    IfxStdIf_Timer_ApplyUpdate          applyUpdate;             /**< \brief \see IfxStdIf_Timer_ApplyUpdate          */
    IfxStdIf_Timer_DisableUpdate        disableUpdate;           /**< \brief \see IfxStdIf_Timer_DisableUpdate        */
    IfxStdIf_Timer_GetInputFrequency    getInputFrequency;       /**< \brief \see IfxStdIf_Timer_GetInputFrequency    */
    IfxStdIf_Timer_Run                  run;                     /**< \brief \see IfxStdIf_Timer_Run                  */
    IfxStdIf_Timer_SetPeriod            setPeriod;               /**< \brief \see IfxStdIf_Timer_SetPeriod            */
    IfxStdIf_Timer_SetSingleMode        setSingleMode;           /**< \brief \see IfxStdIf_Timer_SetSingleMode        */
    IfxStdIf_Timer_SetTrigger           setTrigger;              /**< \brief \see IfxStdIf_Timer_SetTrigger           */
    IfxStdIf_Timer_Stop                 stop;                    /**< \brief \see IfxStdIf_Timer_Stop                 */
    IfxStdIf_Timer_AckTimerIrq          ackTimerIrq;             /**< \brief \see IfxStdIf_Timer_AckTimerIrq          */
    IfxStdIf_Timer_AckTriggerIrq        ackTriggerIrq;           /**< \brief \see IfxStdIf_Timer_AckTriggerIrq        */
};

/** \brief Trigger configuration */
typedef struct
{
    boolean            enabled;             /**< \brief If true, the trigger functionality is Initialised, else ignored */
    Ifx_TimerValue     triggerPoint;        /**< \brief Trigger point in timer ticks */
    Ifx_Priority       isrPriority;         /**< \brief Interrupt isrPriority of the trigger interrupt, if 0 the interrupt is disable */
    IfxSrc_Tos         isrProvider;         /**< \brief Interrupt service provider for the trigger interrupt */
    IfxPort_OutputMode outputMode;          /**< \brief Output mode *//* FIXME use generic type (No specific tricore AURIX type ) */
    IfxPort_PadDriver  outputDriver;        /**< \brief Output pad driver *//* FIXME use generic type (No specific tricore AURIX type ) */
    boolean            risingEdgeAtPeriod;  /**< \brief Set the clock signal polarity, if TRUE, the rising edge is at the period, else at the trigger offset. When the timer is stopped, the output is set to high */
    boolean            outputEnabled;       /**< \brief If TRUE, the output pin is enabled, else disabled. In case the output is disabled, the output pin is not initialized. */
} IfxStdIf_Timer_TrigConfig;

/** \brief Timer configuration */
typedef struct
{
    float32                   frequency;     /**< \brief PWM frequency in Hz. This parameter is only used to initialise the timer structure. An additional cell is required to build the timer. */
    Ifx_Priority              isrPriority;   /**< \brief Interrupt isrPriority of the timer interrupt, if 0 the interrupt is disable */
    IfxSrc_Tos                isrProvider;   /**< \brief Interrupt service provider for the timer interrupt */
    float32                   minResolution; /**< \brief Minimum resolution of the timer in seconds. if 0, this parameter is ignored. If the configuration does not enable this setting a warning is given */
    IfxStdIf_Timer_TrigConfig trigger;       /**< \brief Trigger configuration */
    IfxStdIf_Timer_CountDir   countDir;      /**< \brief Timer counting mode */
    float32 				  startOffset;   /**< \brief FIXME make startOffset as Ifx_TimerValue. Timer initial offset in % of the period */
} IfxStdIf_Timer_Config;

/** \addtogroup library_srvsw_stdif_timer
 *  \{
 */
/** \copydoc IfxStdIf_Timer_GetFrequency */
IFX_INLINE float32 IfxStdIf_Timer_getFrequency(IfxStdIf_Timer *stdIf)
{
    return stdIf->getFrequency(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_GetPeriod */
IFX_INLINE Ifx_TimerValue IfxStdIf_Timer_getPeriod(IfxStdIf_Timer *stdIf)
{
    return stdIf->getPeriod(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_GetTrigger */
IFX_INLINE Ifx_TimerValue IfxStdIf_Timer_getTrigger(IfxStdIf_Timer *stdIf)
{
    return stdIf->getTrigger(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_GetResolution */
IFX_INLINE float32 IfxStdIf_Timer_getResolution(IfxStdIf_Timer *stdIf)
{
    return stdIf->getResolution(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_SetFrequency */
IFX_INLINE boolean IfxStdIf_Timer_setFrequency(IfxStdIf_Timer *stdIf, float32 frequency)
{
    return stdIf->setFrequency(stdIf->driver, frequency);
}


/** \copydoc IfxStdIf_Timer_UpdateInputFrequency */
IFX_INLINE void IfxStdIf_Timer_updateInputFrequency(IfxStdIf_Timer *stdIf)
{
    stdIf->updateInputFrequency(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_ApplyUpdate */
IFX_INLINE void IfxStdIf_Timer_applyUpdate(IfxStdIf_Timer *stdIf)
{
    stdIf->applyUpdate(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_DisableUpdate */
IFX_INLINE void IfxStdIf_Timer_disableUpdate(IfxStdIf_Timer *stdIf)
{
    stdIf->disableUpdate(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_GetInputFrequency */
IFX_INLINE float32 IfxStdIf_Timer_getInputFrequency(IfxStdIf_Timer *stdIf)
{
    return stdIf->getInputFrequency(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_Run */
IFX_INLINE void IfxStdIf_Timer_run(IfxStdIf_Timer *stdIf)
{
    stdIf->run(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_SetPeriod */
IFX_INLINE boolean IfxStdIf_Timer_setPeriod(IfxStdIf_Timer *stdIf, Ifx_TimerValue period)
{
    return stdIf->setPeriod(stdIf->driver, period);
}


/** \copydoc IfxStdIf_Timer_SetSingleMode */
IFX_INLINE void IfxStdIf_Timer_setSingleMode(IfxStdIf_Timer *stdIf, boolean enabled)
{
    stdIf->setSingleMode(stdIf->driver, enabled);
}


/** \copydoc IfxStdIf_Timer_SetTrigger */
IFX_INLINE void IfxStdIf_Timer_setTrigger(IfxStdIf_Timer *stdIf, Ifx_TimerValue triggerPoint)
{
    stdIf->setTrigger(stdIf->driver, triggerPoint);
}


/** \copydoc IfxStdIf_Timer_Stop */
IFX_INLINE void IfxStdIf_Timer_stop(IfxStdIf_Timer *stdIf)
{
    stdIf->stop(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_AckTimerIrq */
IFX_INLINE boolean IfxStdIf_Timer_ackTimerIrq(IfxStdIf_Timer *stdIf)
{
    return stdIf->ackTimerIrq(stdIf->driver);
}


/** \copydoc IfxStdIf_Timer_AckTriggerIrq */
IFX_INLINE boolean IfxStdIf_Timer_ackTriggerIrq(IfxStdIf_Timer *stdIf)
{
    return stdIf->ackTriggerIrq(stdIf->driver);
}


/** \brief Convert timer ticks to seconds
 * \param  clockFreq Timer clock frequency
 * \param  ticks time value in ticks to be converted
 * \return Return the converted time in s
 */
IFX_INLINE float32 IfxStdIf_Timer_tickToS(float32 clockFreq, Ifx_TimerValue ticks)
{
    return ticks / clockFreq;
}


/** \brief Convert seconds to timer ticks
 * \param  clockFreq Timer clock frequency
 * \param  seconds time value in seconds to be converted
 * \return Return the converted time in timer ticks
 */
IFX_INLINE Ifx_TimerValue IfxStdIf_Timer_sToTick(float32 clockFreq, float32 seconds)
{
    return seconds * clockFreq;
}


/** \} */

/** Initialize the configuration structure to default
 *
 * \param config Timer configuration. This parameter is initialised by the function
 *
 */
void IfxStdIf_Timer_initConfig(IfxStdIf_Timer_Config *config);

#endif /* IFXSTDIF_TIMER_H_ */

