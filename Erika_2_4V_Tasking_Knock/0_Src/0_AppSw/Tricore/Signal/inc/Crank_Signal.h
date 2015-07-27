/**
 * \file TIM_Example.h
 * \brief Basic GTM TIM (Timer Input Module) Use Cases.
 *
 * \license
 * You can use this file under the terms of the IFX License.
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the IFX License for more details (IFX_License.txt).
 *
 * This file may be used, copied, and distributed, with or without modification, provided
 * that all copyright notices are retained; that all modifications to this file are
 * prominently noted in the modified file; and that this paragraph is not modified.
 *
 * \copyright Copyright (C) 2012 Infineon Technologies AG
 * \author Alfredo Baratta (alfredo.baratta@infineon.com)
 *
  */

#ifndef TIM_EXAMPLE_H
//#define TIM_EXAMPLE_H

#include "CMU_Setup.h"
#include "Platform_Types.h"
#include "IfxSrc_reg.h"
#include "IfxCpu_Irq.h"
#include "Compilers.h"
#include "IfxPort_reg.h"
#include "CDRV_Simul_Def.h"

void Crank_Signal(uint32 period,double duty);
void CAM_Signal(void );
void CAM_Start_point(void ) ;


#endif
