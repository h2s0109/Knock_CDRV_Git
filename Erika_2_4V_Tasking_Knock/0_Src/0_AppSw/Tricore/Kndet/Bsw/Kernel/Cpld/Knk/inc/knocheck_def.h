/*
 * knocheck_def.h
 *
 *  Created on: 2015. 6. 15.
 *      Author: hansteve
 */

#ifndef KNOCHECK_DEF_H_
#define KNOCHECK_DEF_H_

	typedef enum
	{
		DMA_ISR_FLAG       	= 0,
		WINDOW_ISR_FLAG 	= 1
	} KNOCHECK_ISR_FLAG;

	typedef enum
	{
		BUFFER0       	= 0,
		BUFFER1 		= 1
	} BUFFER_FLAG;

#if(FINAL==CONTROL_DIS)
	typedef enum
	{
		BUFFER0_DMA 		= 1111,
		BUFFER1_DMA 		= 2222,
		BUFFER0_KNWINDOW 	= 3333,
		BUFFER1_KNWINDOW 	= 4444,
	} BUFFER_ISR_FLAG;
#endif

	typedef enum
	{
		NORMAL       		= 0,
		FILTER_INDICES_OVER = 1,
		FILTERSET_OVER 		= 2,
		CHANNEL_OVER 		= 4
	} FILTER_SEL_EXP;

	typedef struct
	{
		uint8 	Endis;
		uint16	StartAngle;
		uint16 	EndAngle;
	} KNOCHECK_CONVAR_T;


	typedef struct
	{
		void  *base;           /**< \brief buffer base address */
		uint16 index;                  /**< \brief buffer current index */
		uint16 length;                 /**< \brief buffer length*/
	} KNOCHECK_CIRBUFF_T;
#define  KNOCHECK_CONVAR KNOCHECK_CONVAR_T
#define  KNOCHECK_CIRBUFF KNOCHECK_CIRBUFF_T
#endif /* KNOCHECK_DEF_H_ */
