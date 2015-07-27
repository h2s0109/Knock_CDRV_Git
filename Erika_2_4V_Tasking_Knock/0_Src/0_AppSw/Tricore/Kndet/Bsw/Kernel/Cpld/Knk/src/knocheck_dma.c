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
#ifdef KNOCHECK_TEST
sint16 Default_Value = 1;
__align (4)
volatile sint16 source[NUM_CHANNELS];
__align (8)
volatile sint16 destination[BUFFER_SIZE];
	#ifdef KNOCHECK_DOUBLE_BUFFERING
	__align (8)
	volatile sint16 destination_2[BUFFER_SIZE];
	#endif /*End of KNOCHECK_DOUBLE_BUFFERING*/
#endif /*End of KNOCHECK_TEST*/




/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void KnoCheck_Dma_IniChanDftCfg(Dma_ChannelConfig *KnoCheck_Dma_ChanDftCfg);
static void KnoCheck_Dma_IniChannels(Dma_Channel *channel, Dma_ChannelConfig *config);
static void Knocheck_Dma_configureTransactionSets(Ifx_DMA_CH *channel, Dma_ChannelConfig *config);
Cpu_ResourceCpu Cpu_getCoreId(void);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void KnoCheck_Dma_Ini(void)
{
    uint32                   i, j;
    Dma_ChannelConfig KnoCheck_DMA_ChanCfg_temp;
    Dma_ChannelSetting DMA_Channel_temp;
    /* initial configuration for all channels */

    KnoCheck_Dma_IniChanDftCfg(&KnoCheck_DMA_ChanCfg_temp);

    /* channel specific configurations */
    for (i = 1; i < 2; ++i)
    {
    	KnoCheck_DMA_ChanCfg_temp.channelId          = (Dma_ChannelId)i;
#ifdef KNOCHECK_TEST
    	KnoCheck_DMA_ChanCfg_temp.sourceAddress 	   = CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &source[0]);
    	KnoCheck_DMA_ChanCfg_temp.destinationAddress = CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination[0]);
	#ifdef KNOCHECK_DOUBLE_BUFFERING
			KnoCheck_DMA_ChanCfg_temp.shadowAddress = CPU_GLB_ADDR_DSPR(Cpu_getCoreId(), &destination_2[0]);
			KnoCheck_DMA_ChanCfg_temp.shadowControl = Dma_ChannelShadow_doubleDestinationBufferingHwSwSwitch;
	#endif /*End of KNOCHECK_DOUBLE_BUFFERING*/
#endif /*End of KNOCHECK_TEST*/

#ifdef DOUBLE_BUFFRING_TEST
    	KnoCheck_DMA_ChanCfg_temp.transferCount = TRANSFER_COUNT;										/*DMA_CH_CHCFGR.B.TREL*/
#else
#endif /*End of DOUBLE_BUFFRING_TEST*/
    	KnoCheck_DMA_ChanCfg_temp.transferCount = BUFFER_SIZE;											/*DMA_CH_CHCFGR.B.TREL*/

    	KnoCheck_DMA_ChanCfg_temp.requestMode = Dma_ChannelRequestMode_oneTransferPerRequest;			/*DMA_CH_CHCFGR.B.RROAT*/
    	KnoCheck_DMA_ChanCfg_temp.operationMode = Dma_ChannelOperationMode_continuous;					/*DMA_CH_CHCFGR.B.CHMODE*/
    	KnoCheck_DMA_ChanCfg_temp.sourceAddressCircularRange = Dma_ChannelIncrementCircular_none;		/*DMA_CH_ADICR.B.CBLS*/
    	KnoCheck_DMA_ChanCfg_temp.destinationAddressCircularRange = Dma_ChannelIncrementCircular_4096;	/*DMA_CH_ADICR.B.CBLD*/

        /*Interrupt will be generated when interruptRaiseThreshold match occurred*/
    	KnoCheck_DMA_ChanCfg_temp.channelInterruptEnabled = TRUE;
    	KnoCheck_DMA_ChanCfg_temp.channelInterruptControl = Dma_ChannelInterruptControl_thresholdLimitMatch;
    	KnoCheck_DMA_ChanCfg_temp.interruptRaiseThreshold = 0;
        SRC_DMA_DMA0_CH1.U =(1 << 11) | (1 << 10) | 57;

        KnoCheck_Dma_IniChannels(&DMA_Channel_temp.chn[i], &KnoCheck_DMA_ChanCfg_temp);
    }

#ifdef KNOCHECK_TEST
    /* prepare buffers */
    for(i =0;i <NUM_CHANNELS;i++)
    {
    	source[i]=Default_Value;
		for (j = 0; j < BUFFER_SIZE; j++)
		{
			destination[j]=0xbeef;
#ifdef KNOCHECK_DOUBLE_BUFFERING
			destination_2[j]=0xbeef;
#endif
		}
    }
