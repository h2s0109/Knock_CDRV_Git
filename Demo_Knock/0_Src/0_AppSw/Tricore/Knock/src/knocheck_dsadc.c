/*
 * knocheck_Dsadc.c
 *
 *  Created on: 2015. 2. 23.
 *      Author: hansteve
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <knocheck_priv.h>
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
void KnoCheck_Dsadc_Ini(void);
static void KnoCheck_Dsadc_IniGlob(void);
static void KnoCheck_Dsadc_IniChanDftCfg(Dsadc_ChannelConfig *config);
static void KnoCheck_Dsadc_IniChan(Dsadc_Channel *channel, const Dsadc_ChannelConfig *ChanDftCfg);
static void KnoCheck_Dsadc_IniModulator(Dsadc_Channel *channel, const Dsadc_ModulatorConfig *ChanDftCfg);
static void KnoCheck_Dsadc_IniDemodulator(Dsadc_Channel *channel, const Dsadc_DemodulatorConfig *ChanDftCfg);
static void KnoCheck_Dsadc_IniCombFilter(Dsadc_Channel *channel, const Dsadc_CombFilterConfig *ChanDftCfg);
static void KnoCheck_Dsadc_IniFirFilter(Dsadc_Channel *channel, const Dsadc_FirFilterConfig *ChanDftCfg);
static float32 KnoCheck_Dsadc_getSpbFrequency(void);
static sint32 KnoCheck_Dsadc_calcDIVx(float32 sourceFreq, float32 *targetFreq);
static void KnoCheck_Dsadc_IniIntegrator(Dsadc_Channel *channel, const Dsadc_IntegratorConfig *ChanDftCfg);
static void KnoCheck_Dsadc_IniAuxFilter(Dsadc_Channel *channel, const Dsadc_AuxFilterConfig *ChanDftCfg);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void KnoCheck_Dsadc_Ini(void)
{
	Dsadc_ChannelConfig cfgKC_DsadcChan;
	Dsadc_ChannelSetting iniKC_DsadcChan;

	/*Initialization GLOBCFG Register*/
	KnoCheck_Dsadc_IniGlob();
	/*Assign DSADC default value*/
	KnoCheck_Dsadc_IniChanDftCfg(&cfgKC_DsadcChan);
	/*SPB = 100MHz DividerFACTOR=8->Sampling Frequency=12.5MHZ Fd=312.50,Pass_band 104.17 COM=10 OSR FIR0=2 FIR1=2 SUM of OSR 40*/
	/* initialize channels */
	cfgKC_DsadcChan.channelId = (ChannelId)CHANNEL_DSADC;
	KnoCheck_Dsadc_IniChan(&iniKC_DsadcChan.dsadcChannels[CHANNEL_DSADC], &cfgKC_DsadcChan);


    /*Start DSADC*/
	DSADC_MOVE_ENGINE_RUN = TRUE;
	DSADC_CHANNEL_RUN = TRUE;
	DSADC_OFFM = 0XFC1F;
	SRC_DSADC_KNOCK.U = (3 << 11) | (1 << 10) | DMA_CHANNEL_SELECT; /* 3<11 => ON DMA, 1<<10 => Enable, DMA_CHANNEL_SELECT CONNECT DMA*/


}

static void KnoCheck_Dsadc_IniGlob(void)
{
	Ifx_DSADC *psDSADC = &MODULE_DSADC;
	Ifx_DSADC_GLOBCFG cfgKC_DsadcGlob;

	/*Clock setting, Can move by user*/
	uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(passwd);
	psDSADC->CLC.U = 0x00000000;
	if (psDSADC->CLC.U)
	{}                       // sync access
	IfxScuWdt_setCpuEndinit(passwd);

	/*DSADC setting*/
	cfgKC_DsadcGlob.B.MCSEL     = ModulatorClock_fDSD;
	cfgKC_DsadcGlob.B.LOSUP     = LowPowerSupply_5V;	/*normal power mode*/
	cfgKC_DsadcGlob.B.PSWC  	= Can_Write;

	psDSADC->GLOBCFG.U = cfgKC_DsadcGlob.U;
}

