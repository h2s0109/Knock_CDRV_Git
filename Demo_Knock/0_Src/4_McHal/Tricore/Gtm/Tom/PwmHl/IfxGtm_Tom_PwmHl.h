/**
 * \file IfxGtm_Tom_PwmHl.h
 * \brief _TOM PWMHL details
 * \ingroup IfxLld_Gtm
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
 * \defgroup IfxLld_Gtm_Tom_PwmHl_Usage How to use the GTM TOM PWM Driver
 * \ingroup IfxLld_Gtm_Tom_PwmHl
 *
 *   This driver implements the PWM functionalities as defined by \ref library_srvsw_stdif_pwmhl.
 *   The user is free to use either the driver specific APIs below or to used the \ref library_srvsw_stdif_pwmhl "standard interface APIs".
 *
 * \section specific Specific implementation
 *   The PWM driver is a concatenation of a \ref IfxLld_Gtm_Tom_Timer "timer" with additional PWM generation cells.
 *
 *   The timer is implemented as described in \ref IfxLld_Gtm_Tom_Timer, it generate an internal trigger
 *   that is used to synchronize all the TOM channel used (shown in red in the pictures below).
 *
 *   The picture below presents two configuration examples:
 *   - On the left, the timer and trigger are generated using the same TOM channel CH0. The 3 dual-complementary
 *     PWMs, CCx and COUTx, are generated using 6 additional TOM channels (CH1 to Ch6).
 *   - On the right, the timer and the trigger uses separate TOM channels to allow the triggering of the ADC
 *     conversions. The order of the CCx and COUTx has been allocated freely between CH9 up to CH14.
 *
 *   \image html "IfxGtm_Tom_PwmHl-0.png" "PWM implementations on GTM TOM"
 *
 *
 *   Each of the TOM channel used for the generation of the CCx and COUTx signals are configured as in the figure below:
 *
 *   \image html "IfxGtm_Tom_PwmHl-1.png" "TOM channel configuration".
 *
 *   The internal trigger (red line) is generated by the \ref IfxLld_Gtm_Tom_Timer "timer" on timer rest and used by the
 *   TOM PWM channels to reset the counters CN0 and transfer the shadow values saved in SR0 and SR1 to the
 *   CM0 and CM1 registers. When the counter CN0 reach the values of the CM0 or CM1 compare registers,
 *   a rising or falling edge is generated on the output PWM signal. The signal active state configuration
 *   define which of the CM0 or CM1 is generating the falling or rising edge.
 *
 *   This trigger mechanism ensure that all the PWM channels, period and trigger are updated at the
 *   exact same time, avoiding any possibility for glitch or incoherent signals.
 *
 *   The clock use is the same as the timer clock.
 *
 *   In order to ensure a coherent update of all registers, the internal trigger must be disable before updating
 *   the timer, trigger and PWM duty cycles shadow values, and enabled once the update is done. The transfer will ocucrs at the next timer reset.
 *
 * \subsection features Features
 *   - This implementation allows from 1 up to 3 dual-complementary PWM channels.
 *   - Resources used:
 *     - Additionally to the linked timer resources used, the following resources are used:
 *       - 2 TOM channels per PWM channels
 *     - All TOM channels used must be own by the same TOM and TGC as for the linked timer
 *   - The TOM channel used for the timer must always have a lower index than the TOM channels
 *     used for the pwm.
 *   - All TOM channels used (including the timer and the trigger) must be contiguous but the order of the TOM channels is free.
 *
 *
 *   For a detailed configuration of the microcontroller, see \ref IfxGtm_Tom_PwmHl_init().
 *
 * \section example Usage Example
 *   Initialisation is done by, e.g:
 * \code
 *   IfxGtm_Tom_PwmHl_Config driverConfig;
 *   IfxGtm_Tom_PwmHl        driverData;
 *   IfxStdIf_PwmHl          pwmhl;
 *   IfxGtm_Tom_PwmHl_initConfig(&driverConfig, &MODULE_GTM);
 *   IfxGtm_Tom_PwmHl_init(&driverData, &driverConfig);
 *   IfxGtm_Tom_PwmHl_stdIfPwmHlInit(pwmhl, &driverData);
 * \endcode
 *
 *   During run-time, \ref library_srvsw_stdif_pwmhl "the interface functions" shall be used, e.g.:
 * \code
 *   IfxStdIf_Timer* timer = IfxStdIf_PwmHl_getTimer(pwmhl);
 *   Ifx_TimerValue onTime[3]; // assume configured for three HL channels
 *
 *   onTime[0] = 10;
 *   onTime[1] = 20;
 *   onTime[2] = 30;
 *
 *   IfxStdIf_Timer_disableUpdate(timer);
 *   IfxStdIf_Timer_setPeriod(timer, period);
 *   IfxStdIf_PwmHl_setOnTime(pwmhl, onTime);
 *   IfxStdIf_Timer_applyUpdate(timer);
 * \endcode
 *
 * \defgroup IfxLld_Gtm_Tom TOM Interface Drivers
 *
 * \defgroup IfxLld_Gtm_Tom_PwmHl TOM PWM Interface Driver
 * \ingroup IfxLld_Gtm_Tom
 * \defgroup IfxLld_Gtm_Tom_PwmHl_Data_Structures Data Structures
 * \ingroup IfxLld_Gtm_Tom_PwmHl
 * \defgroup IfxLld_Gtm_Tom_PwmHl_PwmHl_Functions PwmHl Functions
 * \ingroup IfxLld_Gtm_Tom_PwmHl
 * \defgroup IfxLld_Gtm_Tom_PwmHl_PwmHl_StdIf_Functions PwmHl StdIf Functions
 * \ingroup IfxLld_Gtm_Tom_PwmHl
 */

