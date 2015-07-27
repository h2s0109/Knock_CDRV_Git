/*
 * \copyright Copyright (C) Infineon Technologies (2010)                                
 *                                                                           
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * IFX License for more details (IFX_License.txt).
 * 
 * This file may be used, copied, and distributed, with or without
 * modification, provided that all copyright notices are retained;
 * that all modifications to this file are prominently noted in the
 * modified file; and that this paragraph is not modified.
 *
 *****************************************************/

/********************************************************************************************!
 *  \file
 *  	IfxPhsVrs_common.h
 *  \date
 *  	24-Jan-2012 5:11:54 PM
 *  \author
 *  	Alfredo Baratta
 *  \brief
 *  	Basic Configuration Parameters for the VRS/PHS driver
 *
 **********************************************************************************************/


#include "Ifx_Utility.h"



boolean Init_Complete = FALSE;
IfxVrs_Attributes vrsAttributes;
uint32 firstGap = 0;
volatile uint32 *pmem_TSF_T = (uint32 *) 0xf012c400u;
volatile uint32 *pmem_RDT_T = (uint32 *) 0xf012c000u;
volatile uint32 *DT_Ti = (uint32 *) 0xf012cc00u;    //CLC
uint32 *pmem_ADT_Tx = &GTM_DPLL_ADT_T;
uint8 simulateGap = 0;


void Reset_Procedure(void)
{
	GTM_CTRL.B.RF_PROT = 0;     //SW RST (global), SW interrupt FORCINT, and SW RAM reset functionality is enabled
	GTM_RST.B.RST = 1;
	GTM_DPLL_CTRL_1.B.SWR =1;
	GTM_CTRL.B.RF_PROT = 1;     //SW RST (global), SW interrupt FORCINT, and SW RAM reset functionality is enabled

	/*DPLL Ram initialize state check*/
	while(((GTM_DPLL_RAM_INI.B.INIT_1B == TRUE)||(GTM_DPLL_RAM_INI.B.INIT_1B == TRUE)))
	{

	}
}
void DPLL_Clock_Ini(void)
{
    if ((GTM_CMU_CLK_EN.B.EN_CLK0 == CLK_ENABLED)||(GTM_CMU_CLK_EN.B.EN_CLK1 == CLK_ENABLED))
    {
        GTM_CMU_CLK_EN.B.EN_CLK0 = CLK_DIS;   //Disable it first
        GTM_CMU_CLK_EN.B.EN_CLK1 = CLK_DIS;   //Disable it first
        GTM_TBU_CHEN.B.ENDIS_CH0 = CLK_DIS;   //Disable TBU_CH0 counter
        GTM_TBU_CHEN.B.ENDIS_CH1 = CLK_DIS;   //Disable TBU_CH0 counter
    }

    GTM_CMU_CLK_0_CTRL.B.CLK_CNT = CLK_CNT_100ns;   /* 10ns x (9+1) => 100ns period */
    GTM_CMU_CLK_1_CTRL.B.CLK_CNT = CLK_CNT_1ms; /* 10ns x (99999+1) => 1ms resolution, Used for Crank Timeout */

    GTM_CMU_CLK_EN.B.EN_CLK0 = CLK_EN;   //enable clk_0
    GTM_CMU_CLK_EN.B.EN_CLK1 = CLK_EN;   //enable clk1
}

void Dpll_Ini (void)
{
    // configure DPLL
    GTM_DPLL_CTRL_0.B.TEN =1;
    GTM_DPLL_CTRL_0.B.IFP =0;
    GTM_DPLL_CTRL_0.B.TNU =VRS_DPLL_TNU;

    GTM_DPLL_CTRL_0.B.MLT = (IFX_VRS_TOOTH_STEPS - 1) & 0x0000007FF;
    GTM_DPLL_CTRL_1.B.TSL = IfxVrs_ActiveEdge_falling;  //(High-low slope)
    GTM_DPLL_CTRL_1.B.SYN_NT = 2;
    GTM_DPLL_CTRL_1.B.PIT = 1;//TIME RELATED

    GTM_DPLL_MLS1.U = (uint32) 0x0; /* number of pulses between two state events */
    GTM_DPLL_OSW.U = (uint32) 0x100;    /* Update RAM size,256 entries */

    GTM_DPLL_NTI_CNT.U = (uint32) 0;
    GTM_DPLL_MPVAL1.U = (uint32) 0x00000000;    //Very Important
    GTM_DPLL_PVT.U = (uint32) 25;   //Plausability_val

    GTM_DPLL_TOV.U = IFX_VRS_TS_T_CHECK_GAP;    //TS_T_Check TOV value
    GTM_DPLL_THMI.U = 0;        // Trigger Hold Time min */
    GTM_DPLL_THMA.U = (uint32) 2000;    // Trigger Hold Time max */
    GTM_DPLL_TLR.U = (uint32) 0;    //Disable TLR Check

    GTM_DPLL_IRQ_EN.U = (uint32) 0x07FFFFFF;    /* all interrupts */
    GTM_DPLL_IRQ_EN.B.CDSI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TISI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TAXI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.PWI_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.LL1I_IRQ_EN = 0;
    GTM_DPLL_IRQ_EN.B.TASI_IRQ_EN = 1;
    GTM_DPLL_CTRL_1.B.DEN = 0;
    GTM_DPLL_CTRL_1.B.SSL = 0;
}