static void KnoCheck_Dsadc_IniChanDftCfg(Dsadc_ChannelConfig *KnoCheck_DSADC_ChanDftCfg)
{
	/*Assign_DSADC_modulator_value*/
	KnoCheck_DSADC_ChanDftCfg->modulator.positiveInput      	= InputConfig_inputPin;				/*DSADC_CH_MODCFG.B.INCFGP*/
	KnoCheck_DSADC_ChanDftCfg->modulator.negativeInput      	= InputConfig_inputPin;				/*DSADC_CH_MODCFG.B.INCFGN*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputGain         		= InputGain_factor1;				/*DSADC_CH_MODCFG.B.GAINSEL*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputPin           	= InputPin_a;						/*DSADC_CH_MODCFG.B.INSEL*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputMode           	= Softcontrol;						/*DSADC_CH_MODCFG.B.INMODE*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputMAC				= Preset;							/*DSADC_CH_MODCFG.B.INMAC*/
	KnoCheck_DSADC_ChanDftCfg->modulator.modulatorClockFreq 	= 12.5e6;							/*DSADC_CH_MODCFG.B.DIVM*/
	KnoCheck_DSADC_ChanDftCfg->modulator.commonModeVoltage  	= CommonModeVoltage_c;				/*DSADC_CH_MODCFG.B.CMVS*/
	KnoCheck_DSADC_ChanDftCfg->modulator.modulatorcfg  			= High_NoAntialias;					/*DSADC_CH_MODCFG.B.MCFG*/
	KnoCheck_DSADC_ChanDftCfg->modulator.gaincal  				= No_gain_cal;						/*DSADC_CH_MODCFG.B.GCEN*/
	KnoCheck_DSADC_ChanDftCfg->modulator.autopower				= Auto_on;							/*DSADC_CH_MODCFG.B.APC*/

	/*Assign_DSADC_Demodulator_value*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.inputDataSource	  = InputDataSource_onChipStandAlone;	/*DSADC_CH_DICFG.B.DSRC*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.triggerInput       = TriggerInput_b;						/*DSADC_CH_DICFG.B.TRSEL*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.integrationTrigger = IntegratorTrigger_bypassed;			/*DSADC_CH_DICFG.B.ITRMODE*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.timestampTrigger   = TimestampTrigger_noTrigger;			/*DSADC_CH_DICFG.B.TSTRMODE*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.sampleClockSource  = SampleClockSource_internal;			/*DSADC_CH_DICFG.B.CSRC*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.sampleStrobe       = SampleStrobe_sampleOnRisingEdge;	/*DSADC_CH_DICFG.B.STROBE*/

	/*Assign_DSADC_combfilter_value*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.bypassed         	= FALSE;								/*DSADC_CH_FCFGC.B.CFEN*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.combFilterType   	= MainCombFilterType_comb3;				/*DSADC_CH_FCFGC.B.CFMC*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.combFilterShift 	= MainCombFilterShift_shiftBy3;			/*DSADC_CH_FCFGC.B.MFSC*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.serviceRequest  	= MainServiceRequest_highGateSignal;	/*DSADC_CH_FCFGC.B.SRGM*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.decimationFactor	= 10;									/*DSADC_CH_FCFGC.B.CFMDF*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.startValue      	= 10;									/*DSADC_CH_FCFGC.B.CFMSV*/

	/*Assign_DSADC_FIRfilter_value*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.fir0Enabled       	= TRUE;									/*DSADC_CH_FCFGM.B.FIR0EN*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.fir1Enabled       	= TRUE;									/*DSADC_CH_FCFGM.B.FIR1EN*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.offsetCompensation = TRUE;									/*DSADC_CH_FCFGM.B.OCEN*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.dataShift         	= FirDataShift_shiftBy1; 				/*DSADC_CH_FCFGM.B.DSH*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.internalShift     	= FirDataShift_shiftBy1;       			/*DSADC_CH_FCFGM.B.FSH*/

	/*Assign_DSADC_Integrator_value*/
	KnoCheck_DSADC_ChanDftCfg->integrator.windowSize        = IntegrationWindowSize_internalControl;
	KnoCheck_DSADC_ChanDftCfg->integrator.discardCount      = 0;
	KnoCheck_DSADC_ChanDftCfg->integrator.integrationCount  = 20;
	KnoCheck_DSADC_ChanDftCfg->integrator.integrationCycles = 1;

	/*Assign_DSADC_AuxFilter_value*/
	KnoCheck_DSADC_ChanDftCfg->auxFilter.bypassed         	= TRUE;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.combFilterType   	= AuxCombFilterType_comb1;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.combFilterShift  	= AuxCombFilterShift_noShift;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.serviceRequest   	= AuxServiceRequest_never;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.eventSelect      	= AuxEvent_everyNewResult;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.eventGate        	= AuxGate_definedByESEL;
	KnoCheck_DSADC_ChanDftCfg->auxFilter.decimationFactor 	= 4;
}

