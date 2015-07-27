/*
 * knocheck_interface.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: G122885
 */
#include "knocheck_priv.h"

#ifdef CHANNEL_FILTER_SELECT_TEST
#include "dsplib.h"

/*테스트 체크 포인트
Dest_Copy_DMARAM
배열의 마지만 카운트 값이 1 부터 순차적으로 증가하는지 확인한다.
Buffer 0와 buffer1 교차되면서 들어오는지 확인한다.
마지막에 Window ISR의 DMA값이 들어오는지 확인한다.
Filter_result의 값이 필터 Coeff와 제대로 계산되었는지 확인한다.
v.w Knocheck_FiltInfo;Coeff;Filter_result;Dest_Copy_DMARAM;Data_Num_Input*/


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define DATA_TEST_COUNT 50
/*CAUTION: Coeff only supports multiple of 16
 * If you want use less than the number of 16, please fill the value with 0.
 */
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
uint8 FilterIndices[KNK_PARALLELFILTER_SIZE] ={0,1,2};
uint8 Errortrace;
uint8 Filter_select = 0;
uint16 Remain_Data_Num[DATA_TEST_COUNT]={0,};
uint16 Data_Num_Input[DATA_TEST_COUNT]={0,};
uint16 temp_Data_Num;
uint16 Data_Num_Input_last;
uint32 cntDATA;
sint16 cntDATASET = 0;
uint8 numBuffer;
KNOCHECK_ISR_FLAG ISR_Dist_Flag;
#pragma section neardata
uint16 Input_Data_NUM;
uint8 Run_Chan;
uint8 Run_FiltNum;
#pragma section neardata
#pragma align 8
sint16 Filter_result[DATA_TEST_COUNT][KNK_PARALLELFILTER_SIZE][BUFFER_PLUS_TWO];
/* Input: Data in 1Q15 */
#pragma section neardata
#pragma align 8
sint16 Dest_Copy_DMARAM[DATA_TEST_COUNT][BUFFER_PLUS_TWO];

/* Input: Data in 1Q15 */
#ifdef REAL_FILTER_NUM_TEST
//#pragma section nearrom
#pragma align 8
sint16   Coeff[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] =
{
		{
			0x8000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000
			/*0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000*/
		},
		{
				0x8000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000
				/*0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000*/
		},
		{
				0x8000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000
				/*0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000*/
		}

};
#else
sint16   Coeff[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] =
{
		{
			0x8000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000
		},
		{
			0x8000,0x8000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000
		},
		{
			0x8000,0x8000,0x8000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000,
			0x0000,0x0000,0x0000,0x0000
		},

};
#endif
/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[KNK_PARALLELFILTER_SIZE][KNK_KDF_ORDER];


KnoCirBuff cptr_Coeff= {Coeff, 0, KNK_KDF_ORDER*2};
/* CircBuf for Filter Coefficients (Coeff) */
/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly_0 = {Coeff_Dly[0], 0, KNK_KDF_ORDER*2};
KnoCirBuff cptr_Coeff_Dly_1 = {Coeff_Dly[1], 0, KNK_KDF_ORDER*2};
KnoCirBuff cptr_Coeff_Dly_2 = {Coeff_Dly[2], 0, KNK_KDF_ORDER*2};
KnoCirBuff Knocheck_FiltInfo[KNK_CH_LENGTH][KNK_FILTERSET_SIZE];

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

void Knk_SetCtrlData(uint8 Ch, uint8 InputGain, uint8 FilterIndex)
{
	/*Dummy function for test*/
}
sint32 Knk_GetIntgData(uint8 Ch)
{
	/*Dummy function for test*/
	return 0;
}
void Knk_GetIntgDataEx(uint8 Ch, sint32* IntegratedValues,uint8 ValueLength)
{
	/*Dummy function for test*/
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
void Knk_En(uint8 Ch, uint8 Enable)
{
	/*Dummy function for test*/
}
uint16 Knk_GetStat(uint8 Ch)
{
	/*Dummy function for test*/
	return 0;
}
void Knk_UpdateMeasureWindow(uint8 Ch, uint16 StartAngle,uint16 EndAngle)
{
	/*Dummy function for test*/
}
void Filter_Selection(void)
{
	switch(Filter_select)
	{
	case 0:
		FilterIndices[0]=0;
		break;
	case 1:
		FilterIndices[0]=1;
		break;
	case 2:
		FilterIndices[0]=2;
		break;
	default:
		FilterIndices[0]=2;
		break;
	}
}
void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl)
{
	/* rl is dead parameter need to delete*/
	uint8 ctFilter;
	uint8 selFilter;

	Errortrace = NORMAL;
	/*Store the current Channel number*/
	Run_Chan = Ch;
	/*Channel number check procedure*/
	if (Ch < KNK_CH_LENGTH)
	{
			/* The number of filter check procedure*/
			if(IndexLength <= KNK_PARALLELFILTER_SIZE)
			{
				Run_FiltNum=IndexLength;
			}
			else
			{
				Run_FiltNum=KNK_PARALLELFILTER_SIZE;
				Errortrace |= FILTER_INDICES_OVER;
			}

			for (ctFilter = 0; ctFilter < Run_FiltNum; ctFilter++)
			{
				/*Filter selection check procedure*/
				selFilter = FilterIndices[ctFilter];
				if (selFilter >= KNK_FILTERSET_SIZE)
				{
					selFilter = KNK_FILTERSET_SIZE - 1; /*Selected filter is range over*/
					Errortrace |= FILTERSET_OVER;
				}
				Knocheck_FiltInfo[Ch][ctFilter].base = &(Coeff[selFilter]);
				Knocheck_FiltInfo[Ch][ctFilter].index = 0;
				Knocheck_FiltInfo[Ch][ctFilter].length = KNK_KDF_ORDER*2;
			}
	}
	else
	{
		Errortrace |= CHANNEL_OVER;
	}
}



