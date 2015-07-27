/*
 * knocheck_Config.h
 *
 *  Created on: 2015. 6. 15.
 *      Author: hansteve
 */

#ifndef KNOCHECK_CONFIG_H_
#define KNOCHECK_CONFIG_H_

	#define DMA_CHANNEL_SELECT 0			/*User configurable*/
	#define DSADC_CHANNEL_SELECT 0			/*User configurable*/


	#define MAX_CHANNEL 				3	/*User configurable*/
	#define KNOCHECK_DMA_PRIO 			57	/*User configurable*/
	#define BUFFER_SIZE  				128	/*User configurable*/
	#define PRE_BUFFER_SIZE  			64	/*User configurable*/
	#define KNK_KDF_ORDER				64	/*User configurable*/
	#define KNK_FILTERSET_SIZE			11	/*User configurable*/
	#define KNK_PARALLELFILTER_SIZE		3	/*User configurable*/

#if(FINAL==CONTROL_EN)
	#define A
#elif(FINAL==CONTROL_DIS)
	#define DATA_TEST_COUNT  50
	#define BUFFER_EXT_POSITION_1 	BUFFER_SIZE
	#define BUFFER_EXT_POSITION_2 	BUFFER_SIZE+1
	#define BUFFER_PLUS_TWO 		BUFFER_SIZE+2
	#define ONLY_ONE 				1
	#define ONLY_ONE_ARRAY 			0
