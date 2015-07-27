/*
 * Test_switch.h
 *
 *  Created on: 2015. 4. 3.
 *      Author: hansteve
 */

#ifndef TEST_SWITCH_H_
#define TEST_SWITCH_H_

	#define KNOCHECK_USE
	#define NOT_USED
	#define KNOCHECK_TEST
	//#define KNOCHECK_DSADC_TEST
	#if defined(KNOCHECK_DSADC_TEST)
			#define DSADC_TEST_PRIOD 59
	#endif


	#ifdef KNOCHECK_TEST
			#define NOTUSE_ENDIS 	0
			#define WINDOW_ENDIS 	1
			#define	DMA_ENDIS		2
			#define	DSADC_ENDIS		3
			/*User configurable*/
			#define ENDIS_CONTROL DSADC_ENDIS
			#define CONTROL_DIS 0
			#define CONTROL_EN 1
			#define FINAL CONTROL_EN
	#endif /*End of KNOCHECK_TEST*/
#endif /* TEST_SWITCH_H_ */
