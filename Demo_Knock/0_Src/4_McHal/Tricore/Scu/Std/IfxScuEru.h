/**
 * \file IfxScuEru.h
 * \brief SCU  basic functionality
 * \ingroup IfxLld_Scu
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
 * \defgroup IfxLld_Scu_Std Standard Driver
 * \ingroup IfxLld_Scu
 * \defgroup IfxLld_Scu_Std_Enum ERU: Enumerations
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_External_Request_Selection ERU: External Request Selection
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_Event_Trigger_Logic ERU: Event Trigger Logic
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_Connecting_Matrix ERU: Connecting Matrix
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_Output_Gating_Unit ERU: Output Gating Unit
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_Data_Structures ERU: Data Structures
 * \ingroup IfxLld_Scu_Std
 */

#ifndef IFXSCUERU_H
#define IFXSCUERU_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxScu_cfg.h"
#include "IfxScu_reg.h"
#include "IfxCpu.h"
#include "IfxScu_PinMap.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Scu_Std_Enum
 * \{ */
/** \brief Input line selection for input channel\n
 * Definition in Ifx_SCU.EICR[i].B.EXIS(j) (i = 0,1,2,3 and j = 0,1)
 */
typedef enum
{
    IfxScuEru_ExternalInputSelection_0 = 0,      /**< \brief External input 0 is selected  */
    IfxScuEru_ExternalInputSelection_1,          /**< \brief External input 1 is selected  */
    IfxScuEru_ExternalInputSelection_2,          /**< \brief External input 2 is selected  */
    IfxScuEru_ExternalInputSelection_3,          /**< \brief External input 3 is selected  */
} IfxScuEru_ExternalInputSelection;

/** \brief Input channel for input selection  and conditioning of trigger or gating functions
 */
typedef enum
{
    IfxScuEru_InputChannel_0 = 0,      /**< \brief External Input channel 0  */
    IfxScuEru_InputChannel_1,          /**< \brief External Input channel 1  */
    IfxScuEru_InputChannel_2,          /**< \brief External Input channel 2  */
    IfxScuEru_InputChannel_3,          /**< \brief External Input channel 3  */
    IfxScuEru_InputChannel_4,          /**< \brief External Input channel 4  */
    IfxScuEru_InputChannel_5,          /**< \brief External Input channel 5  */
    IfxScuEru_InputChannel_6,          /**< \brief External Input channel 6  */
    IfxScuEru_InputChannel_7,          /**< \brief External Input channel 7  */
} IfxScuEru_InputChannel;

/** \brief Determines the destination (output channel) for trigger event (if enabled by Ifx_SCU.EICR[i].B.EIEN(j)).\n
 * Definition in Ifx_SCU.EICR[i].B.INP(j) (i = 0,1,2,3 and j = 0,1)
 */
typedef enum
{
    IfxScuEru_InputNodePointer_0 = 0,      /**< \brief Event from input ETLx triggers output OGU0 (signal TRx0)  */
    IfxScuEru_InputNodePointer_1,          /**< \brief Event from input ETLx triggers output OGU1 (signal TRx1)  */
    IfxScuEru_InputNodePointer_2,          /**< \brief Event from input ETLx triggers output OGU2 (signal TRx2)  */
    IfxScuEru_InputNodePointer_3,          /**< \brief Event from input ETLx triggers output OGU3 (signal TRx3)  */
    IfxScuEru_InputNodePointer_4,          /**< \brief Event from input ETLx triggers output OGU4 (signal TRx4)  */
    IfxScuEru_InputNodePointer_5,          /**< \brief Event from input ETLx triggers output OGU5 (signal TRx5)  */
    IfxScuEru_InputNodePointer_6,          /**< \brief Event from input ETLx triggers output OGU6 (signal TRx6)  */
    IfxScuEru_InputNodePointer_7,          /**< \brief Event from input ETLx triggers output OGU7 (signal TRx7)  */
} IfxScuEru_InputNodePointer;

/** \brief Determines the pattern detection influence on the ouput lines ERU_GOUTy and ERU_IOUTy\n
 * Definition in Ifx_SCU.IGCR[i].B.IGP(j) (i = 0,1,2,3 and j = 0,1)
 */
typedef enum
{
    IfxScuEru_InterruptGatingPattern_none         = 0,  /**< \brief IOUTy is inactive, (the pattern is not considered) */
    IfxScuEru_InterruptGatingPattern_alwaysActive = 1,  /**< \brief IOUTy is activated in response to a trigger event, (the pattern is not considered) */
    IfxScuEru_InterruptGatingPattern_patternMatch = 2,  /**< \brief IOUTy is activated if a trigger event occures while the pattern is present, (the pattern is considered) */
    IfxScuEru_InterruptGatingPattern_patternMiss  = 3   /**< \brief IOUTy is activated if a trigger event occures while the pattern is not present, (the pattern is not considered) */
} IfxScuEru_InterruptGatingPattern;

/** \brief Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 */
typedef enum
{
    IfxScuEru_OutputChannel_0 = 0,      /**< \brief Output channel 0  */
    IfxScuEru_OutputChannel_1,          /**< \brief Output channel 1  */
    IfxScuEru_OutputChannel_2,          /**< \brief Output channel 2  */
    IfxScuEru_OutputChannel_3,          /**< \brief Output channel 3  */
    IfxScuEru_OutputChannel_4,          /**< \brief Output channel 4  */
    IfxScuEru_OutputChannel_5,          /**< \brief Output channel 5  */
    IfxScuEru_OutputChannel_6,          /**< \brief Output channel 6  */
    IfxScuEru_OutputChannel_7,          /**< \brief Output channel 7  */
} IfxScuEru_OutputChannel;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Scu_Std_Data_Structures
 * \{ */
/** \brief Structure for external request input selection
 */
typedef struct
{
    IfxScuEru_InputChannel           inputChannel;     /**< \brief Input channel number */
    IfxScuEru_ExternalInputSelection inputSelect;
} IfxScuEru_ERS;

/** \} */

/** \addtogroup IfxLld_Scu_Std_External_Request_Selection
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initialises the External request Pin
 * \param req External request pin
 * \param inputMode Port Input mode
 * \return None
 */
IFX_INLINE void IfxScuEru_initReqPin(IfxScu_Req_In *req, IfxPort_InputMode inputMode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Gets the channel number and input selection information from request Id
 * \param requestId Request Id number
 * \param ers Structure for external request input selection to be filled
 * \return None
 */
IFX_EXTERN void IfxScuEru_getExternalInputSelection(uint8 requestId, IfxScuEru_ERS *ers);

/** \brief Determines which input line is selcted for input channel x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \param inputSignal Input line selection for input channel
 * \return None
 */
IFX_EXTERN void IfxScuEru_selectExternalInput(IfxScuEru_InputChannel inputChannel, IfxScuEru_ExternalInputSelection inputSignal);

/** \} */

/** \addtogroup IfxLld_Scu_Std_Event_Trigger_Logic
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears the external event flag of input channel x (INTFx)
 * \return None
 */
IFX_EXTERN void IfxScuEru_clearAllEventFlags(void);

/** \brief Clears the external event flag of input channel x (INTFx)
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_clearEventFlag(IfxScuEru_InputChannel inputChannel);

/** \brief Clears all the configuration for the given input channel x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_clearInputChannelConfiguration(IfxScuEru_InputChannel inputChannel);

/** \brief Disables the automatic clearing of INTFx when the edge of input channel which has not been selected, is detected
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_disableAutoClear(IfxScuEru_InputChannel inputChannel);

/** \brief Disables the falling edge of input channel to set the bit INTF x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_disableFallingEdgeDetection(IfxScuEru_InputChannel inputChannel);

/** \brief Disables the rising edge of input channel to set the bit INTF x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_disableRisingEdgeDetection(IfxScuEru_InputChannel inputChannel);

/** \brief Enables the automatic clearing of INTFx when the edge of input channel which has not been selected, is detected
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_enableAutoClear(IfxScuEru_InputChannel inputChannel);

/** \brief Enables the falling edge of input channel to set the bit INTF x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_enableFallingEdgeDetection(IfxScuEru_InputChannel inputChannel);

/** \brief Enables the rising edge of input channel to set the bit INTF x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel inputChannel);

/** \brief Returns the status of the flags of all input channels
 * \return All flags Status
 */
IFX_EXTERN uint32 IfxScuEru_getAllEventFlagsStatus(void);

/** \brief Returns the status of the external event flag of input channel x (INTFx)
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return Status (TRUE / FALSE)
 */
IFX_EXTERN boolean IfxScuEru_getEventFlagStatus(IfxScuEru_InputChannel inputChannel);

/** \brief Returns all the configuration for the given input channel x
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return Input channel configuration
 */
IFX_EXTERN uint32 IfxScuEru_getInputChannelConfiguration(IfxScuEru_InputChannel inputChannel);

/** \brief Sets the external event flag of input channel x (INTFx)
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_setEventFlag(IfxScuEru_InputChannel inputChannel);

/** \} */

/** \addtogroup IfxLld_Scu_Std_Connecting_Matrix
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Determines the destination (output channel) for trigger event (if enabled by Ifx_SCU.EICR[i].B.EIEN(j), i = 0,1,2,3 and j = 0,1).
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \param triggerSelect Trigger event selection
 * \return None
 */
IFX_EXTERN void IfxScuEru_connectTrigger(IfxScuEru_InputChannel inputChannel, IfxScuEru_InputNodePointer triggerSelect);

/** \brief Disables the generation of a trigger event for input channel x when the selected edge is detected
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_disableTriggerPulse(IfxScuEru_InputChannel inputChannel);

/** \brief Enables the generation of a trigger event for input channel x when the selected edge is detected
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \return None
 */
IFX_EXTERN void IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel inputChannel);

/** \brief Enables the flag INTFx to take part in the pattern detection for output gating
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \param inputChannel Input channel for input selection  and conditioning of trigger or gating functions
 * \param state FALSE: the bit INTFx does not take part in the pattern detection IPENjy = 0\n
 * TRUE : the bit INTFx is taken into consideration for the pattern detection IPENjy = 1
 * \return None
 */
IFX_EXTERN void IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel outputChannel, IfxScuEru_InputChannel inputChannel, boolean state);

/** \} */

/** \addtogroup IfxLld_Scu_Std_Output_Gating_Unit
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears all the configuration for the given input channel y
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \return None
 */
IFX_EXTERN void IfxScuEru_clearOutputChannelConfiguration(IfxScuEru_OutputChannel outputChannel);

/** \brief Disables the generation of a trigger event for output channel y when the result of the pattern detection changes
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \return None
 */
IFX_EXTERN void IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel outputChannel);