static void KnoCheck_Dsadc_IniChan(Dsadc_Channel *channel, const Dsadc_ChannelConfig *ChanDftCfg)
{

    Ifx_DSADC *psDSADC = &MODULE_DSADC;
    channel->channelId = ChanDftCfg->channelId;
    channel->channel   = &psDSADC->CH[ChanDftCfg->channelId];/*Direct register control available*/

    KnoCheck_Dsadc_IniModulator(channel, &ChanDftCfg->modulator);
    KnoCheck_Dsadc_IniDemodulator(channel, &ChanDftCfg->demodulator);
    KnoCheck_Dsadc_IniCombFilter(channel, &ChanDftCfg->combFilter);
    KnoCheck_Dsadc_IniFirFilter(channel, &ChanDftCfg->firFilter);
    KnoCheck_Dsadc_IniIntegrator(channel, &ChanDftCfg->integrator);
    KnoCheck_Dsadc_IniAuxFilter(channel, &ChanDftCfg->auxFilter);
}

static void KnoCheck_Dsadc_IniModulator(Dsadc_Channel *channel, const Dsadc_ModulatorConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_MODCFG cfgMod;
    float32 targetFreq;
    float32 sourceFreq;

    /*collect_DSADC_Modulator_value*/

    cfgMod.U			= 0; 							/*Assign initial zero value*/

    /*--------------Input Functionality--------------*/
    cfgMod.B.INCFGP		= ChanDftCfg->positiveInput;	/*Configuration of Positive Input Line*/
    cfgMod.B.INCFGN		= ChanDftCfg->negativeInput;	/*Configuration of Negative Input Line*/
    cfgMod.B.GAINSEL	= ChanDftCfg->inputGain;		/*Gain Select of Analog Input Path*/
    cfgMod.B.INSEL		= ChanDftCfg->inputPin;			/*Input Pin Selection*/
    cfgMod.B.INMODE		= ChanDftCfg->inputMode;		/*Input Multiplexer Control Mode*/
    cfgMod.B.INMAC		= ChanDftCfg->inputMAC;			/*Input Multiplexer Action Control*/
    cfgMod.B.INCWC		= Can_Write;       				/*NCFGP, INCFGN, GAINSEL, INSEL,INMODE, INMAC can be written*/

    /*--------------Clock Functionality--------------*/
    targetFreq			= ChanDftCfg->modulatorClockFreq;/*Tagert frequency*/
    sourceFreq			= KnoCheck_Dsadc_getSpbFrequency();
    cfgMod.B.DIVM		= KnoCheck_Dsadc_calcDIVx(sourceFreq, &targetFreq);	/*Divider Factor for Modulator Clock*/
    cfgMod.B.DWC		= Can_Write; 					/*Bitfield DIVM can be written*/

    /*--------------ETC Functionality--------------*/
    cfgMod.B.CMVS		= ChanDftCfg->commonModeVoltage;/* Common Mode Voltage Selection*/
    cfgMod.B.MCFG		= ChanDftCfg->modulatorcfg; 	/*Modulator Configuration*/
    cfgMod.B.GCEN		= ChanDftCfg->gaincal;			/*Gain Calibration Enable*/
    cfgMod.B.APC		= ChanDftCfg->autopower;			/*Gain Calibration Enable*/
    cfgMod.B.MWC		= Can_Write; 					/*Bitfields CMVS, MCFG, GCEN, APC can be written*/

    (channel->channel)->MODCFG = cfgMod;				/* Register setting*/
}


static float32 KnoCheck_Dsadc_getSpbFrequency(void)
{
    Ifx_SCU	*psSCU = &MODULE_SCU;
    float32	oscFreq;
    float32 spbFrequency;
    float32 sourceFrequency;

    oscFreq = IFX_CFG_SCU_XTAL_FREQUENCY;
    sourceFrequency = (oscFreq * (psSCU->PLLCON0.B.NDIV + 1)) / ((psSCU->PLLCON1.B.K2DIV + 1) * (psSCU->PLLCON0.B.PDIV + 1));
    spbFrequency = sourceFrequency / psSCU->CCUCON0.B.SPBDIV;
    return spbFrequency;
}

static sint32 KnoCheck_Dsadc_calcDIVx(float32 sourceFreq, float32 *targetFreq)
{
    float32 bestError = 10e6;
    sint32  bestDiv   = 2, i;

    for (i = 2; i <= 32; i += 2)
    {
        float32 freq  = sourceFreq / i;
        float32 error = __absf(freq - *targetFreq);

        if (__leqf(error, bestError))
        {
            bestError = error;
            bestDiv   = i;

            if (!__neqf(error, 0))
            {
                break;
            }
        }
    }

    *targetFreq = sourceFreq / bestDiv;

    return (bestDiv / 2) - 1;
}

