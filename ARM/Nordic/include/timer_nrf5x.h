/*--------------------------------------------------------------------------
File   : timer_nrf5x.h

Author : Hoang Nguyen Hoan          				Sep. 7, 2017

Desc   : timer class implementation on Nordic nRF5x series

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
Modified by          Date              Description

----------------------------------------------------------------------------*/

#ifndef __TIMER_NRF5x_H__
#define __TIMER_NRF5x_H__

#include <stdint.h>
#include "timer.h"

// *****
// Low frequency timer using Real Time Counter (RTC) 32768 Hz clock source
//
#define TIMER_NRF5X_RTC_BASE_FREQ   		32768
#ifdef NRF52
#define TIMER_NRF5X_RTC_MAX                 3           // 3 RTC available on nRF52
#else
#define TIMER_NRF5X_RTC_MAX                 2           // 2 RTC available on nRF51
#endif
#define TIMER_NRF5X_RTC_MAX_TRIGGER_EVT     4           // Max number of supported counter trigger event

class TimerLFnRF5x : public Timer {
public:
	TimerLFnRF5x();
    virtual ~TimerLFnRF5x();

	virtual bool Init(const TIMER_CFG &Cfg);
	virtual bool Enable();
	virtual void Disable();
	virtual void Reset();
	virtual uint32_t Frequency(uint32_t Freq);
	virtual uint32_t Frequency(void) { return vFreq; }
	virtual uint64_t TickCount();
    int MaxTimerTrigger() { return TIMER_NRF5X_RTC_MAX_TRIGGER_EVT; }
    virtual uint64_t EnableTimerTrigger(int TrigNo, uint64_t nsPeriod,
    									TIMER_TRIG_TYPE Type, TIMER_TRIGCB Handler = NULL);
	virtual uint32_t EnableTimerTrigger(int TrigNo, uint32_t msPeriod,
										TIMER_TRIG_TYPE Type, TIMER_TRIGCB Handler = NULL) {
		return (uint32_t)(EnableTimerTrigger(TrigNo, (uint64_t)msPeriod * 1000000ULL, Type, Handler) / 1000000ULL);
	}
    virtual void DisableTimerTrigger(int TrigNo);

    int GetFreeTimerTrigger(void);

    void IRQHandler();

protected:
private:
    NRF_RTC_Type *vpReg;
    uint32_t vCC[TIMER_NRF5X_RTC_MAX_TRIGGER_EVT];
    //TIMER_TRIG_TYPE vTrigType[TIMER_NRF5X_RTC_MAX_TRIGGER_EVT];
    TIMER_TRIGGER vTrigger[TIMER_NRF5X_RTC_MAX_TRIGGER_EVT];
};

// *****
// High frequency timer using Timer 16MHz clock source
//
#define TIMER_NRF5X_HF_BASE_FREQ   			16000000
#ifdef NRF52
#define TIMER_NRF5X_HF_MAX              	5           // 5 high frequency timer available on nRF52
#define TIMER_NRF5X_HF_MAX_TRIGGER_EVT  	6           // Max number of supported counter trigger event
#else
#define TIMER_NRF5X_HF_MAX              	3           // 3 high frequency timer available on nRF51
#define TIMER_NRF5X_HF_MAX_TRIGGER_EVT  	4           // Max number of supported counter trigger event
#endif


class TimerHFnRF5x : public Timer {
public:
    TimerHFnRF5x();
    virtual ~TimerHFnRF5x();

    virtual bool Init(const TIMER_CFG &Cfg);
    virtual bool Enable();
    virtual void Disable();
    virtual void Reset();
    virtual uint32_t Frequency(uint32_t Freq);
    virtual uint64_t TickCount();
    int MaxTimerTrigger() {
#ifdef NRF52
    	return vMaxNbTrigEvt;
#else
    	return TIMER_NRF5X_HF_MAX_TRIGGER_EVT;
#endif
    }
    virtual uint64_t EnableTimerTrigger(int TrigNo, uint64_t nsPeriod,
    									TIMER_TRIG_TYPE Type, TIMER_TRIGCB Handler = NULL);
	virtual uint32_t EnableTimerTrigger(int TrigNo, uint32_t msPeriod,
										TIMER_TRIG_TYPE Type, TIMER_TRIGCB Handler = NULL) {
		return (uint32_t)(EnableTimerTrigger(TrigNo, (uint64_t)msPeriod * 1000000ULL, Type, Handler) / 1000000ULL);
	}
    virtual void DisableTimerTrigger(int TrigNo);

    int GetFreeTimerTrigger(void);

    void IRQHandler();

protected:
private:

    NRF_TIMER_Type *vpReg;
    int vMaxNbTrigEvt;		// Number of trigger is not the same for all timers.
//#ifdef NRF52
//    uint32_t vCC[TIMER_NRF5X_HF_MAX_TRIGGER_EVT + 2];
    //TIMER_TRIG_TYPE vTrigType[TIMER_NRF5X_HF_MAX_TRIGGER_EVT + 2];
//    TIMER_TRIGGER vTrigger[TIMER_NRF5X_HF_MAX_TRIGGER_EVT + 2];
//#else
    uint32_t vCC[TIMER_NRF5X_HF_MAX_TRIGGER_EVT];
//    TIMER_TRIG_TYPE vTrigType[TIMER_NRF5X_HF_MAX_TRIGGER_EVT];
    TIMER_TRIGGER vTrigger[TIMER_NRF5X_HF_MAX_TRIGGER_EVT];
//#endif
};

#endif // __TIMER_NRF5x_H__