/** \brief Enables the generation of a trigger event for output channel y when the result of the pattern detection changes
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \return None
 */
IFX_EXTERN void IfxScuEru_enablePatternDetectionTrigger(IfxScuEru_OutputChannel outputChannel);

/** \brief Clears all the configuration for the given input channel y
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \return Output channel configuration
 */
IFX_EXTERN uint32 IfxScuEru_getOutputChannelConfiguration(IfxScuEru_OutputChannel outputChannel);

/** \brief Returns the status of the pattern detection result of output channel y (PDRy)
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \return Status (TRUE / FALSE)
 */
IFX_EXTERN boolean IfxScuEru_getPatternDetectionResult(IfxScuEru_OutputChannel outputChannel);

/** \brief Returns the whole pattern detection result of all the selcted output channels
 * \return Detected pattern
 */
IFX_EXTERN uint32 IfxScuEru_getWholePatternDetectionResult(void);

/** \brief Sets the gating pattern of a ouput channel y to determine how the pattern detection influences the output lines GOUT and IOUT
 * \param outputChannel Output channel for combination of events, definition of their effects and distribution to the system (interrupt generation, ...)
 * \param gatingPattern Interrupt gating pattern to determine how the pattern detection influences the ouput lines GOUT and IOUT
 * \return None
 */
IFX_EXTERN void IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel outputChannel, IfxScuEru_InterruptGatingPattern gatingPattern);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxScuEru_initReqPin(IfxScu_Req_In *req, IfxPort_InputMode inputMode)
{
    IfxPort_setPinModeInput(req->pin.port, req->pin.pinIndex, inputMode);

    IfxScuEru_ERS ers;
    IfxScuEru_getExternalInputSelection(req->requestId, &ers);
    IfxScuEru_selectExternalInput(ers.inputChannel, ers.inputSelect);
}


#endif /* IFXSCUERU_H */
