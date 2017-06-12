/***********************************************************************************
* Copyright (c) 2010 - 2017, Motsai
* All rights reserved.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************/

#ifndef NRF52_SAADC_H
#define NRF52_SAADC_H

/**********************************************************************************/

#include "adc.h"

/**********************************************************************************/

#ifdef __cplusplus

class nRF52_SAADC : public ADC
{
public:
    virtual bool init( const adc_cfg_t& prCfg, const adc_dev_t& prDev ) override;

    /// Asynchronous non-blocking read.
    /// When read is complete, ADCEventHandler callback is called.
    virtual void read() override;

    /// Synchronuous blocking read.
    virtual bool read( uint8_t pChannel, int16_t& prValue ) override;

    virtual void uninit() override;
};

#endif // __cplusplus

/**********************************************************************************/

#endif // NRF52_SAADC_H
