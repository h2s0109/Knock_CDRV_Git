/*****************************************************************************
*
* Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
******************************************************************************/

/*!
 * \file dsplib.h
 * \brief Infineon DSP library API include file
 *
 * This file contains the API header for the Infineon DSP library.
 */

#ifndef IFX_DSPLIB_H
#define IFX_DSPLIB_H

/*---------------------------------------------------------------------------*/
/*! FIR filter Q15 format
* \param[in] x vector[n] input signal
* \param[out] y vector[n] result
* \param[in] c vector[m] coeffcients
* \param[in,out] s vector[m-1] intermediate state
* \param[in] n number of samples in input signal
 */
#include "knocheck_def.h"
#pragma section code "cpu1_psram"
void Ifx_firRealQ15a(
                sint16 *    x,
                sint16 *    y,
                KNOCHECK_CIRBUFF c,
                KNOCHECK_CIRBUFF * s,
                uint32      n
                );
#pragma section code restore
#endif /* IFX_DSPLIB_H */


