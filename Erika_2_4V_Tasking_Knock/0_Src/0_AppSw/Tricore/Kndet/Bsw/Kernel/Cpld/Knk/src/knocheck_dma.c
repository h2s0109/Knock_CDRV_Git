/*
 * knocheck_Dma.c
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
void KnoCheck_Dma_Ini(void);
static void KnoCheck_Dma_IniChanDftCfg(Dma_ChannelConfig *KC_Dma_ChanDftCfg);
static void KnoCheck_Dma_IniChan(Dma_Channel *channel, Dma_ChannelConfig *conf);
static void Knocheck_Dma_ConfTranSet(Ifx_DMA_CH *channel, Dma_ChannelConfig *conf);
Cpu_ResourceCpu Cpu_getCoreId(void);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void KnoCheck_Dma_Ini(void)
{
    /*uint32 cntChan;*/
#ifdef KNOCHECK_TEST
    uint32 cntBuffer;
#endif
    /*End of KNOCHECK_TEST*/
    Dma_ChannelConfig cfgKC_DmaChan;
    Dma_ChannelSetting iniKC_DmaChan;

    /* initial configuration for all channels */
    KnoCheck_Dma_IniChanDftCfg(&cfgKC_DmaChan);

    /* channel specific configurations */
/*    for (cntChan = 0; cntChan < MAX_CHANNEL; cntChan++)
    {*/
    	cfgKC_DmaChan.channelId          	= (Dma_ChannelId)DMA_CHANNEL_SELECT;
#ifdef KNOCHECK_TEST
    	//cfgKC_DmaChan.sourceAddress 	   	= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &TestCntsource[0]);
    	cfgKC_DmaChan.sourceAddress 	   	= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &DSADC_RESM);
#else
    	cfgKC_DmaChan.sourceAddress 	   	= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &DSADC_RESM);
#endif
	/*End of KNOCHECK_TEST*/
    	cfgKC_DmaChan.destinationAddress	= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer0_Dest[0]);
		cfgKC_DmaChan.shadowAddress 		= CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &Buffer1_Dest[0]);
		cfgKC_DmaChan.shadowControl 		= Dma_ChannelShadow_doubleDestinationBufferingHwSwSwitch;

    	cfgKC_DmaChan.transferCount 					= 10;											/*DMA_CH_CHCFGR.B.TREL*/

    	cfgKC_DmaChan.requestMode 						= Dma_ChannelRequestMode_oneTransferPerRequest;	/*DMA_CH_CHCFGR.B.RROAT*/
    	cfgKC_DmaChan.operationMode 					= Dma_ChannelOperationMode_continuous;			/*DMA_CH_CHCFGR.B.CHMODE*/
    	cfgKC_DmaChan.sourceAddressCircularRange		= Dma_ChannelIncrementCircular_none;			/*DMA_CH_ADICR.B.CBLS*/
    	cfgKC_DmaChan.destinationAddressCircularRange 	= Dma_ChannelIncrementCircular_2048;			/*DMA_CH_ADICR.B.CBLD*/

    	/*Interrupt will be generated when interruptRaiseThreshold match occurred*/
    	cfgKC_DmaChan.channelInterruptEnabled 			= TRUE;
    	cfgKC_DmaChan.channelInterruptControl 			= Dma_ChannelInterruptControl_thresholdLimitMatch;
    	cfgKC_DmaChan.interruptRaiseThreshold 			= 0;

    	SRC_DMA_KNOCK.U =(1 << 11) | (1 << 10) | KNOCHECK_DMA_PRIO;

        KnoCheck_Dma_IniChan(&iniKC_DmaChan.chn[DMA_CHANNEL_SELECT], &cfgKC_DmaChan);
/*    }*/

#ifdef KNOCHECK_TEST
    /* prepare buffers */
	uint32 cntChan;
    for(cntChan =0 ; cntChan < MAX_CHANNEL ; cntChan++)
    {
    	TestCntsource[cntChan] = 1;
		for (cntBuffer = 0; cntBuffer < BUFFER_SIZE; cntBuffer++)
		{
			Buffer0_Dest[cntBuffer] = 0xbeef;
			Buffer1_Dest[cntBuffer] = 0xbeef;
		}
    }
#endif
    /*End of KNOCHECK_TEST*/
}
/*End of void KnoCheck_Dma_Ini(void)*/

