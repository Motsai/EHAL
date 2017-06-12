/*--------------------------------------------------------------------------
File   : saadc_nrf52.cpp

Author : Simon Ouellet

Copyright (c) 2017, I-SYST inc., all rights reserved

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

#include "rtc_nrf52.h"

extern "C" {
#include "nrf.h"
}

/**********************************************************************************/

#define     NRF52_RTC_COUNT         3

static const IRQn_Type gpIRQ[NRF52_RTC_COUNT] = { RTC0_IRQn, RTC1_IRQn, RTC2_IRQn };
static NRF_RTC_Type* gpRegister[NRF52_RTC_COUNT] = { NRF_RTC0, NRF_RTC1, NRF_RTC2 };

/**********************************************************************************/

/* Converts the specified microsecond time to the closest prior RTC unit. */
static void convert( uint32_t pTimeUs, uint32_t* pUnitsRTC, uint32_t* pUnitsUs)
{
   uint32_t u1, u2, t1, t2, t3;

   t1 = pTimeUs / 15625;
   u1 = t1 * 512;
   t2 = pTimeUs - t1 * 15625;
   u2 = (t2 << 9) / 15625;
   t3 = (((t2 << 9) - u2 * 15625) + 256) >> 9;

   *pUnitsRTC = u1 + u2;
   *pUnitsUs = t3;
}

/**********************************************************************************/

void nRF52_RTC::reset( uint8_t pIndex )
{
    if ( pIndex < NRF52_RTC_COUNT ) {
        gpRegister[pIndex]->EVTENSET = ( RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos );
        gpRegister[pIndex]->INTENSET = ( RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos );
        gpRegister[pIndex]->CC[0]    = 0;
    }
}

/**********************************************************************************/

bool nRF52_RTC::start( uint8_t pIndex )
{
    if ( pIndex < NRF52_RTC_COUNT ) {
        NVIC_ClearPendingIRQ( gpIRQ[pIndex] );
        NVIC_EnableIRQ( gpIRQ[pIndex] );

        gpRegister[pIndex]->TASKS_CLEAR = 1;
        gpRegister[pIndex]->TASKS_START = 1;
        return true;
    }
    return false;
}

/**********************************************************************************/

void nRF52_RTC::stop( uint8_t pIndex )
{
    if ( pIndex < NRF52_RTC_COUNT ) {
        NVIC_ClearPendingIRQ( gpIRQ[pIndex] );
        NVIC_DisableIRQ( gpIRQ[pIndex] );

        gpRegister[pIndex]->TASKS_CLEAR = 1;
        gpRegister[pIndex]->TASKS_STOP = 1;
    }
}

/**********************************************************************************/

void nRF52_RTC::setTimeout( uint8_t pIndex, uint32_t pTimeout )
{
    if ( pIndex < NRF52_RTC_COUNT ) {
        uint32_t rtc_units, us_units;
        convert( pTimeout, &rtc_units, &us_units );

        gpRegister[pIndex]->CC[0]    = rtc_units;
        gpRegister[pIndex]->EVTENSET = ( RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos );
        gpRegister[pIndex]->INTENSET = ( RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos );
    }
}

/**********************************************************************************/