void SetCtrlData_Test(void)
{
	Knk_SetCtrlDataEx(0,0,FilterIndices,1,0);
}
#pragma section code ".text.cpu1_psram"
void Memcopy_Test(void)
{
	//Ifx_DMA *psDMA = &MODULE_DMA;
	if(cntDATA < DATA_TEST_COUNT)
	{
		cntDATASET++;
		Remain_Data_Num[cntDATA]=(uint16)(DMA_CH1_CHCSR.B.TCOUNT);
		temp_Data_Num=Remain_Data_Num[cntDATA];
		Data_Num_Input[cntDATA]=BUFFER_SIZE-Remain_Data_Num[cntDATA];
		Data_Num_Input_last= Data_Num_Input[cntDATA];
		switch(ISR_Dist_Flag)
		{

		case DMA_ISR_FLAG:
			if(numBuffer == BUFFER1)/*!!!CAUTION!!!!*/
			{
				KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination,BUFFER_SIZE);
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_DMA;
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
			}
			else
			{
				KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination_2,BUFFER_SIZE);
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_DMA;
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;

			}
			Input_Data_NUM = BUFFER_SIZE;
			break;
		case WINDOW_ISR_FLAG:
			if(numBuffer == BUFFER0)
			{
				KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination,Data_Num_Input_last);
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_KNWINDOW;
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				cntDATASET = 0;

			}
			else
			{
				KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination_2,Data_Num_Input_last);
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_KNWINDOW;
				Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				cntDATASET = 0;
			}
			Input_Data_NUM = Data_Num_Input_last;
			break;
		default:
			break;
		}

	}
}
uint8 cntFilt =0;
#pragma section code ".text.cpu1_psram"
void FIR_Filter_Test(void)
{
	cntFilt =0;
	if(cntDATA < DATA_TEST_COUNT)
	{
		for(cntFilt = 0; cntFilt <Run_FiltNum ;cntFilt++)
		{
			switch(cntFilt)
			{
			case 0:
				#ifdef OLD_TYPE
					FirBlk_4_16 (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_0, Input_Data_NUM);
				#else
					if(P14_OUT.B.P2 == TRUE) /*only for debugging*/
					{
						P14_OMCR.B.PCL2=1;
					}/*only for debugging*/
					else
					{
						P14_OMSR.B.PS2=1;
					}
					Ifx_firRealQ15a (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_0, Input_Data_NUM);

					if(P14_OUT.B.P8 == TRUE) /*only for debugging*/
					{
						P14_OMCR.B.PCL8=1;
					}/*only for debugging*/
					else
					{
						P14_OMSR.B.PS8=1;
					}
				#endif
				break;
			case 1:
				#ifdef OLD_TYPE
					FirBlk_4_16 (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_1, Input_Data_NUM);
				#else
					Ifx_firRealQ15a (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_1, Input_Data_NUM);
				#endif
				break;
			case 2:
				#ifdef OLD_TYPE
					FirBlk_4_16 (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_2, Input_Data_NUM);
				#else
					Ifx_firRealQ15a (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_2, Input_Data_NUM);
				#endif
				break;
			default:
				Ifx_firRealQ15a (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA][cntFilt], Knocheck_FiltInfo[Run_Chan][cntFilt], &cptr_Coeff_Dly_0, Input_Data_NUM);
				break;
			}

			if (ISR_Dist_Flag == WINDOW_ISR_FLAG)
			{
				for(int i =0; i <KNK_KDF_ORDER;i++)
				{
					Coeff_Dly[cntFilt][i] = 0;
				} //*Coeff_Dly has information which was calculated before task
			}
		}
		cntDATA++;
		if(cntDATA ==DATA_TEST_COUNT-1)
		{
			cntDATA =0;
		}
	}
}


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
#endif
/*End of SIMPLE_FILTER_DOUBLEBUF_TEST*/
