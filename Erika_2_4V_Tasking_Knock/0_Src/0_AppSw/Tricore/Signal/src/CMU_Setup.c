#include "CMU_Setup.h"
#include "Ifx_Types.h"
#include "Ifx_Utility.h"

void setupGtmClocks(void)
{
	if (GTM_CMU_CLK_EN.B.EN_CLK4==CLK_ENABLED)
	{
				GTM_CMU_CLK_EN.B.EN_CLK4=  1; //Disable it first
				GTM_TBU_CHEN.B.ENDIS_CH2 = 1; //Disable TBU_CH1 counter
	}
	if (GTM_CMU_CLK_EN.U==0x00000000) {
		GTM_CMU_GCLK_NUM.U = 0x00000001;
		GTM_CMU_GCLK_DEN.U = 0x00000001;
	}

	//2. TCMU_CLK[x]=(CLK_CNT[x]+1)*tGTM_CMU_GCLK
	GTM_CMU_CLK_4_CTRL.U = 0x00000009; /* ==> (9+1)*tGTM_CMU_GCLK   => 10x10ns (100ns)  */

	//GTM_CMU_CLK.

	//3. Enable Cmu and Fx Clocks
	GTM_CMU_CLK_EN.B.EN_CLK4 = CLK_EN;
	GTM_CMU_CLK_EN.B.EN_FXCLK = CLK_EN; //All the FXCLK will be enabled
}

void setupGtmTbu(void) {
	if (GTM_TBU_CHEN.B.ENDIS_CH2==3) {
		GTM_TBU_CHEN.B.ENDIS_CH2 = 1;
	}
	GTM_TBU_CH2_CTRL.B.CH_CLK_SRC = CMU_CLK4; //TBU_CH0 counter to count CMU_CLK0
	GTM_TBU_CHEN.B.ENDIS_CH2 = 2; //Enable TBU_CH0 counter
}
