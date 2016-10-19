/*
 * knocheck_interface.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: G122885
 */
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

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void knoCheck_GetIntegData(uint8 Ch, sint32* IntegratedValues,uint8 ValueLength)
{
	uint8 ctFilter;
	for(ctFilter = 0;ctFilter<ValueLength;ctFilter++)
	{
		IntegratedValues[ctFilter]=Knocheck_Integrated[Ch][ctFilter];
		Knocheck_Integrated[Ch][ctFilter] = 0;
	}
}


void Knocheck_Ini(void)
{
	uint8 cntKnoCheckIni;
	uint8 ctFilter;
	for(cntKnoCheckIni=0; cntKnoCheckIni < MAX_CHANNEL; cntKnoCheckIni++)
	{
		KnoCheck_WinCon[cntKnoCheckIni].Endis = TRUE;
		KnoCheck_WinCon[cntKnoCheckIni].StartAngle = 4;
		KnoCheck_WinCon[cntKnoCheckIni].EndAngle = 10;
		KnoCheck_Gain[cntKnoCheckIni] = kNOCHECK_GAIN1;
		for(ctFilter=0; ctFilter < KNK_PARALLELFILTER_SIZE; ctFilter++)
		{
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].base = &(KNK_DETECTION_FILTER_SET[0]);
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].index = 0;
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].length = KNOCHECK_FILTER_ORDER*2;
			Knocheck_Integrated[cntKnoCheckIni][ctFilter] = 0;
		}
	}
	Knocheck_Ready_Chan = 0;
}


void KnoCheck_ASWControl(void)
{
	   knoCheck_Enable(KNOCK_CH_1,kNOCHECK_CH_ENABLE);
	   knoCheck_UpdateMeasureWin((uint8)KNOCK_CH_1,4,10);
	   knoCheck_Control(Knocheck_Ready_Chan,kNOCHECK_GAIN1,FilterIndices,3);
}

void knoCheck_Enable(uint8 Ch, uint8 EnDis)
{
	KnoCheck_WinCon[Ch].Endis = EnDis;

	if(KnoCheck_WinCon[Knocheck_Ready_Chan].Endis == TRUE)
	{
			Ifx_DMA *psDMA = &MODULE_DMA;
			psDMA->TSR[DMA_CHANNEL_SELECT].B.ECH = 1;
			DSADC_MOVE_ENGINE_RUN = TRUE;
			DSADC_CHANNEL_RUN = TRUE;
	}
	else
	{
			Ifx_DMA *psDMA = &MODULE_DMA;
			psDMA->TSR[DMA_CHANNEL_SELECT].B.ECH = 1;
			DSADC_MOVE_ENGINE_RUN = FALSE;
			DSADC_CHANNEL_RUN = FALSE;
	}

}

void knoCheck_UpdateMeasureWin(uint8 Ch, uint16 StartAngle,uint16 EndAngle)
{
	volatile uint32 currentAngle_temp;
	volatile uint32 currentAngle_temp2;
	uint32 currentAngle;
	KnoCheck_WinCon[Ch].StartAngle	= StartAngle;
	KnoCheck_WinCon[Ch].EndAngle	= EndAngle;

/*	if(KnoCheck_WinCon[Knocheck_Ready_Chan].Endis == TRUE)
	{*/
		currentAngle = (GTM_TBU_CH1_BASE.U & 0x00FFFFFF);
		currentAngle_temp = KNOCK_WINDOW_OPEN_SR.U;				/*To update value, Should be read*/
		currentAngle_temp2 = KNOCK_WINDOW_CLOSE_SR.U;				/*To update value, Should be read*/
		KNOCK_WINDOW_OPEN_CM.U = currentAngle+273*KnoCheck_WinCon[Knocheck_Ready_Chan].StartAngle;
		KNOCK_WINDOW_CLOSE_CM.U = currentAngle+273*KnoCheck_WinCon[Knocheck_Ready_Chan].EndAngle;
	/*}*/

}

void knoCheck_Control(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength)
{
	/* rl is dead parameter need to delete*/
	uint8 ctFilter;
	uint8 selFilter;

	Ifx_DSADC *psDSADC = &MODULE_DSADC;

	Errortrace = NORMAL;

	KnoCheck_Gain[Ch] = InputGain;
	if(KnoCheck_WinCon[Knocheck_Ready_Chan].Endis == TRUE)
	{
		psDSADC->CH[DSADC_CHANNEL_SELECT].MODCFG.U= (0x8000|KnoCheck_Gain[0]<<4);
	}

	/*Channel number check procedure*/
	if (Ch < KNK_CH_LENGTH)
	{
			/* The number of filter check procedure*/
			if(IndexLength <= KNK_PARALLELFILTER_SIZE)
			{
				KnoCheck_RunFiltNum = IndexLength;
			}
			else
			{
				KnoCheck_RunFiltNum = KNK_PARALLELFILTER_SIZE;
				Errortrace |= FILTER_INDICES_OVER;
			}

			for (ctFilter = 0; ctFilter < KnoCheck_RunFiltNum; ctFilter++)
			{
				/*Filter selection check procedure*/
				selFilter = FilterIndices[ctFilter];
				if (selFilter >= KNK_FILTERSET_SIZE)
				{
					selFilter = KNK_FILTERSET_SIZE - 1; /*Selected filter is range over*/
					Errortrace |= FILTERSET_OVER;
				}
				Knocheck_FiltInfo[Ch][ctFilter].base = &(KNK_DETECTION_FILTER_SET[selFilter]);
				Knocheck_FiltInfo[Ch][ctFilter].index = 0;
				Knocheck_FiltInfo[Ch][ctFilter].length = KNOCHECK_FILTER_ORDER*2;
			}
	}
	else
	{
		Errortrace |= CHANNEL_OVER;
	}
}

