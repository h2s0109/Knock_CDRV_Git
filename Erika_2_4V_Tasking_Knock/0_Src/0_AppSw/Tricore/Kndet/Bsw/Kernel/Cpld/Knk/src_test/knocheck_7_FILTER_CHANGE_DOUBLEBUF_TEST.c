/*
 * knocheck_interface.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: G122885
 */
#include "knocheck_priv.h"
#ifdef CHANGE_FILTER_DOUBLEBUF_TEST
#include "dsplib.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define DATA_TEST_COUNT 100
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
uint16 Input_Data_NUM;

#pragma align 8
sint16 Filter_result[DATA_TEST_COUNT][BUFFER_PLUS_TWO];
/* Input: Data in 1Q15 */

#pragma align 8
sint16 Dest_Copy_DMARAM[DATA_TEST_COUNT][BUFFER_PLUS_TWO];

/* Input: Data in 1Q15 */
#pragma align 8
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
/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[KNK_KDF_ORDER];


KnoCirBuff cptr_Coeff= {Coeff, 0, KNK_KDF_ORDER*2};
/* CircBuf for Filter Coefficients (Coeff) */
/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly = {Coeff_Dly, 0, KNK_KDF_ORDER*2};
KnoCirBuff Knocheck_filter_Total[KNK_CH_LENGTH];

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
	uint8 numFilter;
	uint8 ctFilter;
	uint8 Filter_sel;

	Errortrace = NORMAL;
	/*Channel number check procedure*/
	if (Ch < KNK_CH_LENGTH)
	{
			/* The number of filter check procedure*/
			if(IndexLength <= KNK_PARALLELFILTER_SIZE)
			{
				numFilter=IndexLength;
			}
			else
			{
				numFilter=KNK_PARALLELFILTER_SIZE;
				Errortrace |= FILTER_INDICES_OVER;
			}

			for (ctFilter = 0; ctFilter <numFilter; ctFilter++)
			{
				/*Filter selection check procedure*/
				Filter_sel = FilterIndices[ctFilter];
				if (Filter_sel >= KNK_FILTERSET_SIZE)
				{
					Filter_sel = KNK_FILTERSET_SIZE - 1; /*Selected filter is range over*/
					Errortrace |= FILTERSET_OVER;
				}
#ifndef CHANGE_FILTER_DOUBLEBUF_TEST
				Knocheck_filter_Total[Ch].base = &(Knk_DetectionFilterSet[Filter_sel]);
#else
				Knocheck_filter_Total[Ch].base = &(Coeff[Filter_sel]);
#endif
				Knocheck_filter_Total[Ch].index = 0;
				Knocheck_filter_Total[Ch].length = KNK_KDF_ORDER*2;
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

void Memcopy_Test(void)
{
	Ifx_DMA *psDMA = &MODULE_DMA;
	if(cntDATA < DATA_TEST_COUNT)
	{
		cntDATASET++;
		Remain_Data_Num[cntDATA]=(uint16)(psDMA->CH[1].CHCSR.B.TCOUNT);
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

void FIR_Filter_Test(void)
{
	if(cntDATA < DATA_TEST_COUNT)
	{
		Ifx_firRealQ15a (Dest_Copy_DMARAM[cntDATA],Filter_result[cntDATA], Knocheck_filter_Total[0], &cptr_Coeff_Dly, Input_Data_NUM);
		if (ISR_Dist_Flag == WINDOW_ISR_FLAG)
		{
			for(int i =0; i <KNK_KDF_ORDER;i++)
			{
				Coeff_Dly[i] = 0;
			} //*Coeff_Dly has information which was calculated before task
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
