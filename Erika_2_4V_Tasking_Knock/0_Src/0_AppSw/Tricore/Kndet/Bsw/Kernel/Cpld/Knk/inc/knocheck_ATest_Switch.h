/*
 * Test_switch.h
 *
 *  Created on: 2015. 4. 3.
 *      Author: hansteve
 */

#ifndef TEST_SWITCH_H_
	#define TEST_SWITCH_H_
	#define NOT_USED

	#define KNOCHECK_TEST
	//#define KNOCHECK_DSADC_TEST
	#define KNDET_DEF_H

	#ifdef KNOCHECK_TEST
		#define KNOCHECK_DOUBLE_BUFFERING
		#define CHOOSE_TEST 8

		#if CHOOSE_TEST == 1
			#define DYNAMIC_DATA_FILTER_TEST
			/*
			#ifdef HMC_FILTER_TEST
			#endif End of HMC_FILTER_TEST
			*/

		#elif CHOOSE_TEST == 2
			#define BASIC_FILTER_TEST
			/*
			#ifdef BASIC_FILTER_TEST
			#endif End of BASIC_FILTER_TEST
			*/
			#define INPUT_BIGGER
			//#define FILTER_CALC_TEST

			#ifdef INPUT_BIGGER
			//#define FILTER_SIMPL_FOR_MAIL
			#endif

		#elif CHOOSE_TEST== 3
			#define SETCTRL_TEST
			/*
			#ifdef SETCTRL_TEST
			#endif End of SETCTRL_TEST
			*/
		#elif CHOOSE_TEST == 4
			/*Using Filter_select, can change filter value
			 * TOTAL_FILTER_TEST.cmm*/
			#define TOTAL_FILTER_TEST
			/*
			#ifdef TOTAL_FILTER_TEST
			#endif End of TOTAL_FILTER_TEST
			*/
			#define FILTER_NUM 16
		#elif (CHOOSE_TEST == 5 && defined(KNOCHECK_DOUBLE_BUFFERING))

			#define DOUBLE_BUFFRING_TEST

			/*
			#ifdef DOUBLE_BUFFRING_TEST
			#endif End of DOUBLE_BUFFRING_TEST
			*/
		#elif (CHOOSE_TEST == 6 && defined(KNOCHECK_DOUBLE_BUFFERING))

			#define SIMPLE_FILTER_DOUBLEBUF_TEST

			/*
			#ifdef SIMPLE_FILTER_DOUBLEBUF_TEST
			#endif End of SIMPLE_FILTER_DOUBLEBUF_TEST
			*/
		#elif(CHOOSE_TEST == 7 && defined(KNOCHECK_DOUBLE_BUFFERING))

			#define CHANGE_FILTER_DOUBLEBUF_TEST
			/*
 	 	 	#ifdef CHANGE_FILTER_DOUBLEBUF_TEST
			#endif End of CHANGE_FILTER_DOUBLEBUF_TEST
			*/
		#elif(CHOOSE_TEST == 8 && defined(KNOCHECK_DOUBLE_BUFFERING))

			#define CHANNEL_FILTER_SELECT_TEST
			//#define OLD_TYPE
			#define REAL_FILTER_NUM_TEST
			/*
 	 	 	#ifdef CHANGE_FILTER_DOUBLEBUF_TEST
			#endif End of CHANGE_FILTER_DOUBLEBUF_TEST
			*/
		#endif /*End of CHOOSE_TEST*/

	#endif /*End of KNOCHECK_TEST*/
#endif /* TEST_SWITCH_H_ */
