/*
 * knocheck_GloVar.h
 *
 *  Created on: 2015. 6. 15.
 *      Author: hansteve
 */

#include "Ifx_Types.h"			/* will be changed to HMC header*/
#include "knocheck_ATest_Switch.h"
#include "knocheck_def.h"
#include "knocheck_config.h"
#ifdef KNOCHECK_TEST
	__align (4)
	volatile sint16 TestCntsource[MAX_CHANNEL];
	uint8 	FilterIndices[KNK_PARALLELFILTER_SIZE] ={0,1,2};
#endif
	/*End of KNOCHECK_TEST*/

#ifdef KNOCHECK_TEST
	uint8 	SwitchWin;
	uint8 	SwithEnDis;
	uint8 	Enable_control;
	uint8 	Errortrace;
	sint16 	cntDATASET;
#endif
	/*End of KNOCHECK_TEST*/

__align (2*BUFFER_SIZE)
volatile sint16 Buffer0_Dest[BUFFER_SIZE];
__align (2*BUFFER_SIZE)
volatile sint16 Buffer1_Dest[BUFFER_SIZE];

#pragma section neardata
uint8 KnoCheck_NumBuffer;
uint8 KnoCheck_IgnDataCnt;
uint8 Knocheck_Ready_Chan;
uint8 KnoCheck_RunChan;
uint8 KnoCheck_RunFiltNum;
uint16 KnoCheck_FirNumInput;
uint8 KnoCheck_Gain[MAX_CHANNEL];
KNOCHECK_ISR_FLAG KnoCheck_IsrDistFlg;
KNOCHECK_CONVAR KnoCheck_WinCon[MAX_CHANNEL];
KNOCHECK_CIRBUFF Knocheck_FiltInfo[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
#pragma section neardata restore

#if(FINAL==CONTROL_EN)
	#pragma section neardata
	#pragma align 8
	sint16 Knocheck_DestCopyRam[BUFFER_SIZE];
	#pragma align 8
	sint16 Knocheck_FilterResult[KNK_PARALLELFILTER_SIZE][BUFFER_SIZE];
	uint16 Knocheck_Integrated[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	uint16 Knocheck_Integr[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	uint16 Knocheck_NumIputData;
	#pragma section neardata restore
#elif(FINAL==CONTROL_DIS)
	uint32 cntDATA;
	uint16 Remain_Data_Num[DATA_TEST_COUNT]={0,};
	uint16 Knocheck_NumIputData[DATA_TEST_COUNT]={0,};
	uint16 Knocheck_Integrated[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	uint16 Knocheck_Integr[MAX_CHANNEL][KNK_PARALLELFILTER_SIZE];
	uint16 temp_Data_Num;
	uint16 Knocheck_NumIputData_last;
	/* Input: Data in 1Q15 */
	#pragma section neardata
	#pragma align 8
	sint16 Knocheck_DestCopyRam[DATA_TEST_COUNT][BUFFER_PLUS_TWO];
	#pragma section neardata restore
	#pragma align 8
	sint16 Knocheck_FilterResult[DATA_TEST_COUNT][KNK_PARALLELFILTER_SIZE][BUFFER_PLUS_TWO];
#endif
#pragma section neardata
/* Input: Data in 1Q15 */
#pragma align 8
#if(FINAL==CONTROL_DIS)
sint16   KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] =
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
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000
			},
			{
				0x8000,0x8000,0x0000,0x0000,
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
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000
			},
			{
				0x8000,0x8000,0x8000,0x0000,
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
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000,
				0x0000,0x0000,0x0000,0x0000
			}

	};
#elif(FINAL==CONTROL_EN)
	sint16 KNK_DETECTION_FILTER_SET[KNK_FILTERSET_SIZE][KNK_KDF_ORDER] = {
			// Filter1: 8 kHz
					{ 83, 63, 65526, 65421, 65299, 65202, 65168, 65229, 65389, 87,
							346, 565, 677, 629, 390, 65501, 64941, 64344, 63850,
							63615, 63786, 64460, 131, 1894, 4974, 4974, 1894, 131,
							64460, 63786, 63615, 63850, 64344, 64941, 65501, 390,
							629, 677, 565, 346, 87, 65389, 65229, 65168, 65202,
							65299, 65421, 65526, 63, 83, },
					// Filter2: 10 kHz
					{ 65451, 65476, 11, 91, 156, 159, 72, 65437, 65232, 65077,
							65059, 65226, 26, 435, 762, 840, 552, 65425, 64524,
							63660, 63194, 63469, 64710, 1460, 5601, 5601, 1460,
							64710, 63469, 63194, 63660, 64524, 65425, 552, 840,
							762, 435, 26, 65226, 65059, 65077, 65232, 65437, 72,
							159, 156, 91, 11, 65476, 65451, },
					// Filter3: 12 kHz
					{ 65513, 65519, 65454, 65396, 65406, 65488, 84, 208, 248, 142,
							65434, 65149, 64985, 65091, 65502, 536, 962, 902, 170,
							64446, 63182, 62676, 63624, 829, 6188, 6188, 829,
							63624, 62676, 63182, 64446, 170, 902, 962, 536, 65502,
							65091, 64985, 65149, 65434, 142, 248, 208, 84, 65488,
							65406, 65396, 65454, 65519, 65513, },
					// Filter4: 14 kHz
					{ 65495, 65526, 26, 30, 65524, 65443, 65373, 65381, 65503, 154,
							278, 206, 65444, 65074, 64915, 65203, 358, 1033, 1071,
							75, 63848, 62372, 62602, 7, 6652, 6652, 7, 62602,
							62372, 63848, 75, 1071, 1033, 358, 65203, 64915, 65074,
							65444, 206, 278, 154, 65503, 65381, 65373, 65443,
							65524, 30, 26, 65526, 65495, },
					// Filter5: 16 kHz
					{ 65497, 65489, 65465, 65468, 65535, 83, 99, 4, 65396, 65337,
							65463, 179, 331, 157, 65218, 64824, 65001, 311, 1203,
							1092, 64966, 62611, 61761, 64583, 7041, 7041, 64583,
							61761, 62611, 64966, 1092, 1203, 311, 65001, 64824,
							65218, 157, 331, 179, 65463, 65337, 65396, 4, 99, 83,
							65535, 65468, 65465, 65489, 65497, },
					// Filter6: 18 kz
					{ 65523, 27, 28, 26, 65488, 65418, 65484, 145, 278, 73, 65161,
							64991, 5, 828, 873, 65226, 63991, 64408, 1007, 2517,
							1170, 63351, 61900, 64831, 3723, 3723, 64831, 61900,
							63351, 1170, 2517, 1007, 64408, 63991, 65226, 873, 828,
							5, 64991, 65161, 73, 278, 145, 65484, 65418, 65488, 26,
							28, 27, 65523, },
					// Filter7: 20 kHz
					{ 65497, 31, 63, 22, 65476, 65478, 23, 39, 15, 73, 123, 65396,
							65047, 65361, 808, 1011, 65008, 63508, 64777, 2347,
							2796, 64402, 61227, 64020, 3915, 3915, 64020, 61227,
							64402, 2796, 2347, 64777, 63508, 65008, 1011, 808,
							65361, 65047, 65396, 123, 73, 15, 39, 23, 65478, 65476,
							22, 63, 31, 65497, },
					// Filter8: 7 kHz narrowband
					{ 65478, 65409, 65336, 65276, 65263, 65322, 65468, 156, 427,
							692, 885, 938, 799, 449, 65446, 64791, 64136, 63626,
							63407, 63590, 64221, 65257, 1012, 2348, 3335, 3335,
							2348, 1012, 65257, 64221, 63590, 63407, 63626, 64136,
							64791, 65446, 449, 799, 938, 885, 692, 427, 156, 65468,
							65322, 65263, 65276, 65336, 65409, 65478, },
					// Filter9: 8 kHz narrowband
					{ 45, 61, 30, 65480, 65352, 65217, 65124, 65132, 65289, 64,
							471, 869, 1128, 1133, 811, 172, 64860, 64001, 63369,
							63189, 63610, 64650, 602, 2240, 3471, 3471, 2240, 602,
							64650, 63610, 63189, 63369, 64001, 64860, 172, 811,
							1133, 1128, 869, 471, 64, 65289, 65132, 65124, 65217,
							65352, 65480, 30, 61, 45, },
					// Filter10: 10 kHz narrowband
					{ 65470, 65475, 65533, 82, 201, 265, 228, 28, 65263, 64981,
							64866, 65025, 65488, 578, 1164, 1394, 1056, 159, 64489,
							63422, 63031, 63626, 65193, 1707, 3337, 3337, 1707,
							65193, 63626, 63031, 63422, 64489, 159, 1056, 1394,
							1164, 578, 65488, 65025, 64866, 64981, 65263, 28, 228,
							265, 201, 82, 65533, 65475, 65470, },
					// Filter11 : Bypass
					{ 32767, 0, 0, 0, }

			};

#endif
/* Filter delayLine */
#pragma align 8
sint16  Coeff_Dly[KNK_PARALLELFILTER_SIZE][KNK_KDF_ORDER];
/* CircBuf for Filter Coefficients (Coeff) */
KNOCHECK_CIRBUFF cptr_Coeff= {KNK_DETECTION_FILTER_SET, 0, KNK_KDF_ORDER*2};
/* CircBuf for delayLine */
KNOCHECK_CIRBUFF cptr_Coeff_Dly_0 = {Coeff_Dly[0], 0, KNK_KDF_ORDER*2};
KNOCHECK_CIRBUFF cptr_Coeff_Dly_1 = {Coeff_Dly[1], 0, KNK_KDF_ORDER*2};
KNOCHECK_CIRBUFF cptr_Coeff_Dly_2 = {Coeff_Dly[2], 0, KNK_KDF_ORDER*2};
#pragma section neardata restore
