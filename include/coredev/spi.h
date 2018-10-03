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
#ifndef __SPI_H__
#define __SPI_H__

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
typedef enum __SPI_Status {
	SPISTATUS_OK
} SPISTATUS;

typedef enum __SPI_Mode {
	SPIMODE_MASTER,
	SPIMODE_SLAVE
} SPIMODE;

typedef enum __SPI_Clk_Polarity {
	SPICLKPOL_HIGH,
	SPICLKPOL_LOW
} SPICLKPOL;

typedef enum __SPI_Data_Phase {
	SPIDATAPHASE_FIRST_CLK,		//!< Data phase starts on first clock
	SPIDATAPHASE_SECOND_CLK		//!< Data phase starts on 2nd clock
} SPIDATAPHASE;

typedef enum __SPI_Data_Bit_Order {
	SPIDATABIT_MSB,				//!< Most significant bit first
	SPIDATABIT_LSB				//!< Least significant bit first
} SPIDATABIT;

typedef enum __SPI_Chip_Select {
	SPICSEL_AUTO,	//!< Select control by hardware
	SPICSEL_MAN,	//!< Select control by software
	SPICSEL_EXT,	//!< Select control externally by application
} SPICSEL;

#define SPI_MAX_RETRY			5

#define SPI_SLAVEMODE_MAX_DEV	4	//!< Max number of device (CS) supported in slave mode
 	 	 	 	 	 	 	 	 	//!< the real implementation may support less depending on hardware

#define SPI_MAX_NB_IOPIN		4

/// SPI pins indexes
#define SPI_SCK_IOPIN_IDX		0
#define SPI_MISO_IOPIN_IDX		1
#define SPI_MOSI_IOPIN_IDX		2
#define SPI_SS_IOPIN_IDX		3	//!< Starting index for SPI chip select. This can
									//!< grow to allows multiple devices on same SPI.

#pragma pack(push, 4)

/// Configuration data used to initialize device
typedef struct __SPI_Config {
	int DevNo;				//!< SPI interface number identify by chip select (CS0, CS1,..,CSn)
	SPIMODE Mode;			//!< Master/Slave mode
	const IOPINCFG *pIOPinMap;	//!< Define I/O pins used by SPI
	int NbIOPins;			//!< Total number of I/O pins
	int Rate;				//!< Speed in Hz
	uint32_t DataSize; 		//!< Data Size 4-16 bits
	int MaxRetry;			//!< Max number of retry
	SPIDATABIT BitOrder;	//!< Data bit ordering
	SPIDATAPHASE DataPhase;	//!< Data Out Phase.
	SPICLKPOL ClkPol;		//!< Clock Out Polarity.
	SPICSEL ChipSel;		//!< Chip select mode
	bool bDmaEn;			//!< true - Use DMA mode only on supported devices
	bool bIntEn;			//!< Interrupt enable
	int IntPrio;			//!< Interrupt priority
	DEVINTRF_EVTCB EvtCB;	//!< Event callback
} SPICFG;

/// Device driver data require by low level functions
typedef struct {
	SPICFG Cfg;				//!< Config data
	DEVINTRF DevIntrf;		//!< device interface implementation
	int	FirstRdData;		//!< This is to keep the first dummy read data of SPI
							//!< there are devices that may return a status code through this
	int	CurDevCs;			//!< Current active device CS
	uint8_t *pRxBuff[SPI_SLAVEMODE_MAX_DEV];//!< Pointer to slave mode rx buffer
	int RxBuffLen[SPI_SLAVEMODE_MAX_DEV];	//!< Rx buffer length in bytes
	uint8_t *pTxData[SPI_SLAVEMODE_MAX_DEV];//!< Pointer to slave mode tx data
	int TxDataLen[SPI_SLAVEMODE_MAX_DEV];	//!< Tx data length in bytes
} SPIDEV;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

// Require implementations
bool SPIInit(SPIDEV * const pDev, const SPICFG *pCfgData);

static inline int SPIGetRate(SPIDEV * const pDev) { return pDev->DevIntrf.GetRate(&pDev->DevIntrf); }
static inline int SPISetRate(SPIDEV * const pDev, int Rate) {
	return pDev->DevIntrf.SetRate(&pDev->DevIntrf, Rate);
}
static inline void SPIEnable(SPIDEV * const pDev) { pDev->DevIntrf.Enable(&pDev->DevIntrf); }
static inline void SPIDisable(SPIDEV * const pDev) { pDev->DevIntrf.Disable(&pDev->DevIntrf); }
static inline int SPIRx(SPIDEV * const pDev, int DevCs, uint8_t *pBuff, int Bufflen) {
	return DeviceIntrfRx(&pDev->DevIntrf, DevCs, pBuff, Bufflen);
}
static inline int SPITx(SPIDEV * const pDev, int DevCs, uint8_t *pData, int DataLen) {
	return DeviceIntrfTx(&pDev->DevIntrf, DevCs, pData, DataLen);
}
static inline bool SPIStartRx(SPIDEV * const pDev, int DevAddr) {
	return DeviceIntrfStartRx(&pDev->DevIntrf, DevAddr);
}
static inline int SPIRxData(SPIDEV * const pDev, uint8_t *pBuff, int Bufflen) {
	return DeviceIntrfRxData(&pDev->DevIntrf, pBuff, Bufflen);
}
static inline void SPIStopRx(SPIDEV * const pDev) { DeviceIntrfStopRx(&pDev->DevIntrf); }
static inline bool SPIStartTx(SPIDEV * const pDev, int DevAddr) {
	return DeviceIntrfStartTx(&pDev->DevIntrf, DevAddr);
}
static inline int SPITxData(SPIDEV * const pDev, uint8_t *pData, int Datalen) {
	return DeviceIntrfTxData(&pDev->DevIntrf, pData, Datalen);
}
static inline void SPIStopTx(SPIDEV * const pDev) { DeviceIntrfStopTx(&pDev->DevIntrf); }

