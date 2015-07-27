/**
 * \file debug_utils.h
 * \brief Basic Debug Functions.
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

#ifndef GTM_EXAMPLE_DEF_H
#define GTM_EXAMPLE_DEF_H

#define SOMC (1)
#define SOMP (2)
#define ASYNC_UPDATE (0)
#define SYNC_UPDATE  (1)

#define CMU_CLK0 (0)
#define CMU_CLK1 (1)

/*INTERRUPTS PRIO */
#define TIM0_CH0_PRIO      7
#define TIM1_CH0_PRIO      3
#define TIM1_CH1_PRIO	   3
#define TIM2_CH0_PRIO      3
#define TIM3_CH0_PRIO      4
#define ATOM2_CH0_PRIO     1

#define FIFO0_CH0 (0)
#define FIFO0_CH1 (1)
#define FIFO0_CH2 (2)
#define FIFO0_CH3 (3)
#define FIFO0_CH4 (4)
#define FIFO0_CH5 (5)
#define FIFO0_CH6 (6)
#define FIFO0_CH7 (7)

#define IFX_CPU0 (0)
#define IFX_CPU1 (1)
#define IFX_CPU2 (2)

#define IR_ENABLE (1)
#define IR_DISABLE (0)

#define ATOM2_CH0_IRQ_PRIO_0 (11)
#define ATOM2_CH0_IRQ_PRIO_1 (12)
#define TOM0_CH0_IRQ_PRIO_1 (13)

#endif

