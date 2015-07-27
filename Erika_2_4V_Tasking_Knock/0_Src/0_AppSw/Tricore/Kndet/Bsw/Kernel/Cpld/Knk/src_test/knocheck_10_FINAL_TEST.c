/*
 * knocheck_interface.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: G122885
 */
#include "knocheck_priv.h"

#ifdef FINAL
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

void Knk_SetCtrlData(uint8 Ch, uint8 InputGain, uint8 FilterIndex)
{
	/*Dummy function for test*/
}
/*End of Knk_SetCtrlData(uint8 Ch, uint8 InputGain, uint8 FilterIndex)*/

sint32 Knk_GetIntgData(uint8 Ch)
{
	/*Dummy function for test*/
	return 0;
}

void Knk_GetIntgDataEx(uint8 Ch, sint32* IntegratedValues,uint8 ValueLength)
{
	uint8 ctFilter;
	for(ctFilter = 0;ctFilter<ValueLength;ctFilter++)
	{
		IntegratedValues[ctFilter]=Knocheck_Integrated[Ch][ctFilter];
		Knocheck_Integrated[Ch][ctFilter] = 0;
	}
}

sint32 Knk_GetDetectionValue(uint8 Ch)
{
	/*Dummy function for test*/
	return 0;
}

Knk_DiagDataT Knk_GetDiagData(uint8 Ch)
{
	/*Dummy function for test*/
	Knk_DiagDataT dummy;
	dummy.DiagEvalValueAtMeasWndBeginSignalA = 0;
	dummy.DiagEvalValueAtMeasWndBeginSignalB = 0;
	dummy.MeasWndIntgCnt = 0;
	dummy.b_AddMeasureWindowLength = 0;
	return dummy;
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
		KnoCheck_Gain[cntKnoCheckIni] = KNK_GAIN1;
		for(ctFilter=0; ctFilter < KNK_PARALLELFILTER_SIZE; ctFilter++)
		{
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].base = &(KNK_DETECTION_FILTER_SET[0]);
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].index = 0;
			Knocheck_FiltInfo[cntKnoCheckIni][ctFilter].length = KNK_KDF_ORDER*2;
			Knocheck_Integrated[cntKnoCheckIni][ctFilter] = 0;
		}
	}
	Knocheck_Ready_Chan = 0;
}

#if (ENDIS_CONTROL == WINDOW_ENDIS)||(ENDIS_CONTROL == DMA_ENDIS)||(ENDIS_CONTROL == DSADC_ENDIS)

	void En_Test(void)
	{
		switch(SwithEnDis)
		{
		case 0:
			Knk_En(0,KNK_EN_ENABLE);
			Knk_En(0,KNK_EN_ENABLE);
			Knk_En(0,KNK_EN_ENABLE);
			break;
		case 1:
			Knk_En(0,KNK_EN_ENABLE);
			Knk_En(1,KNK_EN_DISABLE);
			Knk_En(2,KNK_EN_DISABLE);
			break;
		case 2:
			Knk_En(0,KNK_EN_ENABLE);
			Knk_En(1,KNK_EN_DISABLE);
			Knk_En(2,1);
			break;
		case 3:
			Knk_En(0,KNK_EN_ENABLE);
			Knk_En(1,KNK_EN_ENABLE);
			Knk_En(2,KNK_EN_DISABLE);
			break;
		case 4:
			Knk_En(0,KNK_EN_DISABLE);
			Knk_En(1,KNK_EN_DISABLE);
			Knk_En(2,KNK_EN_DISABLE);
			break;
		default:
			break;
		}
	}