static void KnoCheck_Dma_IniChanDftCfg(Dma_ChannelConfig *KC_Dma_ChanDftCfg)
{
	  /* following configuration is used by all channels */
	KC_Dma_ChanDftCfg->channelId                              = Dma_ChannelId_0;

	KC_Dma_ChanDftCfg->sourceAddress                          = 0;														/*DMA_CH_SADR.U*/
	KC_Dma_ChanDftCfg->destinationAddress                     = 0;														/*DMA_CH_DADR.U*/
	KC_Dma_ChanDftCfg->shadowAddress                          = 0;														/*DMA_CH_SHADR.U*/
	KC_Dma_ChanDftCfg->readDataCrc                            = 0;														/*DMA_CH_RDCRCR.U*/
	KC_Dma_ChanDftCfg->sourceDestinationAddressCrc            = 0;														/*DMA_CH_SDCRCR.U*/

	KC_Dma_ChanDftCfg->transferCount                          = 10;														/*DMA_CH_CHCFGR.B.TREL*/
	KC_Dma_ChanDftCfg->blockMode                              = Dma_ChannelMove_1;										/*DMA_CH_CHCFGR.B.BLKM*/
	KC_Dma_ChanDftCfg->requestMode                            = Dma_ChannelRequestMode_completeTransactionPerRequest;	/*DMA_CH_CHCFGR.B.RROAT*/
	KC_Dma_ChanDftCfg->operationMode                          = Dma_ChannelOperationMode_continuous;					/*DMA_CH_CHCFGR.B.CHMODE*/
	KC_Dma_ChanDftCfg->moveSize                               = Dma_ChannelMoveSize_16bit;								/*DMA_CH_CHCFGR.B.CHDW*/
	KC_Dma_ChanDftCfg->pattern                                = Dma_ChannelPattern_0_disable;							/*DMA_CH_CHCFGR.B.PATSEL*/
	KC_Dma_ChanDftCfg->requestSource                          = Dma_ChannelRequestSource_peripheral;					/*DMA_CH_CHCFGR.B.PRSEL*/
	KC_Dma_ChanDftCfg->busPriority                            = Dma_ChannelBusPriority_medium;							/*DMA_CH_CHCFGR.B.DMAPRIO*/

	KC_Dma_ChanDftCfg->hardwareRequestEnabled                 = TRUE;													/*DMA_TSR.B.ECH*/

	KC_Dma_ChanDftCfg->sourceAddressIncrementStep             = Dma_ChannelIncrementCircular_none;						/*DMA_CH_ADICR.B.SMF*/
	KC_Dma_ChanDftCfg->sourceAddressIncrementDirection        = Dma_ChannelIncrementDirection_positive;					/*DMA_CH_ADICR.B.INCS*/
	KC_Dma_ChanDftCfg->sourceAddressCircularRange             = Dma_ChannelIncrementCircular_none;						/*DMA_CH_ADICR.B.CBLS*/
	KC_Dma_ChanDftCfg->destinationAddressIncrementStep        = Dma_ChannelIncrementStep_1;								/*DMA_CH_ADICR.B.DMF*/
	KC_Dma_ChanDftCfg->destinationAddressIncrementDirection   = Dma_ChannelIncrementDirection_positive; 				/*DMA_CH_ADICR.B.INCD*/
	KC_Dma_ChanDftCfg->destinationAddressCircularRange        = Dma_ChannelIncrementCircular_4096;						/*DMA_CH_ADICR.B.CBLD*/
	KC_Dma_ChanDftCfg->shadowControl                          = Dma_ChannelShadow_none;									/*DMA_CH_ADICR.B.SHCT*/
	KC_Dma_ChanDftCfg->sourceCircularBufferEnabled            = TRUE;													/*DMA_CH_ADICR.B.SCBE*/
	KC_Dma_ChanDftCfg->destinationCircularBufferEnabled       = TRUE;													/*DMA_CH_ADICR.B.DCBE*/
	KC_Dma_ChanDftCfg->timestampEnabled                       = FALSE;													/*DMA_CH_ADICR.B.STAMP*/
	KC_Dma_ChanDftCfg->wrapSourceInterruptEnabled             = FALSE;													/*DMA_CH_ADICR.B.WRPSE*/
	KC_Dma_ChanDftCfg->wrapDestinationInterruptEnabled        = FALSE;													/*DMA_CH_ADICR.B.WRPDE*/
	KC_Dma_ChanDftCfg->channelInterruptEnabled                = FALSE;													/*DMA_CH_ADICR.B.INTCT*/
	KC_Dma_ChanDftCfg->channelInterruptControl                = Dma_ChannelInterruptControl_thresholdLimitMatch;		/*DMA_CH_ADICR.B.INTCT*/
	KC_Dma_ChanDftCfg->interruptRaiseThreshold                = 0;														/*DMA_CH_ADICR.B.IRDV*/
	/*To use Double buffing shuould be set ETRL*/
	KC_Dma_ChanDftCfg->transactionRequestLostInterruptEnabled = TRUE;													/*DMA_CH_ADICR.B.ETRL*/
}
/*End of static void KnoCheck_Dma_IniChanDftCfg(Dma_ChannelConfig *KC_Dma_ChanDftCfg)*/

