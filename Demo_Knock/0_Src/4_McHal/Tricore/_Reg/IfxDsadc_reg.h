/**
 * \file IfxDsadc_reg.h
 * \brief
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 * Version: TC27XC_UM_V1.5.R1
 * Specification: tc27xC_um_v1.5_SFR (Revision: UM_V1.5)
 * MAY BE CHANGED BY USER [yes/no]: No
 *
 *                                 IMPORTANT NOTICE
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Dsadc_Cfg Dsadc address
 * \ingroup IfxLld_Dsadc
 * 
 * \defgroup IfxLld_Dsadc_Cfg_BaseAddress Base address
 * \ingroup IfxLld_Dsadc_Cfg
 * 
 * \defgroup IfxLld_Dsadc_Cfg_Dsadc 2-DSADC
 * \ingroup IfxLld_Dsadc_Cfg
 * 
 */
#ifndef IFXDSADC_REG_H
#define IFXDSADC_REG_H 1
/******************************************************************************/
#include "IfxDsadc_regdef.h"
/******************************************************************************/
/** \addtogroup IfxLld_Dsadc_Cfg_BaseAddress
 * \{  */

/** \\brief  DSADC object */
#define MODULE_DSADC /*lint --e(923)*/ ((*(Ifx_DSADC*)0xF0024000u))
/** \}  */
/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxLld_Dsadc_Cfg_Dsadc
 * \{  */

/** \\brief  3C, Access Enable Register 0 */
#define DSADC_ACCEN0 /*lint --e(923)*/ (*(volatile Ifx_DSADC_ACCEN0z*)0xF002403Cu)

/** \\brief  90, Access Protection Register */
#define DSADC_ACCPROT /*lint --e(923)*/ (*(volatile Ifx_DSADC_ACCPROT*)0xF0024090u)

/** \\brief  A0, Carrier Generator Configuration Register */
#define DSADC_CGCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CGCFG*)0xF00240A0u)

/** \\brief  128, Boundary Select Register */
#define DSADC_CH0_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024128u)

/** Alias (User Manual Name) for DSADC_CH0_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH0_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL0	(DSADC_CH0_BOUNDSEL)

/** \\brief  1A0, Carrier Generator Synchronization Register */
#define DSADC_CH0_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00241A0u)

/** Alias (User Manual Name) for DSADC_CH0_CGSYNC.
* To use register names with standard convension, please use DSADC_CH0_CGSYNC.
*/
#define	DSADC_CGSYNC0	(DSADC_CH0_CGSYNC)

/** \\brief  108, Demodulator Input Configuration Register */
#define DSADC_CH0_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024108u)

/** Alias (User Manual Name) for DSADC_CH0_DICFG.
* To use register names with standard convension, please use DSADC_CH0_DICFG.
*/
#define	DSADC_DICFG0	(DSADC_CH0_DICFG)

/** \\brief  118, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH0_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024118u)

/** Alias (User Manual Name) for DSADC_CH0_FCFGA.
* To use register names with standard convension, please use DSADC_CH0_FCFGA.
*/
#define	DSADC_FCFGA0	(DSADC_CH0_FCFGA)

/** \\brief  114, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH0_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024114u)

/** Alias (User Manual Name) for DSADC_CH0_FCFGC.
* To use register names with standard convension, please use DSADC_CH0_FCFGC.
*/
#define	DSADC_FCFGC0	(DSADC_CH0_FCFGC)

/** \\brief  110, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH0_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024110u)

/** Alias (User Manual Name) for DSADC_CH0_FCFGM.
* To use register names with standard convension, please use DSADC_CH0_FCFGM.
*/
#define	DSADC_FCFGM0	(DSADC_CH0_FCFGM)

/** \\brief  120, Integration Window Control Register */
#define DSADC_CH0_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024120u)

/** Alias (User Manual Name) for DSADC_CH0_IWCTR.
* To use register names with standard convension, please use DSADC_CH0_IWCTR.
*/
#define	DSADC_IWCTR0	(DSADC_CH0_IWCTR)

/** \\brief  100, Modulator Configuration Register */
#define DSADC_CH0_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024100u)

/** Alias (User Manual Name) for DSADC_CH0_MODCFG.
* To use register names with standard convension, please use DSADC_CH0_MODCFG.
*/
#define	DSADC_MODCFG0	(DSADC_CH0_MODCFG)

/** \\brief  138, Offset Register Main Filter */
#define DSADC_CH0_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024138u)

/** Alias (User Manual Name) for DSADC_CH0_OFFM.
* To use register names with standard convension, please use DSADC_CH0_OFFM.
*/
#define	DSADC_OFFM0	(DSADC_CH0_OFFM)

/** \\brief  1A8, Rectification Configuration Register */
#define DSADC_CH0_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00241A8u)

/** Alias (User Manual Name) for DSADC_CH0_RECTCFG.
* To use register names with standard convension, please use DSADC_CH0_RECTCFG.
*/
#define	DSADC_RECTCFG0	(DSADC_CH0_RECTCFG)

/** \\brief  140, Result Register Auxiliary Filter */
#define DSADC_CH0_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024140u)

/** Alias (User Manual Name) for DSADC_CH0_RESA.
* To use register names with standard convension, please use DSADC_CH0_RESA.
*/
#define	DSADC_RESA0	(DSADC_CH0_RESA)

/** \\brief  130, Result Register Main Filter */
#define DSADC_CH0_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024130u)

/** Alias (User Manual Name) for DSADC_CH0_RESM.
* To use register names with standard convension, please use DSADC_CH0_RESM.
*/
#define	DSADC_RESM0	(DSADC_CH0_RESM)

/** \\brief  150, Time-Stamp Register */
#define DSADC_CH0_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024150u)

/** Alias (User Manual Name) for DSADC_CH0_TSTMP.
* To use register names with standard convension, please use DSADC_CH0_TSTMP.
*/
#define	DSADC_TSTMP0	(DSADC_CH0_TSTMP)

/** \\brief  228, Boundary Select Register */
#define DSADC_CH1_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024228u)

/** Alias (User Manual Name) for DSADC_CH1_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH1_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL1	(DSADC_CH1_BOUNDSEL)

/** \\brief  2A0, Carrier Generator Synchronization Register */
#define DSADC_CH1_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00242A0u)

/** Alias (User Manual Name) for DSADC_CH1_CGSYNC.
* To use register names with standard convension, please use DSADC_CH1_CGSYNC.
*/
#define	DSADC_CGSYNC1	(DSADC_CH1_CGSYNC)

/** \\brief  208, Demodulator Input Configuration Register */
#define DSADC_CH1_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024208u)

/** Alias (User Manual Name) for DSADC_CH1_DICFG.
* To use register names with standard convension, please use DSADC_CH1_DICFG.
*/
#define	DSADC_DICFG1	(DSADC_CH1_DICFG)

/** \\brief  218, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH1_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024218u)

/** Alias (User Manual Name) for DSADC_CH1_FCFGA.
* To use register names with standard convension, please use DSADC_CH1_FCFGA.
*/
#define	DSADC_FCFGA1	(DSADC_CH1_FCFGA)

/** \\brief  214, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH1_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024214u)

/** Alias (User Manual Name) for DSADC_CH1_FCFGC.
* To use register names with standard convension, please use DSADC_CH1_FCFGC.
*/
#define	DSADC_FCFGC1	(DSADC_CH1_FCFGC)

/** \\brief  210, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH1_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024210u)

/** Alias (User Manual Name) for DSADC_CH1_FCFGM.
* To use register names with standard convension, please use DSADC_CH1_FCFGM.
*/
#define	DSADC_FCFGM1	(DSADC_CH1_FCFGM)

/** \\brief  220, Integration Window Control Register */
#define DSADC_CH1_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024220u)

/** Alias (User Manual Name) for DSADC_CH1_IWCTR.
* To use register names with standard convension, please use DSADC_CH1_IWCTR.
*/
#define	DSADC_IWCTR1	(DSADC_CH1_IWCTR)

/** \\brief  200, Modulator Configuration Register */
#define DSADC_CH1_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024200u)

/** Alias (User Manual Name) for DSADC_CH1_MODCFG.
* To use register names with standard convension, please use DSADC_CH1_MODCFG.
*/
#define	DSADC_MODCFG1	(DSADC_CH1_MODCFG)

/** \\brief  238, Offset Register Main Filter */
#define DSADC_CH1_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024238u)

/** Alias (User Manual Name) for DSADC_CH1_OFFM.
* To use register names with standard convension, please use DSADC_CH1_OFFM.
*/
#define	DSADC_OFFM1	(DSADC_CH1_OFFM)

/** \\brief  2A8, Rectification Configuration Register */
#define DSADC_CH1_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00242A8u)

/** Alias (User Manual Name) for DSADC_CH1_RECTCFG.
* To use register names with standard convension, please use DSADC_CH1_RECTCFG.
*/
#define	DSADC_RECTCFG1	(DSADC_CH1_RECTCFG)

/** \\brief  240, Result Register Auxiliary Filter */
#define DSADC_CH1_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024240u)

/** Alias (User Manual Name) for DSADC_CH1_RESA.
* To use register names with standard convension, please use DSADC_CH1_RESA.
*/
#define	DSADC_RESA1	(DSADC_CH1_RESA)

/** \\brief  230, Result Register Main Filter */
#define DSADC_CH1_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024230u)

/** Alias (User Manual Name) for DSADC_CH1_RESM.
* To use register names with standard convension, please use DSADC_CH1_RESM.
*/
#define	DSADC_RESM1	(DSADC_CH1_RESM)

/** \\brief  250, Time-Stamp Register */
#define DSADC_CH1_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024250u)

/** Alias (User Manual Name) for DSADC_CH1_TSTMP.
* To use register names with standard convension, please use DSADC_CH1_TSTMP.
*/
#define	DSADC_TSTMP1	(DSADC_CH1_TSTMP)

/** \\brief  328, Boundary Select Register */
#define DSADC_CH2_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024328u)

/** Alias (User Manual Name) for DSADC_CH2_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH2_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL2	(DSADC_CH2_BOUNDSEL)

/** \\brief  3A0, Carrier Generator Synchronization Register */
#define DSADC_CH2_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00243A0u)

/** Alias (User Manual Name) for DSADC_CH2_CGSYNC.
* To use register names with standard convension, please use DSADC_CH2_CGSYNC.
*/
#define	DSADC_CGSYNC2	(DSADC_CH2_CGSYNC)

/** \\brief  308, Demodulator Input Configuration Register */
#define DSADC_CH2_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024308u)

/** Alias (User Manual Name) for DSADC_CH2_DICFG.
* To use register names with standard convension, please use DSADC_CH2_DICFG.
*/
#define	DSADC_DICFG2	(DSADC_CH2_DICFG)

/** \\brief  318, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH2_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024318u)

/** Alias (User Manual Name) for DSADC_CH2_FCFGA.
* To use register names with standard convension, please use DSADC_CH2_FCFGA.
*/
#define	DSADC_FCFGA2	(DSADC_CH2_FCFGA)

/** \\brief  314, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH2_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024314u)

/** Alias (User Manual Name) for DSADC_CH2_FCFGC.
* To use register names with standard convension, please use DSADC_CH2_FCFGC.
*/
#define	DSADC_FCFGC2	(DSADC_CH2_FCFGC)

/** \\brief  310, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH2_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024310u)

/** Alias (User Manual Name) for DSADC_CH2_FCFGM.
* To use register names with standard convension, please use DSADC_CH2_FCFGM.
*/
#define	DSADC_FCFGM2	(DSADC_CH2_FCFGM)

/** \\brief  320, Integration Window Control Register */
#define DSADC_CH2_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024320u)

/** Alias (User Manual Name) for DSADC_CH2_IWCTR.
* To use register names with standard convension, please use DSADC_CH2_IWCTR.
*/
#define	DSADC_IWCTR2	(DSADC_CH2_IWCTR)

/** \\brief  300, Modulator Configuration Register */
#define DSADC_CH2_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024300u)

/** Alias (User Manual Name) for DSADC_CH2_MODCFG.
* To use register names with standard convension, please use DSADC_CH2_MODCFG.
*/
#define	DSADC_MODCFG2	(DSADC_CH2_MODCFG)

/** \\brief  338, Offset Register Main Filter */
#define DSADC_CH2_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024338u)

/** Alias (User Manual Name) for DSADC_CH2_OFFM.
* To use register names with standard convension, please use DSADC_CH2_OFFM.
*/
#define	DSADC_OFFM2	(DSADC_CH2_OFFM)

/** \\brief  3A8, Rectification Configuration Register */
#define DSADC_CH2_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00243A8u)

/** Alias (User Manual Name) for DSADC_CH2_RECTCFG.
* To use register names with standard convension, please use DSADC_CH2_RECTCFG.
*/
#define	DSADC_RECTCFG2	(DSADC_CH2_RECTCFG)

/** \\brief  340, Result Register Auxiliary Filter */
#define DSADC_CH2_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024340u)

/** Alias (User Manual Name) for DSADC_CH2_RESA.
* To use register names with standard convension, please use DSADC_CH2_RESA.
*/
#define	DSADC_RESA2	(DSADC_CH2_RESA)

/** \\brief  330, Result Register Main Filter */
#define DSADC_CH2_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024330u)

/** Alias (User Manual Name) for DSADC_CH2_RESM.
* To use register names with standard convension, please use DSADC_CH2_RESM.
*/
#define	DSADC_RESM2	(DSADC_CH2_RESM)

/** \\brief  350, Time-Stamp Register */
#define DSADC_CH2_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024350u)

/** Alias (User Manual Name) for DSADC_CH2_TSTMP.
* To use register names with standard convension, please use DSADC_CH2_TSTMP.
*/
#define	DSADC_TSTMP2	(DSADC_CH2_TSTMP)

/** \\brief  428, Boundary Select Register */
#define DSADC_CH3_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024428u)

/** Alias (User Manual Name) for DSADC_CH3_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH3_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL3	(DSADC_CH3_BOUNDSEL)

/** \\brief  4A0, Carrier Generator Synchronization Register */
#define DSADC_CH3_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00244A0u)

/** Alias (User Manual Name) for DSADC_CH3_CGSYNC.
* To use register names with standard convension, please use DSADC_CH3_CGSYNC.
*/
#define	DSADC_CGSYNC3	(DSADC_CH3_CGSYNC)

/** \\brief  408, Demodulator Input Configuration Register */
#define DSADC_CH3_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024408u)

/** Alias (User Manual Name) for DSADC_CH3_DICFG.
* To use register names with standard convension, please use DSADC_CH3_DICFG.
*/
#define	DSADC_DICFG3	(DSADC_CH3_DICFG)

/** \\brief  418, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH3_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024418u)

/** Alias (User Manual Name) for DSADC_CH3_FCFGA.
* To use register names with standard convension, please use DSADC_CH3_FCFGA.
*/
#define	DSADC_FCFGA3	(DSADC_CH3_FCFGA)

/** \\brief  414, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH3_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024414u)

/** Alias (User Manual Name) for DSADC_CH3_FCFGC.
* To use register names with standard convension, please use DSADC_CH3_FCFGC.
*/
#define	DSADC_FCFGC3	(DSADC_CH3_FCFGC)

/** \\brief  410, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH3_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024410u)

/** Alias (User Manual Name) for DSADC_CH3_FCFGM.
* To use register names with standard convension, please use DSADC_CH3_FCFGM.
*/
#define	DSADC_FCFGM3	(DSADC_CH3_FCFGM)

/** \\brief  420, Integration Window Control Register */
#define DSADC_CH3_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024420u)

/** Alias (User Manual Name) for DSADC_CH3_IWCTR.
* To use register names with standard convension, please use DSADC_CH3_IWCTR.
*/
#define	DSADC_IWCTR3	(DSADC_CH3_IWCTR)

/** \\brief  400, Modulator Configuration Register */
#define DSADC_CH3_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024400u)

/** Alias (User Manual Name) for DSADC_CH3_MODCFG.
* To use register names with standard convension, please use DSADC_CH3_MODCFG.
*/
#define	DSADC_MODCFG3	(DSADC_CH3_MODCFG)

/** \\brief  438, Offset Register Main Filter */
#define DSADC_CH3_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024438u)

/** Alias (User Manual Name) for DSADC_CH3_OFFM.
* To use register names with standard convension, please use DSADC_CH3_OFFM.
*/
#define	DSADC_OFFM3	(DSADC_CH3_OFFM)

/** \\brief  4A8, Rectification Configuration Register */
#define DSADC_CH3_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00244A8u)

/** Alias (User Manual Name) for DSADC_CH3_RECTCFG.
* To use register names with standard convension, please use DSADC_CH3_RECTCFG.
*/
#define	DSADC_RECTCFG3	(DSADC_CH3_RECTCFG)

/** \\brief  440, Result Register Auxiliary Filter */
#define DSADC_CH3_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024440u)

/** Alias (User Manual Name) for DSADC_CH3_RESA.
* To use register names with standard convension, please use DSADC_CH3_RESA.
*/
#define	DSADC_RESA3	(DSADC_CH3_RESA)

/** \\brief  430, Result Register Main Filter */
#define DSADC_CH3_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024430u)

/** Alias (User Manual Name) for DSADC_CH3_RESM.
* To use register names with standard convension, please use DSADC_CH3_RESM.
*/
#define	DSADC_RESM3	(DSADC_CH3_RESM)

/** \\brief  450, Time-Stamp Register */
#define DSADC_CH3_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024450u)

/** Alias (User Manual Name) for DSADC_CH3_TSTMP.
* To use register names with standard convension, please use DSADC_CH3_TSTMP.
*/
#define	DSADC_TSTMP3	(DSADC_CH3_TSTMP)

/** \\brief  528, Boundary Select Register */
#define DSADC_CH4_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024528u)

/** Alias (User Manual Name) for DSADC_CH4_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH4_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL4	(DSADC_CH4_BOUNDSEL)

/** \\brief  5A0, Carrier Generator Synchronization Register */
#define DSADC_CH4_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00245A0u)

/** Alias (User Manual Name) for DSADC_CH4_CGSYNC.
* To use register names with standard convension, please use DSADC_CH4_CGSYNC.
*/
#define	DSADC_CGSYNC4	(DSADC_CH4_CGSYNC)

/** \\brief  508, Demodulator Input Configuration Register */
#define DSADC_CH4_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024508u)

/** Alias (User Manual Name) for DSADC_CH4_DICFG.
* To use register names with standard convension, please use DSADC_CH4_DICFG.
*/
#define	DSADC_DICFG4	(DSADC_CH4_DICFG)

/** \\brief  518, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH4_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024518u)

/** Alias (User Manual Name) for DSADC_CH4_FCFGA.
* To use register names with standard convension, please use DSADC_CH4_FCFGA.
*/
#define	DSADC_FCFGA4	(DSADC_CH4_FCFGA)

/** \\brief  514, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH4_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024514u)

/** Alias (User Manual Name) for DSADC_CH4_FCFGC.
* To use register names with standard convension, please use DSADC_CH4_FCFGC.
*/
#define	DSADC_FCFGC4	(DSADC_CH4_FCFGC)

/** \\brief  510, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH4_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024510u)

/** Alias (User Manual Name) for DSADC_CH4_FCFGM.
* To use register names with standard convension, please use DSADC_CH4_FCFGM.
*/
#define	DSADC_FCFGM4	(DSADC_CH4_FCFGM)

/** \\brief  520, Integration Window Control Register */
#define DSADC_CH4_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024520u)

/** Alias (User Manual Name) for DSADC_CH4_IWCTR.
* To use register names with standard convension, please use DSADC_CH4_IWCTR.
*/
#define	DSADC_IWCTR4	(DSADC_CH4_IWCTR)

/** \\brief  500, Modulator Configuration Register */
#define DSADC_CH4_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024500u)

/** Alias (User Manual Name) for DSADC_CH4_MODCFG.
* To use register names with standard convension, please use DSADC_CH4_MODCFG.
*/
#define	DSADC_MODCFG4	(DSADC_CH4_MODCFG)

/** \\brief  538, Offset Register Main Filter */
#define DSADC_CH4_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024538u)

/** Alias (User Manual Name) for DSADC_CH4_OFFM.
* To use register names with standard convension, please use DSADC_CH4_OFFM.
*/
#define	DSADC_OFFM4	(DSADC_CH4_OFFM)

/** \\brief  5A8, Rectification Configuration Register */
#define DSADC_CH4_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00245A8u)

/** Alias (User Manual Name) for DSADC_CH4_RECTCFG.
* To use register names with standard convension, please use DSADC_CH4_RECTCFG.
*/
#define	DSADC_RECTCFG4	(DSADC_CH4_RECTCFG)

/** \\brief  540, Result Register Auxiliary Filter */
#define DSADC_CH4_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024540u)

/** Alias (User Manual Name) for DSADC_CH4_RESA.
* To use register names with standard convension, please use DSADC_CH4_RESA.
*/
#define	DSADC_RESA4	(DSADC_CH4_RESA)

/** \\brief  530, Result Register Main Filter */
#define DSADC_CH4_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024530u)

/** Alias (User Manual Name) for DSADC_CH4_RESM.
* To use register names with standard convension, please use DSADC_CH4_RESM.
*/
#define	DSADC_RESM4	(DSADC_CH4_RESM)

/** \\brief  550, Time-Stamp Register */
#define DSADC_CH4_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024550u)

/** Alias (User Manual Name) for DSADC_CH4_TSTMP.
* To use register names with standard convension, please use DSADC_CH4_TSTMP.
*/
#define	DSADC_TSTMP4	(DSADC_CH4_TSTMP)

/** \\brief  628, Boundary Select Register */
#define DSADC_CH5_BOUNDSEL /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_BOUNDSEL*)0xF0024628u)

/** Alias (User Manual Name) for DSADC_CH5_BOUNDSEL.
* To use register names with standard convension, please use DSADC_CH5_BOUNDSEL.
*/
#define	DSADC_BOUNDSEL5	(DSADC_CH5_BOUNDSEL)

/** \\brief  6A0, Carrier Generator Synchronization Register */
#define DSADC_CH5_CGSYNC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_CGSYNC*)0xF00246A0u)

/** Alias (User Manual Name) for DSADC_CH5_CGSYNC.
* To use register names with standard convension, please use DSADC_CH5_CGSYNC.
*/
#define	DSADC_CGSYNC5	(DSADC_CH5_CGSYNC)

/** \\brief  608, Demodulator Input Configuration Register */
#define DSADC_CH5_DICFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_DICFG*)0xF0024608u)

/** Alias (User Manual Name) for DSADC_CH5_DICFG.
* To use register names with standard convension, please use DSADC_CH5_DICFG.
*/
#define	DSADC_DICFG5	(DSADC_CH5_DICFG)

/** \\brief  618, Filter Configuration Register, Auxiliary Filter */
#define DSADC_CH5_FCFGA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGA*)0xF0024618u)

/** Alias (User Manual Name) for DSADC_CH5_FCFGA.
* To use register names with standard convension, please use DSADC_CH5_FCFGA.
*/
#define	DSADC_FCFGA5	(DSADC_CH5_FCFGA)

/** \\brief  614, Filter Configuration Register, Main CIC Filter */
#define DSADC_CH5_FCFGC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGC*)0xF0024614u)

/** Alias (User Manual Name) for DSADC_CH5_FCFGC.
* To use register names with standard convension, please use DSADC_CH5_FCFGC.
*/
#define	DSADC_FCFGC5	(DSADC_CH5_FCFGC)

/** \\brief  610, Filter Configuration Register, Main Filter Chain */
#define DSADC_CH5_FCFGM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_FCFGM*)0xF0024610u)

/** Alias (User Manual Name) for DSADC_CH5_FCFGM.
* To use register names with standard convension, please use DSADC_CH5_FCFGM.
*/
#define	DSADC_FCFGM5	(DSADC_CH5_FCFGM)

/** \\brief  620, Integration Window Control Register */
#define DSADC_CH5_IWCTR /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_IWCTR*)0xF0024620u)

/** Alias (User Manual Name) for DSADC_CH5_IWCTR.
* To use register names with standard convension, please use DSADC_CH5_IWCTR.
*/
#define	DSADC_IWCTR5	(DSADC_CH5_IWCTR)

/** \\brief  600, Modulator Configuration Register */
#define DSADC_CH5_MODCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_MODCFG*)0xF0024600u)

/** Alias (User Manual Name) for DSADC_CH5_MODCFG.
* To use register names with standard convension, please use DSADC_CH5_MODCFG.
*/
#define	DSADC_MODCFG5	(DSADC_CH5_MODCFG)

/** \\brief  638, Offset Register Main Filter */
#define DSADC_CH5_OFFM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_OFFM*)0xF0024638u)

/** Alias (User Manual Name) for DSADC_CH5_OFFM.
* To use register names with standard convension, please use DSADC_CH5_OFFM.
*/
#define	DSADC_OFFM5	(DSADC_CH5_OFFM)

/** \\brief  6A8, Rectification Configuration Register */
#define DSADC_CH5_RECTCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RECTCFG*)0xF00246A8u)

/** Alias (User Manual Name) for DSADC_CH5_RECTCFG.
* To use register names with standard convension, please use DSADC_CH5_RECTCFG.
*/
#define	DSADC_RECTCFG5	(DSADC_CH5_RECTCFG)

/** \\brief  640, Result Register Auxiliary Filter */
#define DSADC_CH5_RESA /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESA*)0xF0024640u)

/** Alias (User Manual Name) for DSADC_CH5_RESA.
* To use register names with standard convension, please use DSADC_CH5_RESA.
*/
#define	DSADC_RESA5	(DSADC_CH5_RESA)

/** \\brief  630, Result Register Main Filter */
#define DSADC_CH5_RESM /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_RESM*)0xF0024630u)

/** Alias (User Manual Name) for DSADC_CH5_RESM.
* To use register names with standard convension, please use DSADC_CH5_RESM.
*/
#define	DSADC_RESM5	(DSADC_CH5_RESM)

/** \\brief  650, Time-Stamp Register */
#define DSADC_CH5_TSTMP /*lint --e(923)*/ (*(volatile Ifx_DSADC_CH_TSTMP*)0xF0024650u)

/** Alias (User Manual Name) for DSADC_CH5_TSTMP.
* To use register names with standard convension, please use DSADC_CH5_TSTMP.
*/
#define	DSADC_TSTMP5	(DSADC_CH5_TSTMP)

/** \\brief  0, Clock Control Register */
#define DSADC_CLC /*lint --e(923)*/ (*(volatile Ifx_DSADC_CLC*)0xF0024000u)

/** \\brief  E0, Event Flag Register */
#define DSADC_EVFLAG /*lint --e(923)*/ (*(volatile Ifx_DSADC_EVFLAG*)0xF00240E0u)

/** \\brief  E4, Event Flag Clear Register */
#define DSADC_EVFLAGCLR /*lint --e(923)*/ (*(volatile Ifx_DSADC_EVFLAGCLR*)0xF00240E4u)

/** \\brief  80, Global Configuration Register */
#define DSADC_GLOBCFG /*lint --e(923)*/ (*(volatile Ifx_DSADC_GLOBCFG*)0xF0024080u)

/** \\brief  88, Global Run Control Register */
#define DSADC_GLOBRC /*lint --e(923)*/ (*(volatile Ifx_DSADC_GLOBRC*)0xF0024088u)

/** \\brief  B0, Common Mode Hold Voltage Register 0 */
#define DSADC_GLOBVCMH0 /*lint --e(923)*/ (*(volatile Ifx_DSADC_GLOBVCMH0*)0xF00240B0u)

/** \\brief  B4, Common Mode Hold Voltage Register 1 */
#define DSADC_GLOBVCMH1 /*lint --e(923)*/ (*(volatile Ifx_DSADC_GLOBVCMH1*)0xF00240B4u)

/** \\brief  B8, Common Mode Hold Voltage Register 2 */
#define DSADC_GLOBVCMH2 /*lint --e(923)*/ (*(volatile Ifx_DSADC_GLOBVCMH2*)0xF00240B8u)

/** \\brief  8, Module Identification Register */
#define DSADC_ID /*lint --e(923)*/ (*(volatile Ifx_DSADC_ID*)0xF0024008u)

/** \\brief  34, Kernel Reset Register 0 */
#define DSADC_KRST0 /*lint --e(923)*/ (*(volatile Ifx_DSADC_KRST0*)0xF0024034u)

/** \\brief  30, Kernel Reset Register 1 */
#define DSADC_KRST1 /*lint --e(923)*/ (*(volatile Ifx_DSADC_KRST1*)0xF0024030u)

/** \\brief  2C, Kernel Reset Status Clear Register */
#define DSADC_KRSTCLR /*lint --e(923)*/ (*(volatile Ifx_DSADC_KRSTCLR*)0xF002402Cu)

/** \\brief  28, OCDS Control and Status Register */
#define DSADC_OCS /*lint --e(923)*/ (*(volatile Ifx_DSADC_OCS*)0xF0024028u)
/** \}  */
/******************************************************************************/
/******************************************************************************/
#endif /* IFXDSADC_REG_H */
