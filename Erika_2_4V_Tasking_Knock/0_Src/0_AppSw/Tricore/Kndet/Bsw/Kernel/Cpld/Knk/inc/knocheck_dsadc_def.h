/*
 * knocheck_def.h
 *
 *  Created on: 2015. 2. 27.
 *      Author: hansteve
 */

#ifndef KNOCHECK_DEF_H_
#define KNOCHECK_DEF_H_


/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxSrc.h"/*For interrupt control*/
#include "IfxDsadc_PinMap.h"
#include "IfxDsadc_reg.h"
#include "IfxCpu_Intrinsics.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Dsadc_Std_Enum
 * \{ */
/** \brief Comb Filter (auxiliary) shift control\n
 * Definition in Ifx_DSADC.FCFGA.B.AFSC
 */
typedef enum
{
    AuxCombFilterShift_noShift  = 0,  /**< \brief no shift, use full range */
    AuxCombFilterShift_shiftBy1 = 1,  /**< \brief Shift by 1 */
    AuxCombFilterShift_shiftBy2 = 2,  /**< \brief Shift by 2 */
    AuxCombFilterShift_shiftBy3 = 3   /**< \brief Shift by 3 */
} AuxCombFilterShift;

/** \brief Comb Filter (auxiliary) configuration/type\n
 * Definition in Ifx_DSADC.FCFGA.B.CFAC
 */
typedef enum
{
    AuxCombFilterType_comb1 = 0,  /**< \brief CIC1 */
    AuxCombFilterType_comb2 = 1,  /**< \brief CIC2 */
    AuxCombFilterType_comb3 = 2,  /**< \brief CIC3 */
    AuxCombFilterType_combF = 3   /**< \brief CICF */
} AuxCombFilterType;

/** \brief Service request generation (auxiliary)\n
 * Definition in Ifx_DSADC.FCFGA.B.ESEL
 */
typedef enum
{
    AuxEvent_everyNewResult  = 0, /**< \brief Always, for each new result value */
    AuxEvent_insideBoundary  = 1, /**< \brief If result is inside the boundary band */
    AuxEvent_outsideBoundary = 2  /**< \brief If result is outside the boundary band */
} AuxEvent;

/** \brief Service request generation (auxiliary)\n
 * Definition in Ifx_DSADC.FCFGA.B.EGT
 */
typedef enum
{
    AuxGate_definedByESEL       = 0, /**< \brief Separate: generate events according to ESEL */
    AuxGate_coupledToIntegrator = 1  /**< \brief Coupled: generate events only when the integrator is enabled and after the discard phase defined by bitfield NVALDIS */
} AuxGate;

/** \brief Service request generation (auxiliary)\n
 * Definition in Ifx_DSADC.FCFGA.B.SRGA
 */
typedef enum
{
    AuxServiceRequest_never     = 0,  /**< \brief Never, service requests disabled */
    AuxServiceRequest_auxFilter = 1,  /**< \brief Auxiliary filter: As selected by bitfield ESEL (\ref AuxEvent) */
    AuxServiceRequest_altSource = 2   /**< \brief Alternate source: Capturing of a sign delay value to register CGSYNCx (x = 0 - 5) */
} AuxServiceRequest;

/** \brief Carrier generation mode\n
 * Definition in Ifx_DSADC.CGCFG.B.CGMOD
 */
typedef enum
{
    CarrierWaveformMode_stopped  = 0,  /**< \brief Carrier Generator stopped */
    CarrierWaveformMode_square   = 1,  /**< \brief Carrier Generator generates square wave */
    CarrierWaveformMode_triangle = 2,  /**< \brief Carrier Generator generates triangle wave */
    CarrierWaveformMode_sine     = 3   /**< \brief Carrier Generator generates sine wave */
} CarrierWaveformMode;

/** \brief Specifies the channel Index
 */
typedef enum
{
    ChannelId_0 = 0,  /**< \brief Specifies the channel Index 0 */
    ChannelId_1 = 1,  /**< \brief Specifies the channel Index 1 */
    ChannelId_2 = 2,  /**< \brief Specifies the channel Index 2 */
    ChannelId_3 = 3,  /**< \brief Specifies the channel Index 3 */
    ChannelId_4 = 4,  /**< \brief Specifies the channel Index 4 */
    ChannelId_5 = 5  /**< \brief Specifies the channel Index 5 */
} ChannelId;

/** \brief Modulator common mode voltage selection\n
 * Definition in Ifx_DSADC.MODCFGx.B.CMVS
 */
typedef enum
{
    CommonModeVoltage_a = 0,  /**< \brief VCM = VAREF / 3.03 (1.65 V for VAREF = 5.0 V), recommended for VDDM = 3.3 V1.65V */
    CommonModeVoltage_b = 1,  /**< \brief VCM = VAREF / 2.27 (2.2 V for VAREF = 5.0 V), recommended for low distortion of AC-coupled signals */
    CommonModeVoltage_c = 2   /**< \brief VCM = VAREF / 2.0 (2.5 V for VAREF = 5.0 V), recommended for DC-coupled signals */
} CommonModeVoltage;

/** \brief Modulator Configuration\n
 * Definition in Ifx_DSADC.MODCFGx.B.MCFG
 */
typedef enum
{
	High_NoAntialias	= 0,  /**< \brief High-performance mode active */
	High_Antialias		= 1,  /**< \brief High-performance mode active and anti-alias filter enabled*/
	Stand_NoAntialias	= 2,  /**< \brief Standard-performance mode active */
	Stand_Antialias		= 3   /**< \brief Standard-performance mode active and anti-alias filter enabled*/
} Modulatorcfg;

/** \brief Modulator Gain Calibration Enable\n
 * Definition in Ifx_DSADC.MODCFGx.B.GCEN
 */
typedef enum
{
	No_gain_cal	= 0,	/**< \brief Normal operation */
	Gain_cal	= 1	/**< \brief On-chip gain calibration mode enabled*/
} Gaincal;

/** \brief Modulator Gain Calibration Enable\n
 * Definition in Ifx_DSADC.MODCFGx.B.APC
 */
typedef enum
{
	Auto_off	= 0,	/**< \brief Off: Modulator active while its associated bit MxRUN is set */
	Auto_on		= 1		/**< \brief On: Modulator active while MxRUN is set and the gate signal (selected trigger) is active high*/
} AutoPower;

/** \brief FIR data shift control\n
 * Selects the displacement caused by the data shifter at the FIR filter output\n
 * Definition in Ifx_DSADC.FCFGM.B.DSH
 */
typedef enum
{
    FirDataShift_noShift  = 0,  /**< \brief no shift, use full range */
    FirDataShift_shiftBy1 = 1,  /**< \brief Shift by 1 */
    FirDataShift_shiftBy2 = 2,  /**< \brief Shift by 2 */
    FirDataShift_shiftBy3 = 3   /**< \brief Shift by 3 */
} FirDataShift;

/** \brief FIR shift control\n
 * Selects the displacement caused by the data shifter inbetween the FIR filter blocks.\n
 * Definition in Ifx_DSADC.FCFGM.B.FSH
 */
typedef enum
{
    FirInternalShift_noShift  = 0, /**< \brief no shift, use full range */
    FirInternalShift_shiftBy1 = 1  /**< \brief Shift by 1 */
} FirInternalShift;

/** \brief Modulator configuration of positive/negative input line\n
 * Definition in Ifx_DSADC.MODCFGx.B.INCFGP and Ifx_DSADC.MODCFGx.B.INCFGN
 */
typedef enum
{
    InputConfig_inputPin          = 0,  /**< \brief Modulator input connected to external pin */
    InputConfig_supplyVoltage     = 1,  /**< \brief Modulator input connected to supply voltage V_ddm */
    InputConfig_commonModeVoltage = 2,  /**< \brief Modulator input connected to common mode voltage V_cm */
    InputConfig_referenceGround   = 3   /**< \brief Modulator input connected to reference ground V_ref */
} InputConfig;

/** \brief Demodulator input data source selection\n
 * Definition in Ifx_DSADC.DICFG.B.DSRC
 */
typedef enum
{
    InputDataSource_onChipStandAlone = 0,  /**< \brief On-chip modulator, standalone (3rd order) */
    InputDataSource_onChipCombined   = 1,  /**< \brief On-chip modulator, yield (2nd order) */
    InputDataSource_directInputA     = 2,  /**< \brief External, from input A, direct */
    InputDataSource_invertedInputA   = 3,  /**< \brief External, from input A, inverted */
    InputDataSource_directInputB     = 4,  /**< \brief External, from input B, direct */
    InputDataSource_invertedInputB   = 5   /**< \brief External, from input B, inverted */
} InputDataSource;

/** \brief Modulator gain select of analog input path\n
 * Definition in Ifx_DSADC.MODCFGx.B.GAINSEL
 */
typedef enum
{
    InputGain_factor1  = 0, /**< \brief Input gain factor: 1 */
    InputGain_factor2  = 1, /**< \brief Input gain factor: 2 */
    InputGain_factor4  = 2, /**< \brief Input gain factor: 4 */
    InputGain_factor8  = 3, /**< \brief Input gain factor: 8 */
    InputGain_factor16 = 4  /**< \brief Input gain factor: 16 */
} InputGain;

/** \brief Modulator input pin selection\n
 * Definition in Ifx_DSADC.MODCFGx.B.INMUX
 */
typedef enum
{
    InputPin_a = 0,  /**< \brief Pin A connected to modulator input */
    InputPin_b = 1,  /**< \brief Pin B connected to modulator input */
    InputPin_c = 2,  /**< \brief Pin C connected to modulator input */
    InputPin_d = 3   /**< \brief Pin D connected to modulator input */
} InputPin;

/** \brief Input Multiplexer Control Mode\n
 * Definition in Ifx_DSADC.MODCFGx.B.INMODE
 */
typedef enum
{
    Softcontrol 	= 0,  /**< \brief Software control */
    Trig_falling	= 1,  /**< \brief Trigger event upon a falling edge */
    Trig_rising		= 2,  /**< \brief Trigger event upon a rising edge */
    Trig_both		= 3   /**< \brief Trigger event upon any edge */
} InputMode;

/** \brief Input Multiplexer Action Control\n
 * Definition in Ifx_DSADC.MODCFGx.B.INMAC
 */
typedef enum
{
	Preset 	= 0,  		/**< \brief Preset mode(load INMUX upon a trigger)*/
    Single_step	= 1  	/**< \brief Single-step mode (decrement INMUX upon a trigger, wrap around to <INSEL>) */
} InputMAC;

/** \brief Integrator window size\n
 * Definition in Ifx_DSADC.IWCTR.B.IWS
 */
typedef enum
{
    IntegrationWindowSize_internalControl = 0,  /**< \brief Internal control: stop integrator after REPVAL+1 integration cycles */
    IntegrationWindowSize_externalControl = 1   /**< \brief External control: stop integrator when bit INTEN becomes 0 */
} IntegrationWindowSize;

/** \brief Integrator trigger mode\n
 * NOTE: switch-first to bypassed before using other mode\n
 * Definition in Ifx_DSADC.DICFG.B.ITRMODE
 */
typedef enum
{
    IntegratorTrigger_bypassed     = 0, /**< \brief No integration trigger, integrator bypassed */
    IntegratorTrigger_fallingEdge  = 1, /**< \brief Trigger event upon a falling edge */
    IntegratorTrigger_risingEdge   = 2, /**< \brief Trigger event upon a rising edge */
    IntegratorTrigger_alwaysActive = 3  /**< \brief No trigger, integrator active all the time */
} IntegratorTrigger;

/** \brief Low power supply voltage select\n
 * Definition in Ifx_DSADC.GLOBCFG.B.LOSUP
 */
typedef enum
{
    LowPowerSupply_5V   = 0, /**< \brief Supply Voltage for Analog Circuitry set to 5V */
    LowPowerSupply_3_3V = 1  /**< \brief Supply Voltage for Analog Circuitry set to 3.3V */
} LowPowerSupply;

/** \brief Comb Filter (Main Chain) shift control\n
 * Definition in Ifx_DSADC.FCFGC.B.MFSC
 */
typedef enum
{
    MainCombFilterShift_noShift  = 0,  /**< \brief no shift, use full range */
    MainCombFilterShift_shiftBy1 = 1,  /**< \brief Shift by 1 */
    MainCombFilterShift_shiftBy2 = 2,  /**< \brief Shift by 2 */
    MainCombFilterShift_shiftBy3 = 3   /**< \brief Shift by 3 */
} MainCombFilterShift;

/** \brief Comb Filter (Main Chain) configuration/type\n
 * Definition in Ifx_DSADC.FCFGC.B.CFMC
 */
typedef enum
{
    MainCombFilterType_comb1 = 0,  /**< \brief CIC1 */
    MainCombFilterType_comb2 = 1,  /**< \brief CIC2 */
    MainCombFilterType_comb3 = 2,  /**< \brief CIC3 */
    MainCombFilterType_combF = 3   /**< \brief CICF */
} MainCombFilterType;

/** \brief Service request generation (main chain)\n
 * Definition in Ifx_DSADC.FCFGC.B.SRGM
 */
typedef enum
{
    MainServiceRequest_never          = 0,  /**< \brief Never, service requests disabled */
    MainServiceRequest_highGateSignal = 1,  /**< \brief While gate (selected trigger signal) is high */
    MainServiceRequest_lowGateSignal  = 2,  /**< \brief While gate (selected trigger signal) is low */
    MainServiceRequest_everyNewResult = 3   /**< \brief Always, for each new result value */
} MainServiceRequest;

/** \brief Modulator clock select\n
 * Definition in Ifx_DSADC.GLOBCFG.B.MCSEL
 */
typedef enum
{
    ModulatorClock_off   = 0,  /**< \brief Internal clock off, no source selected */
    ModulatorClock_fDSD  = 1,  /**< \brief f_dsd clock selected */
    ModulatorClock_fERAY = 2,  /**< \brief f_eray clock selected */
    ModulatorClock_fOSC0 = 3   /**< \brief f_osc0 clock selected */
} ModulatorClock;

/** \brief Modulator divider factor for modulator clock\n
 * Definition in Ifx_DSADC.MODCFGx.B.DIVM
 */
typedef enum
{
    ModulatorClockDivider_div2 = 0,      /**< \brief f_mod = f_clk / 2  */
    ModulatorClockDivider_div4,          /**< \brief f_mod = f_clk / 4  */
    ModulatorClockDivider_div6,          /**< \brief f_mod = f_clk / 6  */
    ModulatorClockDivider_div8,          /**< \brief f_mod = f_clk / 8  */
    ModulatorClockDivider_div10,         /**< \brief f_mod = f_clk / 10  */
    ModulatorClockDivider_div12,         /**< \brief f_mod = f_clk / 12  */
    ModulatorClockDivider_div14,         /**< \brief f_mod = f_clk / 14  */
    ModulatorClockDivider_div16,         /**< \brief f_mod = f_clk / 16  */
    ModulatorClockDivider_div18,         /**< \brief f_mod = f_clk / 18  */
    ModulatorClockDivider_div20,         /**< \brief f_mod = f_clk / 20  */
    ModulatorClockDivider_div22,         /**< \brief f_mod = f_clk / 22  */
    ModulatorClockDivider_div24,         /**< \brief f_mod = f_clk / 24  */
    ModulatorClockDivider_div26,         /**< \brief f_mod = f_clk / 26  */
    ModulatorClockDivider_div28,         /**< \brief f_mod = f_clk / 28  */
    ModulatorClockDivider_div30,         /**< \brief f_mod = f_clk / 30  */
    ModulatorClockDivider_div32         /**< \brief f_mod = f_clk / 32  */
} ModulatorClockDivider;

/** \brief Rectifier sign source\n
 * Selects the sign signal that is to be delayed.\n
 * Definition in Ifx_DSADC.RECT.B.SSRC
 */
typedef enum
{
    RectifierSignSource_onChipGenerator = 0,  /**< \brief On-chip carrier generator */
    RectifierSignSource_nextChannel     = 1,  /**< \brief Sign of result of next channel */
    RectifierSignSource_externalA       = 2,  /**< \brief External sign signal A */
    RectifierSignSource_externalB       = 3   /**< \brief External sign signal B */
} RectifierSignSource;

/** \brief Demodulator sample clock source select\n
 * Definition in Ifx_DSADC.DICFG.B.CSRC
 */
typedef enum
{
    SampleClockSource_internal = 0,  /**< \brief Internal clock */
    SampleClockSource_inputA   = 1,  /**< \brief External clock, from Input A */
    SampleClockSource_inputB   = 2,  /**< \brief External clock, from Input B */
    SampleClockSource_inputC   = 3   /**< \brief External clock, from Input C */
} SampleClockSource;

/** \brief Demodulator data strobe generation mode\n
 * Definition in Ifx_DSADC.DICFG.B.STROBE
 */
typedef enum
{
    SampleStrobe_noDataStrobe            = 0, /**< \brief No data strobe */
    SampleStrobe_sampleOnRisingEdge      = 1, /**< \brief Direct clock, a sample trigger is generated at each rising clock edge */
    SampleStrobe_sampleOnFallingEdge     = 2, /**< \brief Direct clock, a sample trigger is generated at each falling clock edge */
    SampleStrobe_sampleOnBothEdges       = 3, /**< \brief Double data, a sample trigger is generated at each rising and falling clock edge */
    SampleStrobe_reserved                = 4, /**< \brief don't use */
    SampleStrobe_sampleOnTwoRisingEdges  = 5, /**< \brief Double clock, a sample trigger is generated at every 2nd rising clock edge */
    SampleStrobe_sampleOnTwoFallingEdges = 6  /**< \brief Double clock, a sample trigger is generated at every 2nd falling clock edge */
} SampleStrobe;

/** \brief Enable/disable the sensitivity of the module to sleep signal\n
 * Definition in Ifx_DSADC.CLC.B.EDIS
 */
typedef enum
{
    SleepMode_enable  = 0, /**< \brief enables sleep mode */
    SleepMode_disable = 1  /**< \brief disables sleep mode */
} SleepMode;

/** \brief Timestamp trigger mode\n
 * Definition in Ifx_DSADC.DICFG.B.TSTRMODE
 */
typedef enum
{
    TimestampTrigger_noTrigger   = 0,  /**< \brief No timestamp trigger */
    TimestampTrigger_fallingEdge = 1,  /**< \brief Trigger event upon a falling edge */
    TimestampTrigger_risingEdge  = 2,  /**< \brief Trigger event upon a rising edge */
    TimestampTrigger_eachEdge    = 3   /**< \brief Trigger event upon each edge */
} TimestampTrigger;

/** \brief Trigger select\n
 * Definition in Ifx_DSADC.DICFG.B.TRSEL
 */
typedef enum
{
    TriggerInput_a = 0,  /**< \brief dsadc trig 0 */
    TriggerInput_b = 1,  /**< \brief dsadc trig 1 */
    TriggerInput_c = 2,  /**< \brief vadc trig 0 */
    TriggerInput_d = 3,  /**< \brief vadc trig 1 */
    TriggerInput_e = 4,  /**< \brief external pin e */
    TriggerInput_f = 5,  /**< \brief external pin f */
    TriggerInput_g = 6,
    TriggerInput_h = 7
} TriggerInput;



/** \brief Write Control typedef*/
typedef enum
{
    No_Write  = 0, /**< \brief No write access to XXXXX parameters*/
    Can_Write = 1  /**< \brief Bitfields XXXXX can be written */
} WriteControl;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


/** \addtogroup Dsadc_DataStructures
 * \{ */
/** \brief Auxiliary comb filter configuration structure
 */
typedef struct
{
   boolean            bypassed;
   AuxCombFilterType  combFilterType;       /**< \brief Comb Filter (auxiliary) configuration/type */
   AuxCombFilterShift combFilterShift;      /**< \brief Comb Filter (auxiliary) shift control */
   AuxServiceRequest  serviceRequest;       /**< \brief Service request generation (auxiliary) */
   AuxEvent           eventSelect;          /**< \brief Service request generation (auxiliary) */
   AuxGate            eventGate;            /**< \brief Service request generation (auxiliary) */
   uint8              decimationFactor;     /**< \brief CIC Filter (Auxiliary) decimation factor (4 .. 32) */
}Dsadc_AuxFilterConfig;

/** \brief Channel Pins Configuration structure
 */

#if 0
typedef struct
{
    constCin_In *cin;         /**< \brief the CIN Pin which should be configured */
    IfxPort_InputMode      cinMode;     /**< \brief the CIN pin input mode which should be configured */
    constDin_In *din;         /**< \brief the DIN Pin which should be configured */
    IfxPort_InputMode      dinMode;     /**< \brief the DIN pin input mode which should be configured */
    constItr_In *itr;         /**< \brief the ITR Pin which should be configured */
    IfxPort_InputMode      itrMode;     /**< \brief the ITR pin input mode which should be configured */
    constDsn_In *dsn;         /**< \brief the DSN Pin which should be configured */
    IfxPort_InputMode      dsnMode;     /**< \brief the DSN pin output mode which should be configured */
    constDsp_In *dsp;         /**< \brief the DSP Pin which should be configured */
    IfxPort_InputMode      dspMode;     /**< \brief the DSP pin output mode which should be configured */
}Dsadc_ChannelPins;
#endif


/** \brief Comb filter configuration structure
 */
typedef struct
{
   boolean             bypassed;             /**< \brief CIC Filter enable/bypass selection */
   MainCombFilterType  combFilterType;       /**< \brief Comb Filter (Main Chain) configuration/type */
   MainCombFilterShift combFilterShift;      /**< \brief Comb Filter (Main Chain) shift control */
   MainServiceRequest  serviceRequest;       /**< \brief Service request generation (main chain) */
   uint16              decimationFactor;     /**< \brief 4 .. 256 */
   uint16              startValue;           /**< \brief 4 .. decimationFactor */
}Dsadc_CombFilterConfig;

/** \brief Demodulator configuration structure
 */
typedef struct
{
   InputDataSource   inputDataSource;        /**< \brief Demodulator input data source selection */
   TriggerInput      triggerInput;           /**< \brief Trigger selection */
   IntegratorTrigger integrationTrigger;     /**< \brief Integrator trigger mode selection */
   TimestampTrigger  timestampTrigger;       /**< \brief Timestamp trigger mode selection */
   SampleClockSource sampleClockSource;      /**< \brief Demodulator sample clock source selection */
   SampleStrobe      sampleStrobe;           /**< \brief Demodulator data strobe generation mode selection */
}Dsadc_DemodulatorConfig;

/** \brief FIR filters configuration structure
 */
typedef struct
{
   boolean			fir0Enabled;            /**< \brief FIR filter 0 enable/disable selection */
   boolean          fir1Enabled;            /**< \brief FIR filter 0 enable/disable selection */
   boolean          offsetCompensation;     /**< \brief Offset Compensation Filter enable/disable selection */
   FirDataShift     dataShift;              /**< \brief FIR data shift control selection */
   FirInternalShift internalShift;          /**< \brief FIR shift control selction */
}Dsadc_FirFilterConfig;

/** \brief Integrator configuration structure
 */
typedef struct
{
   IntegrationWindowSize	windowSize;            /**< \brief Integrator window size */
   uint16					discardCount;          /**< \brief Number of discarded values to after active trigger: 1 .. 64 */
   uint16					integrationCount;      /**< \brief Number of integrated values after discard step: 1 .. 64 */
   uint16					integrationCycles;     /**< \brief Number of integration cycles: 1 .. 16. Used only when \refIntegrationWindowSize_internalControl */
}Dsadc_IntegratorConfig;

/** \brief Modulator configuration structure
 */
typedef struct
{
   InputConfig			positiveInput;          /**< \brief Modulator configuration of positive input line */
   InputConfig			negativeInput;          /**< \brief Modulator configuration of negative input line */
   InputGain			inputGain;              /**< \brief Modulator gain select of analog input path */
   InputPin				inputPin;               /**< \brief Modulator input pin selection */
   InputMode			inputMode;				/**< \brief Input Multiplexer Control Mode */
   InputMAC				inputMAC;				/**< \brief Input Multiplexer Action Control */
   float32           	modulatorClockFreq;     /**< \brief Modulator clock frequency */
   CommonModeVoltage 	commonModeVoltage;      /**< \brief Modulator common mode voltage selection */
   Modulatorcfg			modulatorcfg;			/**< \brief Modulator Configuration */
   Gaincal				gaincal;				/**< \brief Gain Calibration Enable */
   AutoPower			autopower;				/**< \brief Automatic Power Control */
}Dsadc_ModulatorConfig;

/** \brief Rectifier configuration structure
 */
typedef struct
{
   boolean              enabled;        /**< \brief Rectification enable/disable selection */
   RectifierSignSource	signSource;     /**< \brief Rectifier sign source */
   uint8                signDelay;      /**< \brief Sign delay value */
   uint8				signPeriod;     /**< \brief Sign period */
} Dsadc_RectifierConfig;

/** \} */

/** \addtogroup IfxLld_Dsadc_Dsadc_DataStructures
 * \{ */
/** \brief DSADC handle data structure
 */
typedef struct
{
    Ifx_DSADC *dsadc;     /**< \brief Specifies the pointer to the DSADC module registers */
}Dsadc;

/** \brief Carrier generation configuration structure
 */
#if 0
typedef struct
{
   CarrierWaveformMode carrierWaveformMode;     /**< \brief Carrier generation mode */
    boolean                      inverted;                /**< \brief Signal polarity (Normal / Inverted) selection */
    boolean                      bitReversed;             /**< \brief Bit-Reverse PWM Generation (Normal / Bit reverse mode) selection */
    float32                      frequency;               /**< \brief Expected excitation frequency */
    constCgpwm_Out    *pinPos;                  /**< \brief Positive-carrier pin configuration */
    constCgpwm_Out    *pinNeg;                  /**< \brief Negative-carrier pin configuration */
    IfxPort_OutputMode           pinMode;                 /**< \brief Carrier pins output mode */
    IfxPort_PadDriver            pinDriver;               /**< \brief Carrier pins pad driver */
}Dsadc_CarrierGenConfig;
#endif

/** \brief Channel handle structure
 */
typedef struct
{
   Ifx_DSADC	*module;        /**< \brief Specifies the pointer to the DSADC module registers */
   Ifx_DSADC_CH	*channel;       /**< \brief Specifies the pointer to the DSADC channel registers */
   ChannelId 	channelId;     /**< \brief Channel Id */
}Dsadc_Channel;

/** \brief Single channel configuration structure
 */
typedef struct
{
   Ifx_DSADC				*module;          /**< \brief Specifies the pointer to the DSADC module registers */
   ChannelId                channelId;       /**< \brief Channel Id */
   Dsadc_ModulatorConfig    modulator;       /**< \brief Modulator configuration structure */
   Dsadc_DemodulatorConfig  demodulator;     /**< \brief Demodulator configuration structure */
   Dsadc_CombFilterConfig   combFilter;      /**< \brief Comb filter configuration structure */
   Dsadc_FirFilterConfig    firFilter;       /**< \brief FIR filters configuration structure */
   Dsadc_IntegratorConfig   integrator;      /**< \brief Integrator configuration structure */
   Dsadc_AuxFilterConfig    auxFilter;       /**< \brief Auxiliary comb filter configuration structure */
   Dsadc_RectifierConfig    rectifier;       /**< \brief Rectifier configuration structure */
   //constDsadc_ChannelPins *channelPins;     /**< \brief Channel Pins Configuration structure */
}Dsadc_ChannelConfig;

/** \brief Clock configuration data structure
 */
typedef struct
{
   Ifx_DSADC      *dsadc;                    /**< \brief Specifies the pointer to the DSADC module registers */
   ModulatorClock modulatorClockSelect;     /**< \brief Modulator clock selection */
   LowPowerSupply lowPowerSupply;           /**< \brief Low power supply voltage selection */
}Dsadc_Config;

typedef struct
{
    Dsadc_Channel dsadcChannels[IFXDSADC_NUM_CHANNELS];
} Dsadc_ChannelSetting;
//IFX_EXTERN DsadcBasic g_DsadcBasic;
#endif /* KNOCHECK_DEF_H_ */
