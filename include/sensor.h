/**--------------------------------------------------------------------------
@file	sensor.h

@brief	Generic sensor abstraction.

@author	Hoang Nguyen Hoan
@date	Oct. 18, 2017


@license

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

----------------------------------------------------------------------------*/
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdint.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "iopincfg.h"
#include "device.h"
#include "timer.h"

/** @addtogroup Sensors
  * @{
  */



/// @brief	Sensor operating mode.
///
/// Single mode sensor capture once.\n
/// Continuous mode sensor capture at a constant rate set by sampling frequency.
/// Not all sensor devices have CONTINUOUS mode. If not avail, it can be implemented
/// using timer with SINGLE mode.
///
/// @note	Timer configuration and operation is handled by user firmware application
typedef enum __Sensor_OpMode {
	SENSOR_OPMODE_SINGLE,			//!< Single capture
	SENSOR_OPMODE_CONTINUOUS		//!< Continuous capture
} SENSOR_OPMODE;

///@brief	Sensor state.
///
///To indicate current state of the sensor.
///
typedef enum __Sensor_State {
	SENSOR_STATE_SLEEP,				//!< Sleep state low power
	SENSOR_STATE_IDLE,				//!< Idle state powered on
	SENSOR_STATE_SAMPLING			//!< Sampling in progress
} SENSOR_STATE;

#ifdef __cplusplus

/// @brief	Sensor generic base class.
///
/// Require implementations :
///	- bool StartSampling();
///
/// Require implementations from Device base class
///	- bool Enable();
///	- void Disable();
///	- void Reset();
///
class Sensor : virtual public Device {
public:
	/**
	 * @brief	Start sampling data
	 *
	 * This is a require implementation by sensor implementer.\n
	 * This function initiates sensor to do actual measurement.
	 *
	 * @return	true - success
	 * 			false - in case of error or sensor busy measuring
	 */
	virtual bool StartSampling() = 0;

	/**
	 * @brief	Set operating mode.
	 *
	 * Sensor implementation must overload this function to do necessary
	 * hardware settings for the operating mode.
	 * This base implementation only stores the values into member variables
	 *
	 * @param	OpMode : Operating mode
	 * 					- SENSOR_OPMODE_SINGLE
	 * 					- SENSOR_OPMODE_CONTINUOUS
	 * @param	Freq : Sampling frequency in Hz for continuous mode
	 *
	 * @return	true- if success
	 */
	virtual bool Mode(SENSOR_OPMODE OpMode, uint32_t Freq) {
		vOpMode = OpMode;
		vSampFreq = Freq;
        vSampPeriod = 1000000000LL / vSampFreq;

		if (vpTimer && OpMode == SENSOR_OPMODE_CONTINUOUS)
		{
		    vTimerTrigId = vpTimer->EnableTimerTrigger(vSampPeriod, TIMER_TRIG_TYPE_CONTINUOUS,
		                                               TimerTrigHandler, (void*)this);
		}
		return true;
	}

	/**
	 * @brief	Get current operating mode.
	 *
	 * @return	Operating mode.
	 * 				- SENSOR_OPMODE_SINGLE
	 * 				- SENSOR_OPMODE_CONTINUOUS
	 */
	virtual SENSOR_OPMODE Mode() { return vOpMode; }

	/**
	 * @brief	Get sampling period.
	 *
	 * @return	Sampling period in usec
	 */
	virtual uint64_t SamplingPeriod() { return vSampPeriod; }

	/**
	 * @brief	Get sampling frequency.
	 * 		The sampling frequency is relevant only in continuous mode
	 *
	 * @return	Frequency in mHz (milliHerz)
	 */
	virtual uint32_t SamplingFrequency() { return vSampFreq; }

	/**
	 * @brief	Set sampling frequency.
	 *
	 * The sampling frequency is relevant only in continuous mode.
	 *
	 * @return	Frequency in mHz (milliHerz)
	 */
	virtual uint32_t SamplingFrequency(uint32_t Freq) {
		vSampFreq = Freq;
		vSampPeriod = 1000000000LL / vSampFreq;

		return vSampFreq;
	}

	/**
	 * @brief	Set current sensor state
	 *
	 * @param 	State : New state to be set.
	 *				- SENSOR_STATE_SLEEP
	 *				- SENSOR_STATE_IDLE
	 *				- SENSOR_STATE_SAMPLING
	 *
	 * @return	Actual state. In the case where the new state could
	 * 			not be set, it returns the actual state of the sensor.
	 */
	virtual SENSOR_STATE State(SENSOR_STATE State) {
		vState = State;
		return vState;
	}

	/**
	 * @brief	Get current sensor state.
	 *
	 * @return	Current state.
	 *				- SENSOR_STATE_SLEEP
	 *				- SENSOR_STATE_IDLE
	 *				- SENSOR_STATE_SAMPLING
	 */
	virtual SENSOR_STATE State() { return vState; }

	/**
	 * @brief	Read sensor and update internal data with new readings
	 *
	 * This function should be called by a periodic timer to update
	 * sensor data in SENSOR_OPMODE_CONTINUOUS or interrupt or when Read is called
	 * in SENSOR_OPMODE_SINGLE
	 *
	 * @return	true - New data is updated
	 */
	virtual bool UpdateData() = 0;

	static void TimerTrigHandler(Timer * const pTimer, int TrigNo, void * const pContext) {
	    Sensor *sensor = (Sensor*)pContext;

	    sensor->UpdateData();
	    sensor->StartSampling();
	}

protected:

	SENSOR_STATE vState;		//!< Current sensor state
	SENSOR_OPMODE vOpMode;		//!< Current operating mode
	uint32_t vSampFreq;			//!< Sampling frequency in milliHerz, relevant to CONTINUOUS mode
	uint64_t vSampPeriod;		//!< Sampling period in nanosecond.
	Timer *vpTimer;				//!< Timer to use for time stamping data
	bool vbSampling;			//!< true - measurement in progress
	uint64_t vSampleCnt;		//!< Keeping sample count
	uint64_t vSampleTime;		//!< Time stamp when sampling is started
	int vTimerTrigId;
};

extern "C" {
#endif	// __cplusplus

#ifdef __cplusplus
}

#endif	// __cplusplus

/** @} End of group Sensors */

#endif	// __SENSOR_H__
