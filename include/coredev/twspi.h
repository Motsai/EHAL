/**-------------------------------------------------------------------------
@file	spi.h

@brief	Generic SPI (Serial Peripheral Interface) driver definitions.

Current implementation
 	 - Master mode
 	 - Polling

@author	Hoang Nguyen Hoan
@date	Nov. 20, 2011

@license

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

----------------------------------------------------------------------------*/
#ifndef __TWSPI_H__
#define __TWSPI_H__

#include <stdint.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "iopincfg.h"
#include "device_intrf.h"

/** @addtogroup device_intrf	Device Interface
  * @{
  */


// SPI Status code
typedef enum __TWSPI_Status {
	TWSPISTATUS_OK
} TWSPISTATUS;

typedef enum __TWSPI_Clk_Polarity {
    TWSPICLKPOL_HIGH,
    TWSPICLKPOL_LOW
} TWSPICLKPOL;

typedef enum __TWSPI_Data_Bit_Order {
    TWSPIDATABIT_MSB,				//!< Most significant bit first
    TWSPIDATABIT_LSB				//!< Least significant bit first
} TWSPIDATABIT;

#define TWSPI_MAX_RETRY			5

#define TWSPI_MAX_NB_IOPIN		2

/// SPI pins indexes
#define TWSPI_CLK_IOPIN_IDX		0
#define TWSPI_DAT_IOPIN_IDX		1

#pragma pack(push, 4)

/// Configuration data used to initialize device
typedef struct __TWSPI_Config {
	const IOPINCFG *pIOPinMap;	//!< Define I/O pins used by SPI
	int NbIOPins;			//!< Total number of I/O pins
	int Rate;				//!< Speed in Hz
	uint32_t DataSize; 		//!< Data Size 4-32 bits
	int MaxRetry;			//!< Max number of retry
	TWSPIDATABIT BitOrder;	//!< Data bit ordering
	TWSPICLKPOL ClkPol;		//!< Clock Out Polarity.
	DEVINTRF_EVTCB EvtCB;	//!< Event callback
} TWSPICFG;

/// Device driver data require by low level functions
typedef struct {
	TWSPICFG Cfg;				//!< Config data
	DEVINTRF DevIntrf;		//!< device interface implementation
	uint32_t Clk;
} TWSPIDEV;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

// Require implementations
bool TWSPIInit(TWSPIDEV * const pDev, const TWSPICFG *pCfgData);

static inline int TWSPIGetRate(TWSPIDEV * const pDev) { return pDev->DevIntrf.GetRate(&pDev->DevIntrf); }
static inline int TWSPISetRate(TWSPIDEV * const pDev, int Rate) {
	return pDev->DevIntrf.SetRate(&pDev->DevIntrf, Rate);
}
static inline void TWSPIEnable(TWSPIDEV * const pDev) { pDev->DevIntrf.Enable(&pDev->DevIntrf); }
static inline void TWSPIDisable(TWSPIDEV * const pDev) { pDev->DevIntrf.Disable(&pDev->DevIntrf); }
static inline int TWSPIRx(TWSPIDEV * const pDev, int DevCs, uint8_t *pBuff, int Bufflen) {
	return DeviceIntrfRx(&pDev->DevIntrf, DevCs, pBuff, Bufflen);
}
static inline int TWSPITx(TWSPIDEV * const pDev, int DevCs, uint8_t *pData, int DataLen) {
	return DeviceIntrfTx(&pDev->DevIntrf, DevCs, pData, DataLen);
}
static inline bool TWSPIStartRx(TWSPIDEV * const pDev, int DevAddr) {
	return DeviceIntrfStartRx(&pDev->DevIntrf, DevAddr);
}
static inline int TWSPIRxData(TWSPIDEV * const pDev, uint8_t *pBuff, int Bufflen) {
	return DeviceIntrfRxData(&pDev->DevIntrf, pBuff, Bufflen);
}
static inline void TWSPIStopRx(TWSPIDEV * const pDev) { DeviceIntrfStopRx(&pDev->DevIntrf); }
static inline bool TWSPIStartTx(TWSPIDEV * const pDev, int DevAddr) {
	return DeviceIntrfStartTx(&pDev->DevIntrf, DevAddr);
}
static inline int TWSPITxData(TWSPIDEV * const pDev, uint8_t *pData, int Datalen) {
	return DeviceIntrfTxData(&pDev->DevIntrf, pData, Datalen);
}
static inline void TWSPIStopTx(TWSPIDEV * const pDev) { DeviceIntrfStopTx(&pDev->DevIntrf); }


#ifdef __cplusplus
}

/// C++ Two-Wire SPI class wrapper
class TWSPI : public DeviceIntrf {
public:
    TWSPI() {
		memset((void*)&vDevData, 0, (int)sizeof(vDevData));
	}

	virtual ~TWSPI() {
	}

	TWSPI(TWSPI&);	// Copy ctor not allowed

	bool Init(const TWSPICFG &CfgData) { return TWSPIInit(&vDevData, &CfgData); }

	operator DEVINTRF * const () { return &vDevData.DevIntrf; }
	operator TWSPIDEV& () { return vDevData; };			// Get config data
	operator TWSPIDEV * const () { return &vDevData; };	// Get ponter to config data
	int Rate(int RateHz) { return vDevData.DevIntrf.SetRate(&vDevData.DevIntrf, RateHz); }
	int Rate(void) { return vDevData.DevIntrf.GetRate(&vDevData.DevIntrf); }	// Get rate in Hz
	void Enable(void) { DeviceIntrfEnable(&vDevData.DevIntrf); }
	void Disable(void) { DeviceIntrfDisable(&vDevData.DevIntrf); }

	// DevCs is the ordinal starting from 0 of device connected to the SPI bus.
	// It is translated to CS index in the I/O pin map
	virtual bool StartRx(int pAddr) {
		return DeviceIntrfStartRx(&vDevData.DevIntrf, pAddr);
	}
	// Receive Data only, no Start/Stop condition
	virtual int RxData(uint8_t *pBuff, int BuffLen) {
		return DeviceIntrfRxData(&vDevData.DevIntrf, pBuff, BuffLen);
	}
	virtual void StopRx(void) { DeviceIntrfStopRx(&vDevData.DevIntrf); }
	// DevAddr is the ordinal starting from 0 of device connected to the SPI bus.
	// It is translated to CS index in the I/O pin map
	virtual bool StartTx(int DevCs) {
		return DeviceIntrfStartTx(&vDevData.DevIntrf, DevCs);
	}
	// Send Data only, no Start/Stop condition
	virtual int TxData(uint8_t *pData, int DataLen) {
		return DeviceIntrfTxData(&vDevData.DevIntrf, pData, DataLen);
	}
	virtual void StopTx(void) { DeviceIntrfStopTx(&vDevData.DevIntrf); }

private:
	TWSPIDEV vDevData;
};

#endif	// __cplusplus

/** @} end group device_intrf */

#endif	// __TWSPI_H__