#endif

}


static void KnoCheck_Dma_IniChanDftCfg(Dma_ChannelConfig *KnoCheck_Dma_ChanDftCfg)
{
	  /* following configuration is used by all channels */

	KnoCheck_Dma_ChanDftCfg->channelId                              = Dma_ChannelId_0;

	KnoCheck_Dma_ChanDftCfg->sourceAddress                          = 0;												/*DMA_CH_SADR.U*/
	KnoCheck_Dma_ChanDftCfg->destinationAddress                     = 0;												/*DMA_CH_DADR.U*/
	KnoCheck_Dma_ChanDftCfg->shadowAddress                          = 0;												/*DMA_CH_SHADR.U*/
	KnoCheck_Dma_ChanDftCfg->readDataCrc                            = 0;												/*DMA_CH_RDCRCR.U*/
	KnoCheck_Dma_ChanDftCfg->sourceDestinationAddressCrc            = 0;												/*DMA_CH_SDCRCR.U*/

	KnoCheck_Dma_ChanDftCfg->transferCount                          = 10;												/*DMA_CH_CHCFGR.B.TREL*/
	KnoCheck_Dma_ChanDftCfg->blockMode                              = Dma_ChannelMove_1;								/*DMA_CH_CHCFGR.B.BLKM*/
	KnoCheck_Dma_ChanDftCfg->requestMode                            = Dma_ChannelRequestMode_completeTransactionPerRequest;	/*DMA_CH_CHCFGR.B.RROAT*/
	KnoCheck_Dma_ChanDftCfg->operationMode                          = Dma_ChannelOperationMode_continuous;				/*DMA_CH_CHCFGR.B.CHMODE*/
	KnoCheck_Dma_ChanDftCfg->moveSize                               = Dma_ChannelMoveSize_16bit;						/*DMA_CH_CHCFGR.B.CHDW*/
	KnoCheck_Dma_ChanDftCfg->pattern                                = Dma_ChannelPattern_0_disable;						/*DMA_CH_CHCFGR.B.PATSEL*/
	KnoCheck_Dma_ChanDftCfg->requestSource                          = Dma_ChannelRequestSource_peripheral;				/*DMA_CH_CHCFGR.B.PRSEL*/
	KnoCheck_Dma_ChanDftCfg->busPriority                            = Dma_ChannelBusPriority_medium;					/*DMA_CH_CHCFGR.B.DMAPRIO*/

	KnoCheck_Dma_ChanDftCfg->hardwareRequestEnabled                 = TRUE;												/*DMA_TSR.B.ECH*/

	KnoCheck_Dma_ChanDftCfg->sourceAddressIncrementStep             = Dma_ChannelIncrementCircular_none;				/*DMA_CH_ADICR.B.SMF*/
	KnoCheck_Dma_ChanDftCfg->sourceAddressIncrementDirection        = Dma_ChannelIncrementDirection_positive;			/*DMA_CH_ADICR.B.INCS*/
	KnoCheck_Dma_ChanDftCfg->sourceAddressCircularRange             = Dma_ChannelIncrementCircular_none;				/*DMA_CH_ADICR.B.CBLS*/
	KnoCheck_Dma_ChanDftCfg->destinationAddressIncrementStep        = Dma_ChannelIncrementStep_1;						/*DMA_CH_ADICR.B.DMF*/
	KnoCheck_Dma_ChanDftCfg->destinationAddressIncrementDirection   = Dma_ChannelIncrementDirection_positive; 			/*DMA_CH_ADICR.B.INCD*/
	KnoCheck_Dma_ChanDftCfg->destinationAddressCircularRange        = Dma_ChannelIncrementCircular_4096;				/*DMA_CH_ADICR.B.CBLD*/
	KnoCheck_Dma_ChanDftCfg->shadowControl                          = Dma_ChannelShadow_none;							/*DMA_CH_ADICR.B.SHCT*/
	KnoCheck_Dma_ChanDftCfg->sourceCircularBufferEnabled            = TRUE;												/*DMA_CH_ADICR.B.SCBE*/
	KnoCheck_Dma_ChanDftCfg->destinationCircularBufferEnabled       = TRUE;												/*DMA_CH_ADICR.B.DCBE*/
	KnoCheck_Dma_ChanDftCfg->timestampEnabled                       = FALSE;											/*DMA_CH_ADICR.B.STAMP*/
	KnoCheck_Dma_ChanDftCfg->wrapSourceInterruptEnabled             = FALSE;											/*DMA_CH_ADICR.B.WRPSE*/
	KnoCheck_Dma_ChanDftCfg->wrapDestinationInterruptEnabled        = FALSE;											/*DMA_CH_ADICR.B.WRPDE*/
	KnoCheck_Dma_ChanDftCfg->channelInterruptEnabled                = FALSE;											/*DMA_CH_ADICR.B.INTCT*/
	KnoCheck_Dma_ChanDftCfg->channelInterruptControl                = Dma_ChannelInterruptControl_thresholdLimitMatch;	/*DMA_CH_ADICR.B.INTCT*/
	KnoCheck_Dma_ChanDftCfg->interruptRaiseThreshold                = 0;												/*DMA_CH_ADICR.B.IRDV*/
	KnoCheck_Dma_ChanDftCfg->transactionRequestLostInterruptEnabled = TRUE;												/*DMA_CH_ADICR.B.ETRL*/
	KnoCheck_Dma_ChanDftCfg->channelInterruptPriority               = 0;
	KnoCheck_Dma_ChanDftCfg->channelInterruptTypeOfService          = IfxSrc_Tos_cpu0;

}


