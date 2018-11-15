/*--------------------------------------------------------------------------
File   : twspi.cpp

Author : Simon Ouellet          Nov. 15, 2018

Desc   : TWSPI generic implementation

Copyright (c) 2018, I-SYST inc., all rights reserved

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
#include "nrf.h"

#include "iopinctrl.h"
#include "twspi.h"

int TWSPIGetRate(DEVINTRF * const pDev)
{
	int rate = 0;

	if (pDev && pDev->pDevData)
		rate = ((TWSPIDEV*)pDev->pDevData)->Cfg.Rate;

	return rate;
}

// Set data rate in bits/sec (Hz)
// return actual rate
int TWSPISetRate(DEVINTRF * const pDev, int DataRate)
{
	TWSPIDEV *dev = (TWSPIDEV *)pDev->pDevData;

	if (DataRate < 250000)
	{
		dev->Clk = 125000;
	}
	else if (DataRate < 500000)
	{
		dev->Clk = 250000;
	}
	else if (DataRate < 1000000)
	{
		dev->Clk = 500000;
	}
	else if (DataRate < 2000000)
	{
		dev->Clk = 1000000;
	}
	else if (DataRate < 4000000)
	{
		dev->Clk = 2000000;
	}
	else if (DataRate < 8000000)
	{
		dev->Clk = 4000000;
	}
	else
	{
		dev->Clk = 8000000;
	}

	dev->Cfg.Rate = dev->Clk;

	return dev->Cfg.Rate;
}

void TWSPIDisable(DEVINTRF * const pDev)
{
	TWSPIDEV *dev = (TWSPIDEV *)pDev->pDevData;
}

void TWSPIEnable(DEVINTRF * const pDev)
{
    TWSPIDEV *dev = (TWSPIDEV *)pDev->pDevData;
}

// Initial receive
bool TWSPIStartRx(DEVINTRF * const pDev, int pAddr)
{
	TWSPIDEV *dev = (TWSPIDEV *)pDev->pDevData;

	IOPinClear(dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PortNo,
			   dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PinNo);
    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PinNo);

	return true;
}

// Receive Data only, no Start/Stop condition
int TWSPIRxData(DEVINTRF * const pDev, uint8_t *pBuff, int BuffLen)
{
    TWSPIDEV *dev = (TWSPIDEV *)pDev-> pDevData;
	int cnt = 0;

	/*while (BuffLen > 0)
	{
		int l = min(BuffLen, NRF52_SPI_DMA_MAXCNT);

		dev->pReg->RXD.PTR = (uint32_t)pBuff;
		dev->pReg->RXD.MAXCNT = l;
		dev->pReg->RXD.LIST = 0; // Scatter/Gather not supported
		dev->pReg->TXD.PTR = 0;
		dev->pReg->TXD.MAXCNT = 0;
		dev->pReg->TXD.LIST = 0; // Scatter/Gather not supported
		dev->pReg->EVENTS_END = 0;
		dev->pReg->EVENTS_ENDRX = 0;
		dev->pReg->TASKS_START = 1;

		if (nRF52SPIWaitRX(dev, 100000) == false)
			break;

        l = dev->pReg->RXD.AMOUNT;
		BuffLen -= l;
		pBuff += l;
		cnt += l;
	}*/

	return cnt;
}

// Stop receive
void TWSPIStopRx(DEVINTRF * const pDev)
{
    TWSPIDEV *dev = (TWSPIDEV *)pDev-> pDevData;

    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PinNo);
    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PinNo);
}

// Initiate transmit
bool TWSPIStartTx(DEVINTRF * const pDev, int DevCs)
{
    TWSPIDEV *dev = (TWSPIDEV *)pDev-> pDevData;

    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PinNo);
    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PinNo);

	return true;
}

// Transmit Data only, no Start/Stop condition
int TWSPITxData(DEVINTRF * const pDev, uint8_t *pData, int DataLen)
{
	TWSPIDEV *dev = (TWSPIDEV *)pDev-> pDevData;
	int cnt = 0;

	/*while (DataLen > 0)
	{
		int l = min(DataLen, NRF52_SPI_DMA_MAXCNT);
		dev->pReg->RXD.PTR = 0;
		dev->pReg->RXD.MAXCNT = 0;
		dev->pReg->RXD.LIST = 0; // Scatter/Gather not supported
		dev->pReg->TXD.PTR = (uint32_t)pData;
		dev->pReg->TXD.MAXCNT = l;
		dev->pReg->TXD.LIST = 0; // Scatter/Gather not supported
		dev->pReg->EVENTS_END = 0;
		dev->pReg->EVENTS_ENDTX = 0;
		dev->pReg->TASKS_START = 1;

		if (nRF52SPIWaitDMA(dev, 100000) == false)
		{
		    break;
		}

		l = dev->pReg->TXD.AMOUNT;
		DataLen -= l;
		pData += l;
		cnt += l;
	}*/

	return cnt;
}

// Stop transmit
void TWSPIStopTx(DEVINTRF * const pDev)
{
    TWSPIDEV *dev = (TWSPIDEV *)pDev-> pDevData;

    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_CLK_IOPIN_IDX].PinNo);
    IOPinClear(dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PortNo,
               dev->Cfg.pIOPinMap[TWSPI_DAT_IOPIN_IDX].PinNo);
}

bool TWSPIInit(TWSPIDEV * const pDev, const TWSPICFG *pCfgData)
{
	NRF_SPIM_Type *reg;
	uint32_t err_code;
	uint32_t cfgreg = 0;

	// Configure I/O pins
	IOPinCfg(pCfgData->pIOPinMap, pCfgData->NbIOPins);


	pDev->Cfg = *pCfgData;
	pDev->DevIntrf.pDevData = pDev;

	TWSPISetRate(&pDev->DevIntrf, pCfgData->Rate);

	pDev->DevIntrf.Disable = TWSPIDisable;
	pDev->DevIntrf.Enable = TWSPIEnable;
	pDev->DevIntrf.GetRate = TWSPIGetRate;
	pDev->DevIntrf.SetRate = TWSPISetRate;
	pDev->DevIntrf.StartRx = TWSPIStartRx;
	pDev->DevIntrf.RxData = TWSPIRxData;
	pDev->DevIntrf.StopRx = TWSPIStopRx;
	pDev->DevIntrf.StartTx = TWSPIStartTx;
	pDev->DevIntrf.TxData = TWSPITxData;
	pDev->DevIntrf.StopTx = TWSPIStopTx;
	pDev->DevIntrf.EvtCB = pCfgData->EvtCB;
	pDev->DevIntrf.Busy = false;
	pDev->DevIntrf.EnCnt = 1;
	pDev->DevIntrf.MaxRetry = pCfgData->MaxRetry;

	return true;
}