static void KnoCheck_Dma_IniChan(Dma_Channel *channel, Dma_ChannelConfig *config)
{
    Ifx_DMA *psDMA = &MODULE_DMA;
    Ifx_DMA_TSR tsr_temp;
    channel->dma       = psDMA;
    channel->channelId = config->channelId;
    channel->channel   = &psDMA->CH[config->channelId];

    Knocheck_Dma_ConfTranSet(channel->channel, config);

	tsr_temp.U = 0;
	if (config->hardwareRequestEnabled)
	{
		tsr_temp.B.ECH = 1;
	}
	else
	{
		tsr_temp.B.DCH = 1;
	}

	psDMA->TSR[channel->channelId].U = tsr_temp.U;
}
/*End of static void KnoCheck_Dma_IniChan(Dma_Channel *channel, Dma_ChannelConfig *config)*/

static void Knocheck_Dma_ConfTranSet(Ifx_DMA_CH *channel, Dma_ChannelConfig *config)
{
	  /*collect DMA_CH_CHCFGR value*/
        Ifx_DMA_CH_CHCFGR chcfgr_temp;
        chcfgr_temp.U          = 0;
        chcfgr_temp.B.TREL     = config->transferCount;
        chcfgr_temp.B.BLKM     = config->blockMode;
        chcfgr_temp.B.RROAT    = config->requestMode;
        chcfgr_temp.B.CHMODE   = config->operationMode;
        chcfgr_temp.B.CHDW     = config->moveSize;
        chcfgr_temp.B.PRSEL    = config->requestSource;
        chcfgr_temp.B.PATSEL   = config->pattern;

    /*collect Ifx_DMA_CH_ADICR value*/
        Ifx_DMA_CH_ADICR adicr_temp;
        adicr_temp.U       	= 0;
        adicr_temp.B.SMF	= config->sourceAddressIncrementStep;
        adicr_temp.B.INCS	= config->sourceAddressIncrementDirection;
        adicr_temp.B.CBLS	= config->sourceAddressCircularRange;
        adicr_temp.B.SCBE	= config->sourceCircularBufferEnabled;
        adicr_temp.B.DMF	= config->destinationAddressIncrementStep;
        adicr_temp.B.INCD	= config->destinationAddressIncrementDirection;
        adicr_temp.B.CBLD	= config->destinationAddressCircularRange;
        adicr_temp.B.DCBE	= config->destinationCircularBufferEnabled;
        adicr_temp.B.SHCT	= config->shadowControl;
        adicr_temp.B.STAMP	= config->timestampEnabled;
        adicr_temp.B.WRPSE	= config->wrapSourceInterruptEnabled;
        adicr_temp.B.WRPDE	= config->wrapDestinationInterruptEnabled;
        adicr_temp.B.INTCT	= (config->channelInterruptEnabled ? 2 : 0) | (config->channelInterruptControl ? 1 : 0);
        adicr_temp.B.IRDV	= config->interruptRaiseThreshold;
        adicr_temp.B.ETRL	= config->transactionRequestLostInterruptEnabled;

        channel->CHCFGR.U	= chcfgr_temp.U;							/* Register setting*/
		channel->ADICR.U	= adicr_temp.U;								/* Register setting*/
		channel->SADR.U		= config->sourceAddress;					/* Register setting*/
		channel->DADR.U		= config->destinationAddress;				/* Register setting*/
		channel->SHADR.U	= config->shadowAddress;					/* Register setting*/
		channel->SDCRCR.U	= config->sourceDestinationAddressCrc;		/* Register setting*/
		channel->RDCRCR.U	= config->readDataCrc;						/* Register setting*/
}
/*End of static void Knocheck_Dma_ConfTranSet(Ifx_DMA_CH *channel, Dma_ChannelConfig *config)*/

Cpu_ResourceCpu Cpu_getCoreId(void)
{
    Ifx_CPU_CORE_ID reg;
    reg.U = __mfcr(CPU_CORE_ID);
    return (Cpu_ResourceCpu)reg.B.CORE_ID;
}
/*End of Cpu_ResourceCpu Cpu_getCoreId(void)*/