static void KnoCheck_Dma_IniChannels(Dma_Channel *channel, Dma_ChannelConfig *config)
{
    Ifx_DMA *psDMA = &MODULE_DMA;
    Ifx_DMA_TSR tsr_temp;
    channel->dma       = psDMA;
    channel->channelId = config->channelId;
    channel->channel   = &psDMA->CH[config->channelId];

    Knocheck_Dma_configureTransactionSets(channel->channel, config);

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



static void Knocheck_Dma_configureTransactionSets(Ifx_DMA_CH *channel, Dma_ChannelConfig *config)
{
	  /*collect DMA_CH_CHCFGR value*/
        Ifx_DMA_CH_CHCFGR Chcfgr_temp;
        Chcfgr_temp.U          = 0;
        Chcfgr_temp.B.TREL     = config->transferCount;
        Chcfgr_temp.B.BLKM     = config->blockMode;
        Chcfgr_temp.B.RROAT    = config->requestMode;
        Chcfgr_temp.B.CHMODE   = config->operationMode;
        Chcfgr_temp.B.CHDW     = config->moveSize;
        Chcfgr_temp.B.PRSEL    = config->requestSource;
        Chcfgr_temp.B.PATSEL   = config->pattern;

    /*collect Ifx_DMA_CH_ADICR value*/
        Ifx_DMA_CH_ADICR Adicr_temp;
        Adicr_temp.U       = 0;
        Adicr_temp.B.SMF   = config->sourceAddressIncrementStep;
        Adicr_temp.B.INCS  = config->sourceAddressIncrementDirection;
        Adicr_temp.B.CBLS  = config->sourceAddressCircularRange;
        Adicr_temp.B.SCBE  = config->sourceCircularBufferEnabled;
        Adicr_temp.B.DMF   = config->destinationAddressIncrementStep;
        Adicr_temp.B.INCD  = config->destinationAddressIncrementDirection;
        Adicr_temp.B.CBLD  = config->destinationAddressCircularRange;
        Adicr_temp.B.DCBE  = config->destinationCircularBufferEnabled;
        Adicr_temp.B.SHCT  = config->shadowControl;
        Adicr_temp.B.STAMP = config->timestampEnabled;
        Adicr_temp.B.WRPSE = config->wrapSourceInterruptEnabled;
        Adicr_temp.B.WRPDE = config->wrapDestinationInterruptEnabled;
        Adicr_temp.B.INTCT = (config->channelInterruptEnabled ? 2 : 0) | (config->channelInterruptControl ? 1 : 0);
        Adicr_temp.B.IRDV  = config->interruptRaiseThreshold;
        Adicr_temp.B.ETRL  = config->transactionRequestLostInterruptEnabled;

        channel->CHCFGR.U = Chcfgr_temp.U;							/* Register setting*/
		channel->ADICR.U = Adicr_temp.U;							/* Register setting*/
		channel->SADR.U   = config->sourceAddress;					/* Register setting*/
		channel->DADR.U   = config->destinationAddress;				/* Register setting*/

#ifdef KNOCHECK_DOUBLE_BUFFERING
		channel->SHADR.U   = config->shadowAddress;					/* Register setting*/
#endif
		channel->SDCRCR.U = config->sourceDestinationAddressCrc;	/* Register setting*/
		channel->RDCRCR.U = config->readDataCrc;					/* Register setting*/
}

Cpu_ResourceCpu Cpu_getCoreId(void)
{
    Ifx_CPU_CORE_ID reg;
    reg.U = __mfcr(CPU_CORE_ID);
    return (Cpu_ResourceCpu)reg.B.CORE_ID;
}




