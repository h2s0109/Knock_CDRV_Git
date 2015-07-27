/*
 * knocheck_SETCTRL_TEST.c
 *
 *  Created on: 2015. 5. 26.
 *      Author: G122885
 */
#include "knocheck_priv.h"

#ifdef SETCTRL_TEST
KnoCirBuff Knocheck_filter[KNK_CH_LENGTH];
uint8 FilterIndices[KNK_PARALLELFILTER_SIZE] ={1,3,4,5,12};
uint8 filter_sel_switch = 0;
uint8 Errortrace;

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

				Knocheck_filter[Ch].base = &(KNK_DETECTION_FILTER_SET[Filter_sel]);
				Knocheck_filter[Ch].index = 0;
				Knocheck_filter[Ch].length = 20;
			}
	}
	else
	{
		Errortrace |= CHANNEL_OVER;
	}
}

void SetCtrl_selection(void)
{
	switch(filter_sel_switch)
	{
	case 0:
		/*Normal Case 0*/
		Knk_SetCtrlDataEx(0,0,FilterIndices,2,0);
		break;
	case 1:
		/*Channel Over range Case Channel 4 select */
		Knk_SetCtrlDataEx(4,0,FilterIndices,2,0);
		break;
	case 2:
		/*Filter Over range Case-Filter SET 12 select*/
		Knk_SetCtrlDataEx(1,0,FilterIndices,5,0);
		break;
	case 3:
		/*FilterIndices Over range Case*/
		Knk_SetCtrlDataEx(2,0,FilterIndices,6,0);
		break;
	case 4:
		/*Normal Case channel 1*/
		Knk_SetCtrlDataEx(1,0,FilterIndices,0,0);
		break;
	default:
		Knk_SetCtrlDataEx(0,0,FilterIndices,2,0);
		break;
	}
}
#endif /*End of SETCTRL_TEST*/

#if Knk_SetCtrlDataEx

void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,
		uint8 IndexLength, uint16 rl)
{
	if (Ch < KNK_CH_LENGTH) {
		Knk_ManParamT* m = Knk_GetManParam(Ch);
		if (m->En == KNK_EN_DISABLE) {
			Knk_RuntimeT* r = Knk_GetRuntime(Ch);
			_Knk_SetCtrlDataEx(r, Ch, InputGain, FilterIndices, IndexLength);
		}
	}
}

void _Knk_SetCtrlDataEx(Knk_RuntimeT* const r, uint8 Ch, uint8 InputGain,
		uint8* const FilterIndices, uint8 IndexLength)
{
	/*
	 * Polyspace is checked already
	 * Overflow : InputGain의 값은 ASW에서 최대 3으로 제한되어 있기 때문에 문제없음
	 * Shift operations : InputGain의 최대값은 3이기 때문에 문제없음
	 */
	uint8 KnkGain = 1 << InputGain;
	r->InputStageGain = (KnkGain <= 8) ? KnkGain : 8 ;

	Knk_SetFadcGain(Ch, r->InputStageGain);

	int len = (IndexLength < KNK_PARALLELFILTER_SIZE) ? IndexLength
			: KNK_PARALLELFILTER_SIZE;
	for (int i = 0; i < len; ++i) {
		uint8 fi = FilterIndices[i];

		if (fi >= KNK_FILTERSET_SIZE){ // last filter selected when index range over
			fi = KNK_FILTERSET_SIZE - 1;
		}
		Knk_FilterDspT* p = &r->Filters[i];
		Knk_PrepareFilterCoefficient(p,
				(Knk_DspDataT*) &(KNK_DETECTION_FILTER_SET[fi]));
	}
}

inline void Knk_PrepareFilterCoefficient(Knk_FilterDspT* const p,
		Knk_DspDataT* const FilterCoefficient)
{
	p->KDFBuffHBaseReady = FilterCoefficient;
}
#endif