/**
 * @brief	Set SPI slave data for read command.
 *
 * This function sets internal pointer to the location of data to be returned to I2C master upon
 * receiving read command.
 *
 * @param	pDev	: Pointer SPI driver data initialized be SPIInit function
 * @param	SlaveIdx: Slave address index to assign the buffer
 * @param	pBuff	: Pointer to buffer to receive data from master
 * @param	BuffLen	: Total buffer length in bytes
 *
 * @return	None
 */
void SPISetSlaveRxBuffer(SPIDEV * const pDev, int SlaveIdx, uint8_t * const pBuff, int BuffLen);

/**
 * @brief	Set I2C slave buff for write command.
 *
 * This function sets internal pointer to the location of buffer to data from I2C master upon
 * receiving write command.
 *
 * @param	pDev	: Pointer I2C driver data initialized be I2CInit function
 * @param	SlaveIdx: Slave address index to assign the data buffer
 * @param	pData	: Pointer to data buffer to send to master
 * @param	DataLen	: Total data length in bytes
 *
 * @return	None
 */
void SPISetSlaveTxData(SPIDEV * const pDev, int SlaveIdx, uint8_t * const pData, int DataLen);


#ifdef __cplusplus
}

/// C++ SPI class wrapper
class SPI : public DeviceIntrf {
public:
	SPI() {
		memset((void*)&vDevData, 0, (int)sizeof(vDevData));
	}

	virtual ~SPI() {
	}

	SPI(SPI&);	// Copy ctor not allowed

	bool Init(const SPICFG &CfgData) { return SPIInit(&vDevData, &CfgData); }

	operator DEVINTRF * const () { return &vDevData.DevIntrf; }
	operator SPIDEV& () { return vDevData; };	// Get config data
	int Rate(int RateHz) { return vDevData.DevIntrf.SetRate(&vDevData.DevIntrf, RateHz); }
	int Rate(void) { return vDevData.DevIntrf.GetRate(&vDevData.DevIntrf); }	// Get rate in Hz
	void Enable(void) { DeviceIntrfEnable(&vDevData.DevIntrf); }
	void Disable(void) { DeviceIntrfDisable(&vDevData.DevIntrf); }

	// DevCs is the ordinal starting from 0 of device connected to the SPI bus.
	// It is translated to CS index in the I/O pin map
	virtual bool StartRx(int DevCs) {
		return DeviceIntrfStartRx(&vDevData.DevIntrf, DevCs);
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
	int GetFirstRead(void) {return vDevData.FirstRdData;}

	/**
	 * @brief	Set SPI slave rx buffer to receive data from master.
	 *
	 * This function sets internal pointer to the location of data to be returned to I2C master upon
	 * receiving read command.
	 *
	 * @param	SlaveIdx: Slave address index to assign the buffer
	 * @param	pBuff	: Pointer to buffer to receive data from master
	 * @param	BuffLen	: Total buffer length in bytes
	 *
	 * @return	None
	 */
	virtual void SetSlaveRxBuffer(int SlaveIdx, uint8_t * const pBuff, int BuffLen) {
		SPISetSlaveRxBuffer(&vDevData, SlaveIdx, pBuff, BuffLen);
	}

	/**
	 * @brief	Set I2C slave tx data buffer to se to master.
	 *
	 * This function sets internal pointer to the location of buffer to data from I2C master upon
	 * receiving write command.
	 *
	 * @param	SlaveIdx: Slave address index to assign the data buffer
	 * @param	pData	: Pointer to data buffer to send to master
	 * @param	DataLen	: Total data length in bytes
	 *
	 * @return	None
	 */
	virtual void SetSlaveTxData(int SlaveIdx, uint8_t * const pData, int DataLen) {
		SPISetSlaveTxData(&vDevData, SlaveIdx, pData, DataLen);
	}

private:
	SPIDEV vDevData;
};

#endif	// __cplusplus

/** @} end group device_intrf */

#endif	// __SPI_H__
