/**
 * \file IfxGtmCmu.h
 * \brief Basic GTM Clock and TBU setup functions.
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

#ifndef IFXGTMCLK_H
#define IFXGTMCLK_H


#include "IfxGtm_reg.h"

#define FXCLK0   (0)
#define FXCLK1   (1)
#define FXCLK2   (2)
#define FXCLK3   (3)
#define FXCLK4   (4)
#define CMU_CLK0 (0)
#define CMU_CLK1 (1)
#define CMU_CLK2 (2)
#define CMU_CLK3 (3)
#define CMU_CLK4 (4)

void setupGtmClocks(void);
void setupGtmTbu(void);
#endif
