/*
 * knocheck_mem.h
 *
 *  Created on: 2015. 6. 2.
 *      Author: G122885
 */

#ifndef KNOCHECK_MEM_H_
#define KNOCHECK_MEM_H_

#if defined (KNOCK_SEC_CODE)
	#undef  KNOCK_SEC_CODE
	#pragma section code ".text.cpu2_psram"
#endif /* KNOCHECK_MEM_H_ */
#if defined(__TASKING__)
#pragma protect on
#pragma section code
#endif
