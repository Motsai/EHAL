/*--------------------------------------------------------------------------
File   : idelay.h

Author : Hoang Nguyen Hoan          May 22, 2015

Desc   : Delay loop functions.
		 The delay loop is calculated based on a 16MHz ARM
		 loop is invariant to compiler optimization.
		 nsec delay cannot be achieved is at 295ns per loop.

Copyright (c) 2011, I-SYST inc., all rights reserved

Permission to use, copy, modify, and distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies, and none of the
names : I-SYST or its contributors may be used to endorse or
promote products derived from this software without specific prior written
permission.

For info or contributing contact : hnhoan at i-syst dot com

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------
Modified by         Date            Description
----------------------------------------------------------------------------*/
#ifndef __IDELAY_H__
#define __IDELAY_H__
#include <stdint.h>
#include "system_core_clock.h"

extern uint32_t SystemMicroSecLoopCnt;

/**
 * Microsecond delay. This function is based on a 16MHz clock. For higher clock
 * rate SystemMicroSecNopCnt needs to be adjusted.  Adjustment of this variable
 * should be done in the CMSIS SystemCoreCLockUpdate function.
 * This delay is only approximate, it is not 100% accurate.
 */
static inline __attribute__((always_inline)) void usDelay(uint32_t cnt) {
	asm volatile (
		".syntax unified\n"
			"ORRS %[ucnt], %[ucnt]\n"
			"BEQ 2f\n"
			"MOVS r0, %[ucnt]\n"
		"1:\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" NOP\n"
			" SUBS r0, #1\n"
			" BGT 1b\n"
		"2:\n"
		".syntax divided\n"
		:
		: [ucnt] "r" (cnt * SystemMicroSecLoopCnt)
		:"r0"
		 );
}

/**
 * This is highly inaccurate use at you own risk
 *
 * nsec delay cannot be achieved for low cpu clock.
 * this loop is 295ns on a 16MHz cpu
 */
static inline __attribute__((always_inline)) void nsDelay(uint32_t cnt) {
	asm volatile (
		".syntax unified\n"
			"MOVS r1, %[ucnt]\n"
		"1:\n"
			" SUBS r1, #1\n"
			" BGT 1b\n"
		"2:\n"
		".syntax divided\n"
		:
		: [ucnt] "r" ((cnt + (SYSTEM_NSDELAY_CORE_FACTOR >> 1)) / SYSTEM_NSDELAY_CORE_FACTOR)
		:"r1"
		 );
}

#endif	// __IDELAY_H__

