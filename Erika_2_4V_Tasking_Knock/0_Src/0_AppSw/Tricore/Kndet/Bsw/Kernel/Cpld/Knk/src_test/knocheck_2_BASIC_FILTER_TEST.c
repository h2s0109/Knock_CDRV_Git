/*
 * knocheck_BASIC_FILTER_TEST.c
 *
 *  Created on: 2015. 5. 26.
 *      Author: G122885
 */
#include "knocheck_priv.h"
#include "dsplib.h"


#ifdef BASIC_FILTER_TEST

#ifdef INPUT_BIGGER

	#ifdef FILTER_SIMPL_FOR_MAIL
		#define     nInput     9                  //Number of Input Data
		#define     nCoeff     8                  //Filter Order
		/*CAUTION: The number of Coeff should be over 16.
		 * If you want use less than the number of 16, please fill the value with 0.
		 */
		/* Input: Data in 1Q15 */

		#pragma align 8
		sint16   Coeff[nCoeff] =
		{
				0x8000,0x8000,0x0000,0x8000,
				0x0000,0x0000,0x8000,0x8000,
		};
		#pragma align 8
		sint16   Input_Sig[nInput]=
		{
		 0x0003,0x0004,0x0005,0x0008,
		 0x0009,0x0007,0x0002,0x0001,
		 0x0000
		};
	#else
		#define     nInput     24                  //Number of Input Data
		#define     nCoeff     16                  //Filter Order
		/*CAUTION: The number of Coeff should be over 16.
		 * If you want use less than the number of 16, please fill the value with 0.
		 */
		/* Input: Data in 1Q15 */

		#pragma align 8
		sint16   Coeff[nCoeff] =
		{
				0x8000,0x8000,0x8000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x8000,
		};
		#pragma align 8
		sint16   Input_Sig[nInput]=
		{
		 0x0005,0x0004,0x0003,0x0002,0x0002,0x0002,
		 0x0002,0x0002,0x0002,0x0002,0x0002,0x0002,
		 0x0002,0x0002,0x0002,0x0002,0x0002,0x0002,
		 0x0002,0x0002,0x0002,0x0002,0x0002,0x0005
		// 0x0002,0x0002,0x0002,0x0002,0x0002
		};
	#endif

	sint16  Filter_result[nInput];
#endif /*End of INPUT_BIGGER*/

#ifdef FILTER_CALC_TEST

	#define     nInput     4                  //Number of Input Data
	#define     nCoeff     8                  //Filter Order
	/*CAUTION: The number of Coeff should be over 16.
	 * If you want use less than the number of 16, please fill the value with 0.
	 */
	uint8 Fir_count = 0;
	/* Input: Data in 1Q15 */

	#pragma align 8
	sint16   Input_Sig_1[nInput]=
	{
			0x0003,0x0004,0x0005,0x0008
	};
	#pragma align 8
	sint16   Input_Sig_2[nInput]=
	{
			0x0000,0x0000,0x0000,0x0000
	};
	#pragma align 8
	sint16   Input_Sig_3[nInput]=
	{
			0x0000,0x0000,0x0000,0x0000
	};

	/* Output: Result Buffer in 1Q15 */
	sint16  Filter_result_1[20];
	sint16  Filter_result_2[20];
	sint16  Filter_result_3[20];
	sint16  Filter_result_4[20];
	sint16  Filter_result_5[20];
	sint16  Filter_result_6[20];

	/* Filter Coefficients in 1Q15 */
	#pragma align 8
	sint16   Coeff[nCoeff] =
	{
			0x8000,0x8000,0x0000,0x8000,
			0x0000,0x0000,0x8000,0x8000,
	};
	/*At here, 0x8000 means -1*/
#endif /*End of FILTER_CALC_TEST*/

KnoCirBuff cptr_Coeff= {Coeff, 0, nCoeff*2};
/* CircBuf for Filter Coefficients (Coeff) */

/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[nCoeff];

#ifdef FILTER_CALC_TEST
	#pragma align 8
	sint16  Coeff_Dly_result[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_1[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_2[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_3[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_4[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_5[nCoeff];
	#pragma align 8
	sint16  Coeff_Dly_inter_6[nCoeff];
#endif  /*End of FILTER_CALC_TEST*/

/* CircBuf for delayLine */
KnoCirBuff cptr_Coeff_Dly = {Coeff_Dly_result, 0, nCoeff*2};

void FIR_Filter_Test(void)
{
#ifdef INPUT_BIGGER

	#ifdef FILTER_SIMPL_FOR_MAIL
		FirBlk_4_16 (Input_Sig, Filter_result, cptr_Coeff, &cptr_Coeff_Dly, nInput);
	#else
		Ifx_firRealQ15a (Input_Sig, Filter_result, cptr_Coeff, &cptr_Coeff_Dly, nInput);
	#endif
	for(int i =0; i <nCoeff;i++)
	{
		Coeff_Dly[i] = 0;
	} //*Coeff_Dly has information which was calculated before task

#elif FILTER_CALC_TEST
	//FirBlk_4_16(Input_Sig, Filter_result, cptr_Coeff, &cptr_Coeff_Dly, nInput);
	if(Fir_count == 0)
	{
		FirBlk_4_16(Input_Sig_1, Filter_result_1, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_1[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}
	else if(Fir_count == 1)
	{
		FirBlk_4_16(Input_Sig_2, Filter_result_2, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_2[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}
	else if(Fir_count == 2)
	{
		FirBlk_4_16(Input_Sig_3, Filter_result_3, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_3[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}
	else if(Fir_count == 4)
	{
		FirBlk_4_16(Input_Sig_3, Filter_result_4, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_4[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}
	else if(Fir_count == 5)
	{
		FirBlk_4_16(Input_Sig_3, Filter_result_5, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_5[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}
	else if(Fir_count == 6)
	{
		FirBlk_4_16(Input_Sig_3, Filter_result_6, cptr_Coeff, &cptr_Coeff_Dly, nInput);
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly_inter_6[i]=Coeff_Dly_result[i];
		} //*Coeff_Dly has information which was calculated before task
	}

	Fir_count++;

	if(Fir_count == 6)
	{
		Fir_count = 0;
		for(int i =0; i <nCoeff;i++)
		{
			Coeff_Dly[i] = 0;
			Coeff_Dly_result[i] = 0;
			Coeff_Dly_inter_1[i] = 0;
			Coeff_Dly_inter_2[i] = 0;
			Coeff_Dly_inter_3[i] = 0;
			Coeff_Dly_inter_4[i] = 0;
			Coeff_Dly_inter_5[i] = 0;
			Coeff_Dly_inter_6[i] = 0;
		} //*Coeff_Dly has information which was calculated before task

	}
#endif /*End of FILTER_CALC_TEST*/

}
#endif /*End of BASIC_FILTER_TEST*/
