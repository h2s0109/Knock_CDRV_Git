/*
 * knocheck_Dsadc.c
 *
 *  Created on: 2015. 2. 23.
 *      Author: hansteve
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "knocheck_priv.h"
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
static uint8 DsadcChannelAvailable[4] = {0, 1, 0, 0};

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void KnoCheck_DSADC_Ini(void)
{
	uint32 chn;
	Dsadc_ChannelConfig KnoCheck_DSADC_ChanCfg_temp;
	Dsadc_ChannelSetting DSADC_channel_temp;

	/*Initialization GLOBCFG Register*/
	KnoCheck_Dsadc_IniGlob();
	/*Assign DSADC default value*/
	KnoCheck_Dsadc_IniChanDftCfg(&KnoCheck_DSADC_ChanCfg_temp);

	/* initialize channels */
    for (chn = 0; chn < 2; ++chn)
    {
        if (DsadcChannelAvailable[chn])
        {
        	KnoCheck_DSADC_ChanCfg_temp.channelId = (ChannelId)chn;
            KnoCheck_Dsadc_IniChan(&DSADC_channel_temp.dsadcChannels[chn], &KnoCheck_DSADC_ChanCfg_temp);
#if 0
            KnoCheck_Dsadc_ChanEnDis(chn, KNK_EN_ENABLE);
#endif
        }
    }

    /*Start DSADC*/

    Ifx_DSADC *psDSADC = &MODULE_DSADC;
    psDSADC->GLOBRC.B.M1RUN =1;
    psDSADC->GLOBRC.B.CH1RUN =1;
//  psDSADC->GLOBRC.U = psDSADC->GLOBRC.U | psDSADC->GLOBRC.B.M0RUN | psDSADC->GLOBRC.B.CH0RUN;
    SRC_DSADCSRM1.U = (3 << 11) | (1 << 10) | 1; /* 0<11 => ON CPU0, 1<<10 => Enable*, 55 INT_PRIO*/
};

static void KnoCheck_Dsadc_IniGlob(void)
{
	Ifx_DSADC *psDSADC = &MODULE_DSADC;
	Ifx_DSADC_GLOBCFG KnCheck_DSADC_GlobCfg;

	/*Clock setting, Can move by user*/
	uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(passwd);
	psDSADC->CLC.U = 0x00000000;
	if (psDSADC->CLC.U)
	{}                       // sync access
	IfxScuWdt_setCpuEndinit(passwd);
	/*DSADC setting*/
	KnCheck_DSADC_GlobCfg.B.MCSEL     = ModulatorClock_fDSD;
	KnCheck_DSADC_GlobCfg.B.LOSUP     = LowPowerSupply_5V;/*normal power mode*/
	KnCheck_DSADC_GlobCfg.B.PSWC  		= Can_Write;

	psDSADC->GLOBCFG.U = KnCheck_DSADC_GlobCfg.U;
}