#endif
	/*End of (ENDIS_CONTROL == WINDOW_ENDIS)||(ENDIS_CONTROL == DMA_ENDIS)||(ENDIS_CONTROL == DSADC_ENDIS)*/

	void Knk_En(uint8 Ch, uint8 EnDis)
	{
		KnoCheck_WinCon[Ch].Endis = EnDis;

		if(KnoCheck_WinCon[Knocheck_Ready_Chan].Endis == TRUE)
		{
			#if (ENDIS_CONTROL == WINDOW_ENDIS)
				GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT5 = 0x2;
			#elif (ENDIS_CONTROL == DMA_ENDIS)
				Ifx_DMA *psDMA = &MODULE_DMA;
				psDMA->TSR[DMA_CHANNEL_SELECT].B.ECH = 1;
			#elif (ENDIS_CONTROL == DSADC_ENDIS)
				Ifx_DMA *psDMA = &MODULE_DMA;
				psDMA->TSR[DMA_CHANNEL_SELECT].B.ECH = 1;
				DSADC_MOVE_ENGINE_RUN = TRUE;
				DSADC_CHANNEL_RUN = TRUE;
			#endif
		}
		else
		{
			#if (ENDIS_CONTROL == WINDOW_ENDIS)
				GTM_ATOM2_AGC_ENDIS_STAT.B.ENDIS_STAT5 = 0x1;
			#elif (ENDIS_CONTROL == DMA_ENDIS)
				Ifx_DMA *psDMA = &MODULE_DMA;
				psDMA->TSR[DMA_CHANNEL_SELECT].B.DCH = 1;
			#elif (ENDIS_CONTROL == DSADC_ENDIS)
				Ifx_DMA *psDMA = &MODULE_DMA;
				psDMA->TSR[DMA_CHANNEL_SELECT].B.ECH = 1;
				DSADC_MOVE_ENGINE_RUN = FALSE;
				DSADC_CHANNEL_RUN = FALSE;
			#endif
		}

	}

uint16 Knk_GetStat(uint8 Ch)
{
	/*Dummy function for test*/
	return 0;
}

void UpdateMeasureWindow_Test(void)
{
	switch(SwitchWin)
	{
	case 0:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,10);
		break;
	case 1:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,12);
		Knk_UpdateMeasureWindow((uint8)KNK_CH2,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH3,4,14);
		break;
	case 2:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,16);
		Knk_UpdateMeasureWindow((uint8)KNK_CH2,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH3,4,10);
		break;
	case 3:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,14);
		Knk_UpdateMeasureWindow((uint8)KNK_CH2,4,14);
		Knk_UpdateMeasureWindow((uint8)KNK_CH3,4,10);
		break;
	case 4:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,2,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH2,4,14);
		Knk_UpdateMeasureWindow((uint8)KNK_CH3,3,12);
		break;
	default:
		Knk_UpdateMeasureWindow((uint8)KNK_CH1,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH2,4,10);
		Knk_UpdateMeasureWindow((uint8)KNK_CH3,4,10);
		break;
	}
}

void Knk_UpdateMeasureWindow(uint8 Ch, uint16 StartAngle,uint16 EndAngle)
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

void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl)
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
				Knocheck_FiltInfo[Ch][ctFilter].length = KNK_KDF_ORDER*2;
			}
	}
	else
	{
		Errortrace |= CHANNEL_OVER;
	}
}
uint8 gain_test=0;
void SetCtrlData_Test(void)
{
	Knk_SetCtrlDataEx(Knocheck_Ready_Chan,gain_test,FilterIndices,3,0);

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
#if(FINAL==CONTROL_EN)
#pragma section code ".text.cpu1_psram"
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
		for(cntDly =0; cntDly <KNK_KDF_ORDER;cntDly++)
		{
			Coeff_Dly[cntFilt][cntDly] = 0;
		}
	}
}

