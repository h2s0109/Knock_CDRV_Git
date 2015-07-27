/*
 * knocheck_interface.c
 *
 *  Created on: 2015. 5. 29.
 *      Author: G122885
 */
#include "knocheck_priv.h"
#ifdef DOUBLE_BUFFRING_TEST
#include "dsplib.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
/*CAUTION: Coeff only supports multiple of 16*/
#define DATA_TEST_COUNT 1000
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

uint16 Remain_Data_Num[DATA_TEST_COUNT]={0,};
uint16 Data_Num_Input[DATA_TEST_COUNT]={0,};
uint16 temp_Data_Num;
uint16 Data_Num_Input_last;
uint32 cntDATA;
sint16 cntDATASET = 0;
uint8 numBuffer;
KNOCHECK_ISR_FLAG ISR_Dist_Flag;

/* Input: Data in 1Q15 */
#pragma align 8
sint16 Dest_Copy_DMARAM[DATA_TEST_COUNT][BUFFER_PLUS_TWO];
sint16 Dest_Copy[ONLY_ONE][BUFFER_PLUS_TWO];

/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[KNK_KDF_ORDER];

/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly = {Coeff_Dly, 0, KNK_KDF_ORDER*2};
//#pragma align 8
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
void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl)
{
	/*Dummy function for test*/
}

void Memcopy_Test(void)
{
	Ifx_DMA *psDMA = &MODULE_DMA;
	if(cntDATA < DATA_TEST_COUNT)
	{
		Remain_Data_Num[cntDATA]=(uint16)(psDMA->CH[1].CHCSR.B.TCOUNT);
		temp_Data_Num=Remain_Data_Num[cntDATA];
		Data_Num_Input[cntDATA]=BUFFER_SIZE-Remain_Data_Num[cntDATA];
		Data_Num_Input_last= Data_Num_Input[cntDATA];
		cntDATASET++;
		switch(ISR_Dist_Flag)
		{

		case DMA_ISR_FLAG:
				if(numBuffer == BUFFER1)/*!!!CAUTION!!!!*/
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination,TRANSFER_COUNT);
					Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER0_DMA;
					Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy_DMARAM[cntDATA],(uint64*)destination_2,TRANSFER_COUNT);
					Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_1]=BUFFER1_DMA;
					Dest_Copy_DMARAM[cntDATA][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
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
			break;
		default:

			break;
		}
		cntDATA++;
	}
	else
	{
		cntDATASET++;
		temp_Data_Num=(uint16)(psDMA->CH[1].CHCSR.B.TCOUNT);
		Data_Num_Input_last=BUFFER_SIZE-temp_Data_Num;
		switch(ISR_Dist_Flag)
		{

		case DMA_ISR_FLAG:
				if(numBuffer == BUFFER1)/*!!!CAUTION!!!!*/
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy[ONLY_ONE_ARRAY],(uint64*)destination,21);
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_1]=BUFFER0_DMA;
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy[ONLY_ONE_ARRAY],(uint64*)destination_2,21);
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_1]=BUFFER1_DMA;
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_2]=cntDATASET;
				}
			break;
		case WINDOW_ISR_FLAG:
				if(numBuffer == BUFFER0)
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy[ONLY_ONE_ARRAY],(uint64*)destination,Data_Num_Input_last);
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_1]=BUFFER0_KNWINDOW;
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_2]=cntDATASET;
					cntDATASET = 0;
				}
				else
				{
					KnoCheck_MemCopy64((uint64*)Dest_Copy[ONLY_ONE_ARRAY],(uint64*)destination_2,Data_Num_Input_last);
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_1]=BUFFER1_KNWINDOW;
					Dest_Copy[ONLY_ONE_ARRAY][BUFFER_EXT_POSITION_2]=cntDATASET;
					cntDATASET = 0;
				}
			break;
		default:

			break;
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
/*End of DOUBLE_BUFFRING_TEST*/