static void KnoCheck_Dsadc_IniChanDftCfg(Dsadc_ChannelConfig *KnoCheck_DSADC_ChanDftCfg)
{
	/*Assign_DSADC_modulator_value*/
	KnoCheck_DSADC_ChanDftCfg->modulator.positiveInput      	= InputConfig_inputPin;				/*DSADC_CH_MODCFG.B.INCFGP*/
	KnoCheck_DSADC_ChanDftCfg->modulator.negativeInput      	= InputConfig_referenceGround;		/*DSADC_CH_MODCFG.B.INCFGN*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputGain         		= InputGain_factor1;				/*DSADC_CH_MODCFG.B.GAINSEL*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputPin           	= InputPin_b;						/*DSADC_CH_MODCFG.B.INSEL*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputMode           	= Trig_rising;						/*DSADC_CH_MODCFG.B.INMODE*/
	KnoCheck_DSADC_ChanDftCfg->modulator.inputMAC				= Preset;							/*DSADC_CH_MODCFG.B.INMAC*/
	KnoCheck_DSADC_ChanDftCfg->modulator.modulatorClockFreq 	= 12.5e6;							/*DSADC_CH_MODCFG.B.DIVM*/
	KnoCheck_DSADC_ChanDftCfg->modulator.commonModeVoltage  	= CommonModeVoltage_c;				/*DSADC_CH_MODCFG.B.CMVS*/
	KnoCheck_DSADC_ChanDftCfg->modulator.modulatorcfg  			= High_NoAntialias;					/*DSADC_CH_MODCFG.B.MCFG*/
	KnoCheck_DSADC_ChanDftCfg->modulator.gaincal  				= No_gain_cal;						/*DSADC_CH_MODCFG.B.GCEN*/
	KnoCheck_DSADC_ChanDftCfg->modulator.autopower				= Auto_on;							/*DSADC_CH_MODCFG.B.APC*/

	/*Assign_DSADC_Demodulator_value*/
	KnoCheck_DSADC_ChanDftCfg->demodulator.inputDataSource	= InputDataSource_onChipStandAlone;		/*DSADC_CH_DICFG.B.DSRC*/
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
	KnoCheck_DSADC_ChanDftCfg->combFilter.decimationFactor	= 8;									/*DSADC_CH_FCFGC.B.CFMDF*/
	KnoCheck_DSADC_ChanDftCfg->combFilter.startValue      	= 8;									/*DSADC_CH_FCFGC.B.CFMSV*/

	/*Assign_DSADC_FIRfilter_value*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.fir0Enabled       	= TRUE;									/*DSADC_CH_FCFGM.B.FIR0EN*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.fir1Enabled       	= TRUE;									/*DSADC_CH_FCFGM.B.FIR1EN*/
	KnoCheck_DSADC_ChanDftCfg->firFilter.offsetCompensation = FALSE;								/*DSADC_CH_FCFGM.B.OCEN*/
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
	//KnoCheck_DSADC_ChanDftCfg->channelPins            		= NULL_PTR;
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
#if 0
void KnoCheck_Dsadc_ChanEnDis(uint8 Ch, uint8 Enable)
{
	Ifx_DSADC *psDSADC = &MODULE_DSADC;
    switch(ch)
    case KNK_CH1:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M0RUN =1;
			psDSADC->GLOBRC.B.CH0RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M0RUN =0;
			psDSADC->GLOBRC.B.CH0RUN =0;
    	}
    	break;
    case KNK_CH2:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M1RUN =1;
			psDSADC->GLOBRC.B.CH1RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M1RUN =0;
			psDSADC->GLOBRC.B.CH1RUN =0;
    	}
    	break;
    case KNK_CH3:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M2RUN =1;
			psDSADC->GLOBRC.B.CH2RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M2RUN =0;
			psDSADC->GLOBRC.B.CH2RUN =0;
    	}
    	break;
    case KNK_CH4:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M3RUN =1;
			psDSADC->GLOBRC.B.CH3RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M3RUN =0;
			psDSADC->GLOBRC.B.CH3RUN =0;
    	}
    	break;
/*DO NOT FORGET Hardware number need to be changed.*/
/*    case 4:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M4RUN =1;
			psDSADC->GLOBRC.B.CH4RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M4RUN =0;
			psDSADC->GLOBRC.B.CH4RUN =0;
    	}
    case 5:
    	if(Enable == KNK_EN_ENABLE)
    	{
			psDSADC->GLOBRC.B.M5RUN =1;
			psDSADC->GLOBRC.B.CH5RUN =1;
    	}
    	else
    	{
			psDSADC->GLOBRC.B.M5RUN =0;
			psDSADC->GLOBRC.B.CH5RUN =0;
    	}*/
    default:
    	psDSADC->GLOBRC =0;
    	break;
}



void Knk_En(uint8 Ch, uint8 Enable)
{
	KnoCheck_Dsadc_ChanEnDis(Ch,Enable);
}
#endif

