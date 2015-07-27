/*
 * knocheck_DYNAMIC_DATA_FILTER_TEST.c
 *
 *  Created on: 2015. 5. 27.
 *      Author: G122885
 */

#include "knocheck_priv.h"
#include "dsplib.h"


#ifdef DYNAMIC_DATA_FILTER_TEST
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define     nInput     20                  //Number of Input Data
#define     nCoeff     16                  //Filter Order
/*CAUTION: The number of Coeff should be over 16.
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
/* Input: Data in 1Q15 */
#pragma align 8
sint16 destination_copy[nInput];
#pragma align 8
sint16 destination_copy_res[nInput];
/* Output: Result Buffer in 1Q15 */
sint16  Filter_result[nInput];
/* Filter Coefficients in 1Q15 */
#pragma align 8
sint16   Coeff[nCoeff] =
{
		0x8000,0x8000,0x8000,0,0,0,0,0,0,0,0,0,0,0,0,0
};
KnoCirBuff cptr_Coeff= {Coeff, 0, nCoeff*2};
/* CircBuf for Filter Coefficients (Coeff) */

/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[nCoeff];

/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly = {Coeff_Dly, 0, nCoeff*2};

/*At here, 0x8000 means -1*/
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/

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

void FIR_Filter_Test(void)
{
    for(int i =0; i <nCoeff;i++)
    {
    	Coeff_Dly[i] =0;
    } /*Coeff_Dly has information which was calculated before task*/

	Ifx_firRealQ15a (destination_copy, destination_copy_res, cptr_Coeff, &cptr_Coeff_Dly, nInput);
}


#endif /*End of HMC_FILTER_TEST*/
