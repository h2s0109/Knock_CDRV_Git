/*
 * knocheck_total_filtering_test.c
 *
 *  Created on: 2015. 5. 26.
 *      Author: G122885
 */
#include "knocheck_priv.h"
#include "dsplib.h"

#ifdef TOTAL_FILTER_TEST
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define     nInput     20                  /*Number of Input Data*/
#if (FILTER_NUM == 64)
#define     nCoeff     64                  //Filter Order
#elif (FILTER_NUM == 16)
#define     nCoeff     16                  //Filter Order
#endif
/*CAUTION: The number of Coeff should be over 16.
 * If you want use less than the number of 16, please fill the value with 0.
 */
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

typedef enum
{
	NORMAL       = 0,
	FILTER_INDICES_OVER = 1,
	FILTERSET_OVER = 2,
	CHANNEL_OVER = 4
} FILTER_SEL_EXP;

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
uint8 FilterIndices[KNK_PARALLELFILTER_SIZE] ={0,2,4,5,12};
uint8 Errortrace;
uint8 Filter_select = 0;

#pragma align 8
#if (FILTER_NUM == 64)
sint16 KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] =
{

					{ 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0x8000,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0},

					{ 0x8000,0x8000,0x8000,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0x8000,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0},

					{ 0,0,0x8000,0x8000,0,0,0x8000,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,0,0,
					 0,0,0,0}
};
#elif (FILTER_NUM == 16)
sint16 KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] =
{

					{
					0x8000,0x8000,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
					},
					{
					0x8000,0x8000,0x8000,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
					}
};
#endif

/* Input: Data in 1Q15 */
#pragma align 8
sint16 destination_copy[nInput];
/* Result: Data in 1Q15 */
#pragma align 8
sint16 destination_copy_res[nInput];

/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[nCoeff];

/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly = {Coeff_Dly, 0, nCoeff*2};
//#pragma align 8
KnoCirBuff Knocheck_filter_Total[KNK_CH_LENGTH];

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
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

				Knocheck_filter_Total[Ch].base = &(KNK_DETECTION_FILTER_SET[Filter_sel]);
				Knocheck_filter_Total[Ch].index = 0;
				Knocheck_filter_Total[Ch].length = KNK_KDF_ORDER*2;
			}
	}
	else
	{
		Errortrace |= CHANNEL_OVER;
	}
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

void SetCtrlData_Total_Filter_Test(void)
{
	Knk_SetCtrlDataEx(0,0,FilterIndices,1,0);
}

void Memcopy_Test(void)
{
	KnoCheck_MemCopy64((uint64*)destination_copy,(uint64*)destination,nInput);
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


void FIR_Filter_Test_Total(void)
{
    for(int i =0; i <nCoeff;i++)
    {
    	Coeff_Dly[i] =0;
    } /*Coeff_Dly has information which was calculated before task*/

	Ifx_firRealQ15a (destination_copy, destination_copy_res, Knocheck_filter_Total[0], &cptr_Coeff_Dly, nInput);
}

#endif /*End of TOTAL_FILTER_TEST*/