#ifndef IFXGTM_TOM_PWMHL_H
#define IFXGTM_TOM_PWMHL_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxStdIf_PwmHl.h"
#include "IfxGtm_Tom_Timer.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Maximal number of channels handled by the driver. One channel has a top and bottom pwm output
 * Limited to 3 because the PWM channels used must be owned by the same TGC
 */
#define IFXGTM_TOM_PWMHL_NUM_CHANNELS (3)

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/

typedef struct IfxGtm_Tom_PwmHl_s IfxGtm_Tom_PwmHl;

typedef void                    (*IfxGtm_Tom_PwmHl_Update)(IfxGtm_Tom_PwmHl *driver, Ifx_TimerValue *tOn);

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Tom_PwmHl_Data_Structures
 * \{ */
/** \brief Multi-channels PWM object definition (channels only)
 */
typedef struct
{
    Ifx_TimerValue  deadtime;             /**< \brief Dead time between the top and bottom channel in ticks */
    Ifx_TimerValue  minPulse;             /**< \brief minimum pulse that is output, shorter pulse time will be output as 0% duty cycle */
    Ifx_TimerValue  maxPulse;             /**< \brief internal parameter */
    Ifx_Pwm_Mode    mode;                 /**< \brief actual PWM mode */
    sint8           setMode;              /**< \brief A non zero flag indicates that the PWM mode is being modified */
    Ifx_ActiveState ccxActiveState;       /**< \brief Top PWM active state */
    Ifx_ActiveState coutxActiveState;     /**< \brief Bottom PWM active state */
    boolean         inverted;             /**< \brief Flag indicating the center aligned inverted mode (TRUE) */
    uint8           channelCount;         /**< \brief Number of PWM channels, one channel is made of a top and bottom channel */
} IfxGtm_Tom_PwmHl_Base;

/** \} */

/** \addtogroup IfxLld_Gtm_Tom_PwmHl_Data_Structures
 * \{ */
/** \brief GTM TOM: PWM HL configuration
 */
typedef struct
{
    IfxStdIf_PwmHl_Config      base;      /**< \brief PWM HL standard interface configuration */
    IfxGtm_Tom_Timer          *timer;     /**< \brief Pointer to the linked timer object */
    IfxGtm_Tom                 tom;       /**< \brief TOM unit used */
    const IfxGtm_Tom_ToutMapP *ccx;       /**< \brief Pointer to an array of size base.channelCount/2 containing the channels used. All channels used for ccx and coutx must be adjacent to the channel used for the timer, order is not important. */
    const IfxGtm_Tom_ToutMapP *coutx;     /**< \brief Pointer to an array of size base.channelCount/2 containing the channels used. All channels used for ccx and coutx must be adjacent to the channel used for the timer, order is not important */
} IfxGtm_Tom_PwmHl_Config;

/** \brief Structure for PWM configuration
 */
typedef struct
{
    boolean                 inverted;     /**< \brief Inverted configuration for the selected mode */
    IfxGtm_Tom_PwmHl_Update update;       /**< \brief update call back function for the selected mode */
} IfxGtm_Tom_PwmHl_Mode;

/** \brief GTM TOM PWM driver
 */
struct IfxGtm_Tom_PwmHl_s
{
    IfxGtm_Tom_PwmHl_Base   base;                                     /**< \brief Multi-channels PWM object definition (channels only) */
    IfxGtm_Tom_Timer       *timer;                                    /**< \brief Pointer to the linked timer object */
    IfxGtm_Tom_PwmHl_Update update;                                   /**< \brief Update function for actual selected mode */
    Ifx_GTM_TOM            *tom;                                      /**< \brief TOM unit used */
    Ifx_GTM_TOM_TGC        *tgc;                                      /**< \brief TGC unit used */
    IfxGtm_Tom_Ch           ccx[IFXGTM_TOM_PWMHL_NUM_CHANNELS];       /**< \brief TOM channels used for the CCCX outputs */
    IfxGtm_Tom_Ch           coutx[IFXGTM_TOM_PWMHL_NUM_CHANNELS];     /**< \brief TOM channels used for the OUTX outputs */
    IfxGtm_Tom_Ch          *ccxTemp;                                  /**< \brief cached value */
    IfxGtm_Tom_Ch          *coutxTemp;                                /**< \brief cached value */
};

/** \} */

/** \addtogroup IfxLld_Gtm_Tom_PwmHl_PwmHl_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializes the timer object
 * note To ensure that the channels counter are reset by the timer and do not overflow, leading to random signal on the output, the timer must be started before the call to this function.
 * Note: all PWM channels must use the same TGC
 * Note: the current implementation must use the same TGC for the timer and PWM channels
 * \param driver GTM TOM PWM driver
 * \param config GTM TOM: PWM HL configuration
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Tom_PwmHl_init(IfxGtm_Tom_PwmHl *driver, const IfxGtm_Tom_PwmHl_Config *config);

/** \brief Initialize the configuration structure to default
 * \param config Channel configuration. This parameter is Initialised by the function
 * \return None
 */
IFX_EXTERN void IfxGtm_Tom_PwmHl_initConfig(IfxGtm_Tom_PwmHl_Config *config);

/** \} */

/** \addtogroup IfxLld_Gtm_Tom_PwmHl_PwmHl_StdIf_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the dead time
 * \see IfxStdIf_PwmHl_GetDeadtime
 * \param driver GTM TOM PWM driver
 * \return Dead Time
 */
IFX_EXTERN float32 IfxGtm_Tom_PwmHl_getDeadtime(IfxGtm_Tom_PwmHl *driver);

/** \brief Returns the minimum pulse
 * \see IfxStdIf_PwmHl_GetMinPulse
 * \param driver GTM TOM PWM driver
 * \return Min Pulse
 */
IFX_EXTERN float32 IfxGtm_Tom_PwmHl_getMinPulse(IfxGtm_Tom_PwmHl *driver);

/** \brief Returns Pwm mode
 * \see IfxStdIf_PwmHl_GetMode
 * \param driver GTM TOM PWM driver
 * \return Pwm mode
 */
IFX_EXTERN Ifx_Pwm_Mode IfxGtm_Tom_PwmHl_getMode(IfxGtm_Tom_PwmHl *driver);

/** \brief Sets the dead time
 * \see IfxStdIf_PwmHl_setDeadtime
 * \param driver GTM TOM PWM driver
 * \param deadtime Dead time value
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Tom_PwmHl_setDeadtime(IfxGtm_Tom_PwmHl *driver, float32 deadtime);

/** \brief Sets the minimum pulse
 * \see IfxStdIf_PwmHl_setMinPulse
 * \param driver GTM TOM PWM driver
 * \param minPulse Minimum pulse
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Tom_PwmHl_setMinPulse(IfxGtm_Tom_PwmHl *driver, float32 minPulse);

/** \brief Sets the PWM mode, the mode is only applied after setOnTime() + applyUpdate()
 * \see IfxStdIf_PwmHl_SetMode
 * \param driver GTM TOM PWM driver
 * \param mode Pwm mode
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Tom_PwmHl_setMode(IfxGtm_Tom_PwmHl *driver, Ifx_Pwm_Mode mode);

/** \brief Sets the ON time
 * \see IfxStdIf_PwmHl_SetOnTime
 * \param driver GTM TOM PWM driver
 * \param tOn ON time
 * \return None
 */
IFX_EXTERN void IfxGtm_Tom_PwmHl_setOnTime(IfxGtm_Tom_PwmHl *driver, Ifx_TimerValue *tOn);

/** \brief Set up channels
 * \see IfxStdIf_PwmHl_SetupChannels
 * \param driver GTM TOM PWM driver
 * \param activeCh Active channel
 * \param stuckSt Stuck state
 * \return None
 */
IFX_EXTERN void IfxGtm_Tom_PwmHl_setupChannels(IfxGtm_Tom_PwmHl *driver, boolean *activeCh, boolean *stuckSt);

/** \brief Initialises the statndard interface Pwm
 * \param stdif Standard interface object, will be initialized by the function
 * \param driver Interface driver to be used by the standard interface. must be initialised separately
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxGtm_Tom_PwmHl_stdIfPwmHlInit(IfxStdIf_PwmHl *stdif, IfxGtm_Tom_PwmHl *driver);

/** \} */

#endif /* IFXGTM_TOM_PWMHL_H */