#endif
/*End of #if(FINAL==CONTROL_EN)*/
	#define ATOM2_CH6 1
	#define ATOM2_CH5 2
	#define GTM_KNOCK_SELECT ATOM2_CH6		/*User configurable*/

	#define GTM_KNOCK_MODULE(MODULE,A1)				GTM_##MODULE##_##A1
	#define GTM_KNOCK_MODULE_CHANNEL(MODULE,A1,A2)	GTM_##MODULE##_##A1##_##A2
	#define CONCATE(A1,A2,A3)						A1##_##A2##A3
	#define CONCATE2(A1,A2)							A1##_##A2

	#if DSADC_CHANNEL_SELECT == 0
		#define SRC_DSADC_KNOCK 		SRC_DSADCSRM0
		#define CHANNEL_DSADC 			0
		#define	SELGTMTODSADC 			SEL0
		#define DSADC_MOVE_ENGINE_RUN	DSADC_GLOBRC.B.M0RUN
		#define DSADC_CHANNEL_RUN		DSADC_GLOBRC.B.CH0RUN
		#define DSADC_OFFM				DSADC_CH0_OFFM.B.OFFSET
		#define DSADC_RESM 				DSADC_RESM0
	#elif DSADC_CHANNEL_SELECT == 1
		#define SRC_DSADC_KNOCK SRC_DSADCSRM1
		#define CHANNEL_DSADC 1
		#define	SELGTMTODSADC SEL1
		#define DSADC_MOVE_ENGINE_RUN	DSADC_GLOBRC.B.M1RUN
		#define DSADC_CHANNEL_RUN		DSADC_GLOBRC.B.CH1RUN
		#define DSADC_OFFM				DSADC_CH1_OFFM.B.OFFSET
		#define DSADC_RESM 				DSADC_RESM1
	#elif DSADC_CHANNEL_SELECT == 2
		#define SRC_DSADC_KNOCK SRC_DSADCSRM2
		#define CHANNEL_DSADC 2
		#define	SELGTMTODSADC SEL2
		#define DSADC_MOVE_ENGINE_RUN	DSADC_GLOBRC.B.M2RUN
		#define DSADC_CHANNEL_RUN		DSADC_GLOBRC.B.CH2RUN
		#define DSADC_OFFM				DSADC_CH2_OFFM.B.OFFSET
		#define DSADC_RESM 				DSADC_RESM2
	#elif DSADC_CHANNEL_SELECT == 3
		#define SRC_DSADC_KNOCK SRC_DSADCSRM3
		#define CHANNEL_DSADC 3
		#define	SELGTMTODSADC SEL3
		#define DSADC_MOVE_ENGINE_RUN	DSADC_GLOBRC.B.M3RUN
		#define DSADC_CHANNEL_RUN		DSADC_GLOBRC.B.CH3RUN
		#define DSADC_OFFM				DSADC_CH3_OFFM.B.OFFSET
		#define DSADC_RESM 				DSADC_RESM3
	#elif DSADC_CHANNEL_SELECT == 4
		#define SRC_DSADC_KNOCK SRC_DSADCSRM4
		#define CHANNEL_DSADC 4
		#define	SELGTMTODSADC SEL4
		#define DSADC_MOVE_ENGINE_RUN	DSADC_GLOBRC.B.M4RUN
		#define DSADC_CHANNEL_RUN		DSADC_GLOBRC.B.CH4RUN
		#define DSADC_OFFM				DSADC_CH4_OFFM.B.OFFSET
		#define DSADC_RESM 				DSADC_RESM4
	#endif

	#if DMA_CHANNEL_SELECT == 0
		#define CHANNEL_DMA 0
		#define SRC_DMA_KNOCK SRC_DMA_DMA0_CH0
	#elif DMA_CHANNEL_SELECT == 1
		#define CHANNEL_DMA 1
		#define SRC_DMA_KNOCK SRC_DMA_DMA0_CH1
		#define SRC_DSADC_KNOCK SRC_DSADCSRM1
	#endif

	#if GTM_KNOCK_SELECT == ATOM2_CH5
		#define GTM_KNOCk_MODUL_CHANNEL_CTRL		GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,CTRL)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_MODE 	GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,IRQ_MODE)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_EN 		GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,IRQ_EN)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY 	GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,IRQ_NOTIFY)
		#define GTM_KNOCk_MODUL_AGC_GLB_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_GLB_CTRL)
		#define GTM_KNOCk_MODUL_AGC_ENDIS_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_ENDIS_CTRL)
		#define GTM_KNOCk_MODUL_AGC_OUTEN_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_OUTEN_CTRL)
		#define GTM_KNOCk_MODUL_AGC_FUPD_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_FUPD_CTRL)
		#define GTM_KNOCk_MODUL_AGC_ENDIS_STAT 		GTM_KNOCK_MODULE(ATOM2,AGC_ENDIS_STAT)
		#define UPEN_C 								CONCATE(UPEN,CTRL,5)
		#define ENDIS_C 							CONCATE(ENDIS,CTRL,5)
		#define OUTEN_C 							CONCATE(OUTEN,CTRL,5)
		#define FUPD_C 								CONCATE(FUPD,CTRL,5)
		#define ENDIS_S 							CONCATE(ENDIS,STAT,5)
		#define KNOCK_WINDOW_OPEN_SR                GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,SR0)
		#define KNOCK_WINDOW_CLOSE_SR               GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,SR1)
		#define KNOCK_WINDOW_OPEN_CM                GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,CM0)
		#define KNOCK_WINDOW_CLOSE_CM               GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH5,CM1)
		#define SRC_KNOCK_WINDOW 					SRC_GTMATOM22
		#define KNOCK_WINDOW_PRIO					52
	#elif GTM_KNOCK_SELECT == ATOM2_CH6
		#define GTM_KNOCk_MODUL_CHANNEL_CTRL		GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,CTRL)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_MODE 	GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,IRQ_MODE)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_EN 		GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,IRQ_EN)
		#define GTM_KNOCk_MODUL_CHANNEL_IRQ_NOTIFY 	GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,IRQ_NOTIFY)
		#define GTM_KNOCk_MODUL_AGC_GLB_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_GLB_CTRL)
		#define GTM_KNOCk_MODUL_AGC_ENDIS_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_ENDIS_CTRL)
		#define GTM_KNOCk_MODUL_AGC_OUTEN_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_OUTEN_CTRL)
		#define GTM_KNOCk_MODUL_AGC_FUPD_CTRL 		GTM_KNOCK_MODULE(ATOM2,AGC_FUPD_CTRL)
		#define GTM_KNOCk_MODUL_AGC_ENDIS_STAT 		GTM_KNOCK_MODULE(ATOM2,AGC_ENDIS_STAT)
		#define UPEN_C 								CONCATE(UPEN,CTRL,6)
		#define ENDIS_C 							CONCATE(ENDIS,CTRL,6)
		#define OUTEN_C 							CONCATE(OUTEN,CTRL,6)
		#define FUPD_C 								CONCATE(FUPD,CTRL,6)
		#define ENDIS_S 							CONCATE(ENDIS,STAT,6)
		#define KNOCK_WINDOW_OPEN_SR                GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,SR0)
		#define KNOCK_WINDOW_CLOSE_SR               GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,SR1)
		#define KNOCK_WINDOW_OPEN_CM                GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,CM0)
		#define KNOCK_WINDOW_CLOSE_CM               GTM_KNOCK_MODULE_CHANNEL(ATOM2,CH6,CM1)
		#define SRC_KNOCK_WINDOW 					SRC_GTMATOM23
		#define KNOCK_WINDOW_PRIO					52
		#define KNOCK_CHNNEL						6
	#endif

#endif /* KNOCHECK_CONFIG_H_ */