static void KnoCheck_Dsadc_IniModulator(Dsadc_Channel *channel, const Dsadc_ModulatorConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_MODCFG ModCfg_temp;
    float32 targetFreq;
    float32 sourceFreq;

    /*collect_DSADC_Modulator_value*/

    ModCfg_temp.U			= 0; 							/*Assign initial zero value*/

    /*--------------Input Functionality--------------*/
    ModCfg_temp.B.INCFGP	= ChanDftCfg->positiveInput;	/*Configuration of Positive Input Line*/
    ModCfg_temp.B.INCFGN	= ChanDftCfg->negativeInput;	/*Configuration of Negative Input Line*/
    ModCfg_temp.B.GAINSEL	= ChanDftCfg->inputGain;		/*Gain Select of Analog Input Path*/
    ModCfg_temp.B.INSEL		= ChanDftCfg->inputPin;			/*Input Pin Selection*/
    ModCfg_temp.B.INMODE	= ChanDftCfg->inputMode;		/*Input Multiplexer Control Mode*/
    ModCfg_temp.B.INMAC		= ChanDftCfg->inputMAC;			/*Input Multiplexer Action Control*/
    ModCfg_temp.B.INCWC		= Can_Write;       				/*NCFGP, INCFGN, GAINSEL, INSEL,INMODE, INMAC can be written*/

    /*--------------Clock Functionality--------------*/
    targetFreq				= ChanDftCfg->modulatorClockFreq;/*Tagert frequency*/
    sourceFreq				= KnoCheck_Dsadc_getSpbFrequency();
    ModCfg_temp.B.DIVM		= KnoCheck_Dsadc_calcDIVx(sourceFreq, &targetFreq);	/*Divider Factor for Modulator Clock*/
    ModCfg_temp.B.DWC		= Can_Write; 					/*Bitfield DIVM can be written*/

    /*--------------ETC Functionality--------------*/
    ModCfg_temp.B.CMVS		= ChanDftCfg->commonModeVoltage;/* Common Mode Voltage Selection*/
    ModCfg_temp.B.MCFG		= ChanDftCfg->modulatorcfg; 	/*Modulator Configuration*/
    ModCfg_temp.B.GCEN		= ChanDftCfg->gaincal;			/*Gain Calibration Enable*/
    ModCfg_temp.B.APC		= ChanDftCfg->autopower;			/*Gain Calibration Enable*/
    ModCfg_temp.B.MWC		= Can_Write; 					/*Bitfields CMVS, MCFG, GCEN, APC can be written*/

    (channel->channel)->MODCFG = ModCfg_temp;				/* Register setting*/
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
    Ifx_DSADC_CH_DICFG DeModCfg_temp;

    /*collect_DSADC_Demodulator_value*/

    DeModCfg_temp.U                   = 0;	/*Assign initial zero value*/

    /*--------------Input Functionality--------------*/
    DeModCfg_temp.B.DSRC              = ChanDftCfg->inputDataSource;	/*Input Data Source Select*/
    DeModCfg_temp.B.DSWC              = Can_Write; 						/*Bitfield DSRC can be written*/

    /*--------------Trigger Functionality--------------*/
    DeModCfg_temp.B.ITRMODE           = ChanDftCfg->integrationTrigger;	/*Integrator Trigger Mode*/
    DeModCfg_temp.B.TSTRMODE          = ChanDftCfg->timestampTrigger;	/*Timestamp Trigger Mode*/
    DeModCfg_temp.B.TRSEL             = ChanDftCfg->triggerInput;		/*Trigger Select*/
    DeModCfg_temp.B.TRWC              = Can_Write; 						/*Bitfields TRSEL, TSTRMODE, ITRMODE can be written*/

    /*--------------Sampling Functionality--------------*/
    DeModCfg_temp.B.CSRC              = ChanDftCfg->sampleClockSource;	/*Sample Clock Source Select*/
    DeModCfg_temp.B.STROBE            = ChanDftCfg->sampleStrobe;		/*Data Strobe Generation Mode*/
    DeModCfg_temp.B.SCWC              = Can_Write; 						/*Bitfields STROBE, CSRC can be written*/

    (channel->channel)->DICFG = DeModCfg_temp;							/* Register setting*/
}
static void KnoCheck_Dsadc_IniCombFilter(Dsadc_Channel *channel, const Dsadc_CombFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGC Comb_temp;

    /*collect_DSADC_Combfilter_value*/

    Comb_temp.U                   = 0;	/*Assign initial zero value*/

    Comb_temp.B.CFMDF             = ChanDftCfg->decimationFactor - 1;
    Comb_temp.B.CFMC              = ChanDftCfg->combFilterType;
    Comb_temp.B.CFEN              = (ChanDftCfg->bypassed == FALSE) ? 1 : 0;

    Comb_temp.B.MFSC              = ChanDftCfg->combFilterShift;

    Comb_temp.B.SRGM              = ChanDftCfg->serviceRequest;
    Comb_temp.B.CFMSV             = ChanDftCfg->startValue - 1;

    (channel->channel)->FCFGC = Comb_temp;
}

