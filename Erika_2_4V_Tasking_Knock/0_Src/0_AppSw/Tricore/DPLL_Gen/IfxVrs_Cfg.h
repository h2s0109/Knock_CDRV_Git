/*
 * IfxVrs_Cfg.h
 *
 *  Created on: Jun 27, 2013
 *      Author: baratta
 */

#ifndef IFXVRS_CFG_H_
#define IFXVRS_CFG_H_

/** System clock frequency 10MHz*/
#define SYS_CLK 10 //Max GTM Clock ns
/** System clock period*/
#define SYS_CLK_NS (1000/SYS_CLK)
#define IFX_VRS_TOOTH_STEPS    (273) /** !<micro ticks generated for each tooth (up to 1024) */

//#define PROF_36_1 //Libralato
//#define PROF_60_6
#define PROF_60_2


#define IFX_VRS_NTEETH          (60) /** !<number of teeth HALF_SCALE (360 degrees) */
#define IFX_VRS_GAPTEETH        (2)  /** !<number of missing teeth HALF_SCALE (360 degrees) */
#define IFX_VRS_NCYCLES         (2)  /** !<number of cycles */
#define IFX_VRS_TOOTH_ANGLE (( 7200 / IFX_VRS_NCYCLES) / IFX_VRS_NTEETH) //0.1deg Resolution


/*Plausibility check - The slope is suppressed, if it appears before PVT time.*/
#define IFX_VRS_PVT_CHECK_FIRST_TOOTH  (410) //if DPLL_CTRL1.PIT==1 => (IFX_VRS_PVT_CHECK_FIRST_TOOTH*DT_T)/1024
#define IFX_VRS_PVT_CHECK_NORMAL_TEETH (512)

#define IFX_VRS_TS_T_CHECK_FIRST_TOOTH ((uint32) (0x0B00))  //i.e 1.5 Timeout=(IFX_VRS_TS_T_CHECK_FIRST_TEETH * DT_T)/1024
#define IFX_VRS_TS_T_CHECK_GAP    ((IFX_VRS_GAPTEETH+0.7)*1024)       //i.e.1.5 Timeout=(IFX_VRS_TS_T_CHECK_GAP * DT_T)/1024
#define IFX_VRS_TS_T_CHECK_NORMAL_TEETH (0x0599) //i.e.1.5 Timeout=(IFX_VRS_TS_T_CHECK_NORMAL_TEETH * DT_T)/1024

#define VRS_DPLL_TNU ((uint32)(IFX_VRS_NTEETH - 1)) /*! Number of Crankshaft (TRIGGER) Teeth HALF_SCALE*/


#define IFX_PHS_EDGE_NUM (2)
#define IFX_PHS_FIRST_ANGLE (44) /* 360.0 degrees */

#define IFX_TOT_VIRTUAL_TEETH_FULL_SCALE (IFX_VRS_NTEETH*IFX_VRS_NCYCLES)
#define NUM_SAMPLE_IDN_CLC (IFX_VRS_NTEETH)

#define IFX_VRS_LASTTOOTH (IFX_VRS_NTEETH-IFX_VRS_GAPTEETH)
#define IFX_VRS_REAL_TEETH_HALF_SCALE (IFX_VRS_LASTTOOTH)

#define IFX_VRS_LAST_TOOTH_MIN_1    (IFX_VRS_LASTTOOTH - 3)
#define IFX_VRS_LAST_TOOTH          (IFX_VRS_LASTTOOTH - 2)
#define IFX_VRS_FIRST_TOOTH         (IFX_VRS_LASTTOOTH - 1)


#define IFX_VRS_MAX_NTEETH (IFX_VRS_NCYCLES*IFX_VRS_TOOTH_STEPS)

#define VRS_FIRST_APT_2C_TOOTH_1st_ROT   (0)
#define VRS_FIRST_APT_2C_TOOTH_2nd_ROT   (IFX_VRS_LASTTOOTH)   //58 for 60-2
#define VRS_FIRST_TOOTH_IN_HOLE_1st_ROT  (IFX_VRS_LASTTOOTH)   //58 for 60-2
#define VRS_SECOND_TOOTH_IN_HOLE_1st_ROT (IFX_VRS_LASTTOOTH+1) //59 for 60-2
#define VRS_FIRST_TOOTH_IN_HOLE_2nd_ROT  ((IFX_TOT_VIRTUAL_TEETH_FULL_SCALE)-2) //118 for 60-2
#define VRS_SECOND_TOOTH_IN_HOLE_2nd_ROT ((IFX_TOT_VIRTUAL_TEETH_FULL_SCALE)-1) //120-1 for 60-2

#define IFX_VRS_PARAM_NOT_SET (0xFFFFFFFF)
#define VRS_MICROTICS_REV (IFX_VRS_NTEETH * IFX_VRS_TOOTH_STEPS)
#define VRS_MICROTICS_TOT ((VRS_MICROTICS_REV * IFX_VRS_NCYCLES))
#define VRS_DEGREE_TO_MICROTICS(a) (a * (VRS_MICROTICS_REV/3600))

/** PROFILE CONFIGURATION **/
#define IFX_VRS_NORMAL_TOOTH_LEN    (0x10000U)      /* CRANK Normal increment Value */

#define IFX_VRS_GAP_TOOTH_LEN   ((IFX_VRS_NORMAL_TOOTH_LEN * IFX_VRS_GAPTEETH) + IFX_VRS_NORMAL_TOOTH_LEN)/* CRANK Gap Teeth = 3 x Normal increment Value */

#define IFX_VRS_TE0I_ENABLE     (0x02000U)      /* CRANK Ratio3 - Normal increment Value */
#define IFX_VRS_TE1I_ENABLE     (0x04000U)      /* CRANK Ratio1 2 bis value*/
#define IFX_VRS_TE2I_ENABLE     (0x06000U)      /* CRANK Ratio1 2 - 3 Normal incremnt */
#define IFX_VRS_TE3I_ENABLE     (0x08000U)      /* TE3I enable */

#define IFX_VRS_ADT_SET_TE4I (0x0000A000U)               /* Enable TE4 Interrupt */

#define IFX_VRS_ADT_SET_TE3I   (0x8000U) /* Enable TE3 */
#define IFX_VRS_ADT_RESET_TE3I (0xF7FFFU) /* Disable TE3 */

#endif /* IFXVRS_CFG_H_ */