static void KnoCheck_Dsadc_IniDemodulator(Dsadc_Channel *channel, const Dsadc_DemodulatorConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_DICFG cfgDeMod;

    /*collect_DSADC_Demodulator_value*/

    cfgDeMod.U                   = 0;	/*Assign initial zero value*/

    /*--------------Input Functionality--------------*/
    cfgDeMod.B.DSRC              = ChanDftCfg->inputDataSource;	/*Input Data Source Select*/
    cfgDeMod.B.DSWC              = Can_Write; 						/*Bitfield DSRC can be written*/

    /*--------------Trigger Functionality--------------*/
    cfgDeMod.B.ITRMODE           = ChanDftCfg->integrationTrigger;	/*Integrator Trigger Mode*/
    cfgDeMod.B.TSTRMODE          = ChanDftCfg->timestampTrigger;	/*Timestamp Trigger Mode*/
    cfgDeMod.B.TRSEL             = ChanDftCfg->triggerInput;		/*Trigger Select*/
    cfgDeMod.B.TRWC              = Can_Write; 						/*Bitfields TRSEL, TSTRMODE, ITRMODE can be written*/

    /*--------------Sampling Functionality--------------*/
    cfgDeMod.B.CSRC              = ChanDftCfg->sampleClockSource;	/*Sample Clock Source Select*/
    cfgDeMod.B.STROBE            = ChanDftCfg->sampleStrobe;		/*Data Strobe Generation Mode*/
    cfgDeMod.B.SCWC              = Can_Write; 						/*Bitfields STROBE, CSRC can be written*/

    (channel->channel)->DICFG = cfgDeMod;							/* Register setting*/
}
static void KnoCheck_Dsadc_IniCombFilter(Dsadc_Channel *channel, const Dsadc_CombFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGC cfgComb;

    /*collect_DSADC_Combfilter_value*/

    cfgComb.U                   = 0;	/*Assign initial zero value*/

    cfgComb.B.CFMDF             = ChanDftCfg->decimationFactor - 1;
    cfgComb.B.CFMC              = ChanDftCfg->combFilterType;
    cfgComb.B.CFEN              = (ChanDftCfg->bypassed == FALSE) ? 1 : 0;

    cfgComb.B.MFSC              = ChanDftCfg->combFilterShift;

    cfgComb.B.SRGM              = ChanDftCfg->serviceRequest;
    cfgComb.B.CFMSV             = ChanDftCfg->startValue - 1;

    (channel->channel)->FCFGC = cfgComb;
}

static void KnoCheck_Dsadc_IniFirFilter(Dsadc_Channel *channel, const Dsadc_FirFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGM cfgFir;

    /*collect_DSADC_FIRfilter_value*/

    cfgFir.U                   = 0;	/*Assign initial zero value*/

    cfgFir.B.FIR0EN            = ChanDftCfg->fir0Enabled;
    cfgFir.B.FIR1EN            = ChanDftCfg->fir1Enabled;
    cfgFir.B.OCEN              = ChanDftCfg->offsetCompensation;
    cfgFir.B.DSH               = ChanDftCfg->dataShift;
    cfgFir.B.FSH               = ChanDftCfg->internalShift;

    (channel->channel)->FCFGM = cfgFir;
}

static void KnoCheck_Dsadc_IniIntegrator(Dsadc_Channel *channel, const Dsadc_IntegratorConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_IWCTR cfgInteg;

    /*collect_DSADC_FIRfilter_value*/

    cfgInteg.U                   = 0;									/*Assign initial zero value*/

    cfgInteg.B.REPVAL            = ChanDftCfg->integrationCycles - 1;	/*Number of Integration Cycles*/
    cfgInteg.B.NVALDIS           = ChanDftCfg->discardCount;			/*Number of Values Discarded*/
    cfgInteg.B.IWS               = ChanDftCfg->windowSize;				/*Integration Window Size*/
    cfgInteg.B.NVALINT           = ChanDftCfg->integrationCount - 1;	/*Number of Values Integrated*/

    (channel->channel)->IWCTR = cfgInteg;
}

static void KnoCheck_Dsadc_IniAuxFilter(Dsadc_Channel *channel, const Dsadc_AuxFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGA cfgAux;

    /*collect_DSADC_Auxfilter_value*/

    cfgAux.U                   = 0;									/*Assign initial zero value*/

    cfgAux.B.CFADF             = ChanDftCfg->decimationFactor - 1;	/*CIC Filter (Auxiliary) Decimation Factor*/
    cfgAux.B.CFAC              = ChanDftCfg->combFilterType;		/*CIC Filter (Auxiliary) Configuration*/

    cfgAux.B.SRGA              = ChanDftCfg->serviceRequest;		/*Service Request Generation Auxiliary Filter*/
    cfgAux.B.ESEL              = ChanDftCfg->eventSelect;			/*Event Select*/
    cfgAux.B.EGT               = ChanDftCfg->eventGate;				/*Event Gating*/

    cfgAux.B.AFSC              = ChanDftCfg->combFilterShift;		/*Auxiliary Filter Shift Control*/

    (channel->channel)->FCFGA = cfgAux;
}