static void KnoCheck_Dsadc_IniFirFilter(Dsadc_Channel *channel, const Dsadc_FirFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGM Fir_temp;

    /*collect_DSADC_FIRfilter_value*/

    Fir_temp.U                   = 0;	/*Assign initial zero value*/

    Fir_temp.B.FIR0EN            = ChanDftCfg->fir0Enabled;
    Fir_temp.B.FIR1EN            = ChanDftCfg->fir1Enabled;
    Fir_temp.B.OCEN              = ChanDftCfg->offsetCompensation;
    Fir_temp.B.DSH               = ChanDftCfg->dataShift;
    Fir_temp.B.FSH               = ChanDftCfg->internalShift;

    (channel->channel)->FCFGM = Fir_temp;
}
static void KnoCheck_Dsadc_IniIntegrator(Dsadc_Channel *channel, const Dsadc_IntegratorConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_IWCTR Integ_temp;

    /*collect_DSADC_FIRfilter_value*/

    Integ_temp.U                   = 0;									/*Assign initial zero value*/

    Integ_temp.B.REPVAL            = ChanDftCfg->integrationCycles - 1;	/*Number of Integration Cycles*/
    Integ_temp.B.NVALDIS           = ChanDftCfg->discardCount;			/*Number of Values Discarded*/
    Integ_temp.B.IWS               = ChanDftCfg->windowSize;			/*Integration Window Size*/
    Integ_temp.B.NVALINT           = ChanDftCfg->integrationCount - 1;	/*Number of Values Integrated*/

    (channel->channel)->IWCTR = Integ_temp;
}
static void KnoCheck_Dsadc_IniAuxFilter(Dsadc_Channel *channel, const Dsadc_AuxFilterConfig *ChanDftCfg)
{
    Ifx_DSADC_CH_FCFGA Aux_temp;

    /*collect_DSADC_Auxfilter_value*/

    Aux_temp.U                   = 0;								/*Assign initial zero value*/

    Aux_temp.B.CFADF             = ChanDftCfg->decimationFactor - 1;	/*CIC Filter (Auxiliary) Decimation Factor*/
    Aux_temp.B.CFAC              = ChanDftCfg->combFilterType;			/*CIC Filter (Auxiliary) Configuration*/

    Aux_temp.B.SRGA              = ChanDftCfg->serviceRequest;			/*Service Request Generation Auxiliary Filter*/
    Aux_temp.B.ESEL              = ChanDftCfg->eventSelect;				/*Event Select*/
    Aux_temp.B.EGT               = ChanDftCfg->eventGate;				/*Event Gating*/

    Aux_temp.B.AFSC              = ChanDftCfg->combFilterShift;			/*Auxiliary Filter Shift Control*/

    (channel->channel)->FCFGA = Aux_temp;
}

#ifdef KNOCHECK_DSADC_TEST
sint16 result_DSADC[1][10000];
uint16 count_i;

void KnoCheck_DSADC_Demo(void)
{
	Ifx_DSADC *psDSADC = &MODULE_DSADC;
	if(count_i<1000)
	{
		//psDSADC->CH[0].OFFM.B.OFFSET=1200 ;
	result_DSADC[0][count_i]=(sint16)(psDSADC->CH[0].RESM.B.RESULT);
	count_i++;
	}

}
#endif