#elif(FINAL==CONTROL_DIS)
#pragma section code "cpu1_psram"
	void Memcopy_Test(void)
	{
		if(cntDATA < DATA_TEST_COUNT)
		{
			cntDATASET++;
			Ifx_DMA *psDMA = &MODULE_DMA;
			Remain_Data_Num[cntDATA]= (uint16)psDMA->CH[DMA_CHANNEL_SELECT].CHCSR.B.TCOUNT;
			temp_Data_Num=Remain_Data_Num[cntDATA];
			Knocheck_NumIputData[cntDATA]=BUFFER_SIZE-Remain_Data_Num[cntDATA];
			Knocheck_NumIputData_last= Knocheck_NumIputData[cntDATA];
			switch(KnoCheck_IsrDistFlg)
			{
			case DMA_ISR_PRE_FLAG:
				if(KnoCheck_NumBuffer == BUFFER1)/*!!!CAUTION!!!!*/
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer0_Dest,PRE_BUFFER_SIZE);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_preDMA;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer1_Dest,PRE_BUFFER_SIZE);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_preDMA;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;

				}
				KnoCheck_FirNumInput = PRE_BUFFER_SIZE;
				break;
			case DMA_ISR_FLAG:
				if(KnoCheck_NumBuffer == BUFFER1)/*!!!CAUTION!!!!*/
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer0_Dest,BUFFER_SIZE);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_DMA;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer1_Dest,BUFFER_SIZE);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_DMA;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;

				}
				KnoCheck_FirNumInput = BUFFER_SIZE;
				break;
			case WINDOW_ISR_FLAG:
				if(KnoCheck_NumBuffer == BUFFER0)
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer0_Dest,Knocheck_NumIputData_last);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_KNWINDOW;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
					cntDATASET = 0;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Knocheck_DestCopyRam[cntDATA],(uint64*)Buffer1_Dest,Knocheck_NumIputData_last);
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_KNWINDOW;
					Knocheck_DestCopyRam[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
					cntDATASET = 0;
				}
				KnoCheck_FirNumInput = Knocheck_NumIputData_last;
				break;
			default:
				break;
			}

		}
	}
#pragma section code restore
#pragma section code "cpu1_psram"
	void FIR_Filter_Test(void)
	{
		uint8 cntFilt;
		if(cntDATA < DATA_TEST_COUNT)
		{
			for(cntFilt = 0; cntFilt <KnoCheck_RunFiltNum ;cntFilt++)
			{

				switch(cntFilt)
				{
				case 0:
							Ifx_firRealQ15a (Knocheck_DestCopyRam[cntDATA],Knocheck_FilterResult[cntDATA][cntFilt], Knocheck_FiltInfo[KnoCheck_RunChan][cntFilt], &cptr_Coeff_Dly_0, KnoCheck_FirNumInput);
					break;
				case 1:
							Ifx_firRealQ15a (Knocheck_DestCopyRam[cntDATA],Knocheck_FilterResult[cntDATA][cntFilt], Knocheck_FiltInfo[KnoCheck_RunChan][cntFilt], &cptr_Coeff_Dly_1, KnoCheck_FirNumInput);
					break;
				case 2:
							Ifx_firRealQ15a (Knocheck_DestCopyRam[cntDATA],Knocheck_FilterResult[cntDATA][cntFilt], Knocheck_FiltInfo[KnoCheck_RunChan][cntFilt], &cptr_Coeff_Dly_2, KnoCheck_FirNumInput);
					break;
				default:
					break;
				}
				/*End of switch(cntFilt)*/

				/*Coeff_Dly has information which was calculated before task*/
				if (KnoCheck_IsrDistFlg == WINDOW_ISR_FLAG)
				{
					for(int i =0; i <KNK_KDF_ORDER;i++)
					{
						Coeff_Dly[cntFilt][i] = 0;
					}
				}
			}/*End of for(cntFilt = 0; cntFilt <KnoCheck_RunFiltNum ;cntFilt++)*/
			cntDATA++;
			if(cntDATA ==DATA_TEST_COUNT-1)
			{
				cntDATA =0;
			}
		}
	}
#pragma section code restore
#endif
/*End of #if(FINAL==CONTROL_EN)*/

#endif
	/*End of FINAL*/
