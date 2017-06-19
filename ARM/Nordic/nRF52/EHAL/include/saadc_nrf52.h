/*--------------------------------------------------------------------------
File   : saadc_nrf52.h

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

#ifndef NRF52_SAADC_H
#define NRF52_SAADC_H

/**********************************************************************************/

#include "adc.h"

/**********************************************************************************/

#ifdef __cplusplus

class nRF52_SAADC : public ADC
{
public:
    virtual void disable() override;
    virtual void disable( uint8_t pChannelNo );

    virtual void enable() override;
    virtual void enable( uint8_t pChannelNo );

    virtual bool init( const adc_cfg_t* prCfg, const adc_channel_cfg_t* ppChannel, uint32_t pChannelCount ) override;

    /// Asynchronous non-blocking read.
    /// When read is complete, ADCEventHandler callback is called.
    virtual void read() override;

    /// Synchronuous blocking read.
    virtual bool read( uint8_t pChannel, int16_t& prValue ) override;

private:
    const adc_cfg_t* mpCfg;
    const adc_channel_cfg_t* mpChannelCfg;
    uint32_t mChannelCount;

    bool init();
    bool init( uint8_t pChannel );
    bool initChannel( const adc_channel_cfg_t& prCfg );
};

#endif // __cplusplus

/**********************************************************************************/

#endif // NRF52_SAADC_H