#pragma section code "cpu1_psram"
void KnoCheck_MemCopy64(uint64* xDest_pu64,uint64* xSrc_pcu64, uint16 numBytes)
{
    uint8 i;
    uint8 ctLoop64;
    uint8 ctLoop16;
    uint16* xDest_pu16;
    uint16* xSrc_pcu16;
    ctLoop64= numBytes/4;
    ctLoop16= numBytes%4;

    for(i = 0;i<ctLoop64;i++ )
    {
        *xDest_pu64=*xSrc_pcu64;
        ++xDest_pu64;
        ++xSrc_pcu64;
    }
    xDest_pu16 = (uint16*)xDest_pu64;
    xSrc_pcu16 = (uint16*)xSrc_pcu64;
    for(i = 0;i<ctLoop16;i++ )
    {
        *xDest_pu16++=*xSrc_pcu16++;
    }
}
#pragma section code restore


#pragma section code "cpu1_psram"
void KnoCheck_Memcopy(void)
{
		uint16 numRemainData;
		Ifx_DMA *psDMA = &MODULE_DMA;
		numRemainData = (uint16)psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.TCOUNT;
		Knocheck_NumIputData=(uint16)BUFFER_SIZE-numRemainData;
		switch(KnoCheck_IsrDistFlg)
		{
		case DMA_ISR_PRE_FLAG:
			if(KnoCheck_NumBuffer == BUFFER1)/*!!!CAUTION!!!!*/
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer0_Dest,PRE_BUFFER_SIZE);
			}
			else
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer1_Dest,PRE_BUFFER_SIZE);
			}
			KnoCheck_FirNumInput = PRE_BUFFER_SIZE;
			break;
		case DMA_ISR_FLAG:
			if(KnoCheck_NumBuffer == BUFFER1)/*!!!CAUTION!!!!*/
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer0_Dest,(uint16)BUFFER_SIZE);
			}
			else
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer1_Dest,(uint16)BUFFER_SIZE);
			}
			KnoCheck_FirNumInput = BUFFER_SIZE;
			break;
		case WINDOW_ISR_FLAG:
			if(KnoCheck_NumBuffer == BUFFER0)
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer0_Dest,Knocheck_NumIputData);
			}
			else
			{
				KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam,(uint64*)Buffer1_Dest,Knocheck_NumIputData);
			}
			KnoCheck_FirNumInput = Knocheck_NumIputData;
			break;
		default:
			break;
		}
}
#pragma section code restore
void KnoCheck_Filter(void)
{
	uint8 cntFilt;
	for(cntFilt = 0; cntFilt <KnoCheck_RunFiltNum ;cntFilt++)
	{
		switch(cntFilt)
		{
		case 0:
					Ifx_firRealQ15a (Knocheck_DestCopyRam,Knocheck_FilterResult[cntFilt], Knocheck_FiltInfo[Knocheck_Ready_Chan][cntFilt], &cptr_Coeff_Dly_0, KnoCheck_FirNumInput);
			break;
		case 1:
					Ifx_firRealQ15a (Knocheck_DestCopyRam,Knocheck_FilterResult[cntFilt], Knocheck_FiltInfo[Knocheck_Ready_Chan][cntFilt], &cptr_Coeff_Dly_1, KnoCheck_FirNumInput);
			break;
		case 2:
					Ifx_firRealQ15a (Knocheck_DestCopyRam,Knocheck_FilterResult[cntFilt], Knocheck_FiltInfo[Knocheck_Ready_Chan][cntFilt], &cptr_Coeff_Dly_2, KnoCheck_FirNumInput);
			break;
		default:
			break;
		}
		/*End of switch(cntFilt)*/
		if(KnoCheck_IgnDataCnt>=5)
		{
			for(uint8 cntInteg = 0;cntInteg<KnoCheck_FirNumInput;cntInteg++)
			{
				Knocheck_Integr[cntFilt][cntInteg] 	=ABS(Knocheck_FilterResult[cntFilt][cntInteg]);
				Knocheck_Integrated[Knocheck_Ready_Chan][cntFilt] = Knocheck_Integrated[Knocheck_Ready_Chan][cntFilt]+(sint32)Knocheck_Integr[cntFilt][cntInteg];
			}
		}
	}
	/*End of for(cntFilt = 0; cntFilt <KnoCheck_RunFiltNum ;cntFilt++)*/
}

void KnoCheck_ClearCoeffDly(void)
{
	uint8 cntFilt;
	uint8 cntDly;
	for(cntFilt = 0; cntFilt <KnoCheck_RunFiltNum ;cntFilt++)
	{
		for(cntDly =0; cntDly <KNOCHECK_FILTER_ORDER;cntDly++)
		{
			Coeff_Dly[cntFilt][cntDly] = 0;
		}
	}
}