void Dpll_Interrupt_Ini (void)
{
	GTM_DPLL_IRQ_EN.U = (uint32) 0x06fffBd7; //ALL DPLL IR ENABLE
    SRC_GTMTIM00.U	= (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TIM00_INT_NUM; 	/* 0<11 => ON IFXVRS_CPU2, 1<<10 => Enable */
    SRC_GTMATOM40.U	= (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | ATOM40_INT_NUM;  /* ATOM4_CH0/CH1, Enabled */

    SRC_GTMDPLL7.U	= (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | MTI_INT_NUM;  	/* 0<11 => ON IFXVRS_CPU2, 0<<10 => Disabled *///MTI
    SRC_GTMDPLL9.U	= (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | TASI_INT_NUM;	/* TAS 0<11 => ON IFXVRS_CPU2, 1<<10 => Enable -TRIGGER active slope detected while NTI_CNT is zero */
    SRC_GTMDPLL13.U = (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | GLI_INT_NUM;		/* 0<11 => ON IFXVRS_CPU2, 1<<10 => Enable *///GL
    SRC_GTMDPLL18.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TE0_INT_NUM;		/* TE0 used for a EventOnTooth */
    SRC_GTMDPLL19.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TE1_INT_NUM;		/* TE1 */
    SRC_GTMDPLL20.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TE2_INT_NUM;		/* TE2 */
    SRC_GTMDPLL21.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TE3_INT_NUM;		/* TE3 */
    SRC_GTMDPLL22.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | TE4_INT_NUM;		/* TE4 */
    SRC_GTMDPLL23.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | CDTI_INT_NUM;	/* CDTI */

    SRC_GTMDPLL25.U = (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | TORI_INT_NUM;	/*Trigger out of range interrupt */
    SRC_GTMDPLL14.U = (IFXVRS_CPU2 << 11) | (DISABLED_INT << 10) | LLI_INT_NUM;  	/* Lost Lock Interrupt */
    SRC_GTMDPLL10.U = (IFXVRS_CPU2 << 11) | (ENABLED_INT << 10)  | PWI_INT_NUM;  	/* Lost Lock Interrupt */
}

void Signal_Input_Ini (void)
{
	 /*Configure TIM0_CH0 (TRIGGER = Crankshaft) */
	GTM_TIM0_CH0_FLT_RE.U = (uint32) 0x00000040;    /* Acceptance time for rising edge */
	GTM_TIM0_CH0_FLT_FE.U = (uint32) 0x00000050;    /* Acceptance time for falling edge */
	GTM_TIM0_CH0_CTRL.B.ISL = 1;
	GTM_TIM0_CH0_CTRL.B.DSL = 0;
	GTM_TIM0_CH0_CTRL.B.TIM_MODE = TIEM;
	GTM_TIM0_CH0_CTRL.B.FLT_CTR_RE =1;
	GTM_TIM0_CH0_IRQ_MODE.B.IRQ_MODE = PULSE_NOTIFY;
	GTM_TIM0_CH0_IRQ_EN.B.NEWVAL_IRQ_EN = IRQ_ENABLE;
	GTM_TIM0INSEL.U = 0x1;     						 /*ATOM2_CH0 ===> TIM0_CH0 (TOUT/TIN18) - TRIGGER (P00.09)*/
	GTM_TIM0_CH0_CTRL.B.TIM_EN = 1;
}

void Timeout_Detect_Mode(void)
{
	GTM_TIM0_CH0_CTRL.B.TOCTRL = FALLING_EDGE;
	GTM_TIM0_CH0_TDUV.U = ENGINE_STOP_TIMEOUT | CMU_CLK1 << 28;
	GTM_TIM0_CH0_IRQ_EN.B.TODET_IRQ_EN = IRQ_ENABLE;
}

void Debug_port_Ini(void)
{
	P00_IOCR8.B.PC10 = 0x10;//only for debugging
	P02_IOCR0.B.PC0 = 0x10;//only for debugging
	P02_IOCR0.B.PC1 = 0x10;//only for debugging
	P02_IOCR0.B.PC2 = 0x10;//only for debugging
	P02_IOCR0.B.PC3 = 0x10;//only for debugging
	P20_IOCR4.B.PC6 = 0x10;//only for debugging
	P14_IOCR0.B.PC2 = 0x10;//only for debugging
	P14_IOCR8.B.PC8 = 0x10;//only for debugging
}
void port_14_2(void)
{
	P14_OMSR.B.PS2=1; /*only for debugging*/
	P14_OMCR.B.PCL2=1;/*only for debugging*/
}
void port_0_10(void)
{
	P00_OMSR.B.PS10=1; /*only for debugging*/
	P00_OMCR.B.PCL10=1;/*only for debugging*/
}
void port_2_0(void)
{
	P02_OMSR.B.PS0=1; /*only for debugging*/
	P02_OMCR.B.PCL0=1;/*only for debugging*/
}
void port_2_1(void)
{
	P02_OMSR.B.PS1=1; /*only for debugging*/
	P02_OMCR.B.PCL1=1;/*only for debugging*/
}
void port_2_2(void)
{
	P02_OMSR.B.PS2=1; /*only for debugging*/
	P02_OMCR.B.PCL2=1;/*only for debugging*/
}

void port_2_3(void)
{


	P02_OMSR.B.PS3=1; /*only for debugging*/
	P02_OMCR.B.PCL3=1;/*only for debugging*/


}
void port_20_6(void)
{


	P20_OMSR.B.PS6=1; /*only for debugging*/
	P20_OMCR.B.PCL6=1;/*only for debugging*/


}

