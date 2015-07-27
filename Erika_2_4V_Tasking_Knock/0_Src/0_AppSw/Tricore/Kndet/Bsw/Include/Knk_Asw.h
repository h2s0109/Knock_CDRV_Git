/**
 * @file	Knk_Asw.h
 * @brief 	Knock Detection Driver - Knock Detection Driver APIs
 * @ingroup DOC_KNK_API
 * @author 	PowerTrain Control System Team, Hyundai Motor Company
 * @version 1.0.0
 * @note
 * Changes:
 * 	- 2011-06-01 Sanghoon Kim	Initial Writing
 * 	- Last commit: $Id: Knk_Asw.h 9684 2014-06-27 04:24:05Z 6218111 $
 *
 * <hr>	The information in this file is the property of Hyundai-Kia Motors
 * and may not be copied, or communicated to a third party, or used,
 * for any purpose other than that for which it is supplied without the express
 * written consent of Hyundai-Kia Motors.
 * <center>&copy; COPYRIGHT 2010 Hyundai Motor Company</center>
 */
#include "Ifx_Types.h"                 /* Do not remove this include */
#include "knocheck_pub.h"

#if defined(FINAL)
	/**
	 * @name Knock Channels
	 */
	/// @{	@ingroup DOC_KNK_CH
	#define KNK_CH1						0		///< Knock Channel 1
	#define KNK_CH2						1		///< Knock Channel 2
	#define KNK_CH3						2		///< Knock Channel 3
	#define KNK_CH4						3		///< Knock Channel 4
	#define KNK_CHX						7		///< Reserved
	#define KNK_CH_LENGTH				4		///< Max channel
	/// @}
	#define KNK_EN_DISABLE				0		///< Disable
	#define KNK_EN_ENABLE				1		///< Enable

	/**
	 * @name Sensor Gain Selector
	 */
	/// @{	@ingroup DOC_KNK_GAIN
	#define KNK_GAIN1					1		///< Knock Gain 1
	#define KNK_GAIN2					2		///< Knock Gain 2
	#define KNK_GAIN4					4		///< Knock Gain 4
	#define KNK_GAIN8					8		///< Knock Gain 8
	/// @}
	extern void Knk_En(uint8 Ch, uint8 Enable);
	extern uint16 Knk_GetStat(uint8 Ch);
	extern void Knk_UpdateMeasureWindow(uint8 Ch, uint16 StartAngle, uint16 EndAngle);
	// Normal Operation
	extern void Knk_SetCtrlData(uint8 Ch, uint8 InputGain, uint8 FilterIndex);
	extern sint32 Knk_GetIntgData(uint8 Ch);
	// Extensional Operation
	extern void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,uint8 IndexLength, uint16 rl);
	extern void Knk_GetIntgDataEx(uint8 Ch, sint32* IntegratedValues,uint8 ValueLength);
	extern sint32 Knk_GetDetectionValue(uint8 Ch);
	// Diagnosis
	extern Knk_DiagDataT Knk_GetDiagData(uint8 Ch);

#endif /*End of */

#ifndef NOT_USED
#ifndef KNK_ASW_H_
#define KNK_ASW_H_

#include <Bsw/Include/Sys/Std_Types.h>

#include "Knk_Runtime.h"
/**
 * @name Knock Channels
 */
/// @{	@ingroup DOC_KNK_CH
#define KNK_CH1						0		///< Knock Channel 1
#define KNK_CH2						1		///< Knock Channel 2
#define KNK_CH3						2		///< Knock Channel 3
#define KNK_CH4						3		///< Knock Channel 4
#define KNK_CHX						7		///< Reserved
#define KNK_CH_LENGTH				4		///< Max channel
/// @}

/**
 * @name Knock States
 */
/// @{	@ingroup DOC_KNK_STAT
#define KNK_STAT_CH_DISABLE			0		///< KNK 채널 사용하지 않는 상태
#define KNK_STAT_CH_IDLE			1		///< Measure Window 설정, 필터 계수 설정
#define KNK_STAT_CH_DIAG_SIGA		2		///< Diagnosis of Signal A input
#define KNK_STAT_CH_DIAG_SIGB		3		///< Diagnosis of Signal B input
#define KNK_STAT_CH_READY			4		///< Measure Window 시작각 근방 도달, Filter 점유 상태
#define KNK_STAT_CH_ACTIVE			5		///< Knock Detection Filter 동작 상태
/// @}

/**
 * @name Enableness Selector
 */
/// @{	@ingroup DOC_KNK_EN
#define KNK_EN_DISABLE				0		///< Disable
#define KNK_EN_ENABLE				1		///< Enable

#define ADDMW_EN_DISABLE			0		///< Disable
#define ADDMW_EN_ENABLE				1		///< Enable

/// @}

/**
 * @name Sensor Gain Selector
 */
/// @{	@ingroup DOC_KNK_GAIN
#define KNK_GAIN1					1		///< Knock Gain 1
#define KNK_GAIN2					2		///< Knock Gain 2
#define KNK_GAIN4					4		///< Knock Gain 4
#define KNK_GAIN8					8		///< Knock Gain 8
/// @}


extern void Knk_En(uint8 Ch, uint8 Enable);
extern uint16 Knk_GetStat(uint8 Ch);
extern void Knk_UpdateMeasureWindow(uint8 Ch, uint16 StartAngle,
		uint16 EndAngle);
//Additional Measure Window API///////////////////////////////////////////////////////
//Pre-Ignition Information
extern void AddMW_En(uint8 Ch, uint8 Enable);
extern void AddMW_UpdateMeasureWindow(uint8 Ch, uint16 AddStartAngle,uint16 AddEndAngle);
extern void AddMW_GetIntgDataEx(uint8 Ch, sint32* IntegratedAddValues, uint8 AddValueLength);
extern boolean GetDiagAddMeasureWindow_Length(uint8 Ch);
////////////////////////////////////////////////////////////////////////////////////////
//Update Additional MW Value
extern sint32 AddMW_GetIntgData(uint8 Ch);
// Normal Operation
extern void Knk_SetCtrlData(uint8 Ch, uint8 InputGain, uint8 FilterIndex);
extern sint32 Knk_GetIntgData(uint8 Ch);

// Extensional Operation
extern void Knk_SetCtrlDataEx(uint8 Ch, uint8 InputGain, uint8* const FilterIndices,
		uint8 IndexLength, uint16 rl);
extern void Knk_GetIntgDataEx(uint8 Ch, sint32* IntegratedValues,
		uint8 ValueLength);
extern sint32 Knk_GetDetectionValue(uint8 Ch);

// Diagnosis
extern Knk_DiagDataT Knk_GetDiagData(uint8 Ch);

#endif /* KNK_ASW_H_ */

#endif
/*End of NOT_USED*/
