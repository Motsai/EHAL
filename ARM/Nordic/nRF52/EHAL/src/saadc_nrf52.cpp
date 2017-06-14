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

#include "saadc_nrf52.h"

extern "C" {
#include "nrf_drv_saadc.h"
}

/**********************************************************************************/

#define     NRF52_SAADC_COUNT           8

/**********************************************************************************/

/*static adc_dev_t gsDev[NRF52_SAADC_COUNT] = { { 0, nullptr },
                                              { 1, nullptr },
                                              { 2, nullptr },
                                              { 3, nullptr },
                                              { 4, nullptr },
                                              { 5, nullptr },
                                              { 6, nullptr },
                                              { 7, nullptr } };*/

static nrf_saadc_value_t gsBuffer[2][NRF52_SAADC_COUNT];

/**********************************************************************************/

void nRF52_SAADC_EventHandler( nrf_drv_saadc_evt_t const* ppEvent )
{
    static uint32_t lCount = 0;
    switch ( ppEvent->type )
    {
    case NRF_DRV_SAADC_EVT_DONE:
        lCount++;
        nrf_drv_saadc_buffer_convert(ppEvent->data.done.p_buffer, NRF52_SAADC_COUNT);

        /*if ( gsDev.handler != nullptr ) {
            gsDev.handler( *ppEvent->data.done.p_buffer );
        }*/
        break;
    }
}

/**********************************************************************************/

void nRF52_SAADC::disable()
{
    nrf_saadc_task_trigger( NRF_SAADC_TASK_STOP );
    nrf_saadc_disable();
    for ( uint32_t i = 0; i < mChannelCount; ++i ) {
        nrf_saadc_channel_input_set( i, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED );
    }
}

/**********************************************************************************/

void nRF52_SAADC::disable( uint8_t pChannelNo )
{
    nrf_saadc_channel_input_set( pChannelNo, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED );
}

/**********************************************************************************/

void nRF52_SAADC::enable()
{
    init();
}

/**********************************************************************************/

void nRF52_SAADC::enable( uint8_t pChannelNo )
{
    if ( pChannelNo < mChannelCount ) {
        initChannel( mpChannelCfg[pChannelNo] );
    }
}

/**********************************************************************************/

bool nRF52_SAADC::init()
{
    uint32_t lErrCode;

    nrf_drv_saadc_config_t config;
    config.resolution = (nrf_saadc_resolution_t)mpCfg->resolution;
    config.oversample = (nrf_saadc_oversample_t)mpCfg->oversample;
    config.interrupt_priority = (uint8_t)mpCfg->priority;
    config.low_power_mode = true;

    lErrCode = nrf_drv_saadc_init( &config, nRF52_SAADC_EventHandler );
    if ( lErrCode != NRF_SUCCESS ) return false;

    for ( int i = 0; i < mChannelCount; i++ ) {
        initChannel( mpChannelCfg[i] );
    }

    for ( int i = 0; i < 2; i++ )
    {
        uint32_t lErrCode = nrf_drv_saadc_buffer_convert( gsBuffer[i], NRF52_SAADC_COUNT );
        if ( lErrCode != NRF_SUCCESS ) return false;
    }

    return true;
}

/**********************************************************************************/

bool nRF52_SAADC::initChannel( const adc_channel_cfg_t& prCfg )
{
    uint32_t lErrCode;

    nrf_saadc_channel_config_t channelConfig;
    channelConfig.acq_time = (nrf_saadc_acqtime_t)prCfg.acquisitionTime;
    channelConfig.burst = (nrf_saadc_burst_t)prCfg.burst;
    channelConfig.gain = (nrf_saadc_gain_t)prCfg.gain;
    channelConfig.mode = (nrf_saadc_mode_t)prCfg.mode;
    channelConfig.pin_n = (nrf_saadc_input_t)prCfg.pinN;
    channelConfig.pin_p = (nrf_saadc_input_t)prCfg.pinP;
    channelConfig.reference = (nrf_saadc_reference_t)prCfg.reference;
    channelConfig.resistor_n = (nrf_saadc_resistor_t)prCfg.resistorN;
    channelConfig.resistor_p = (nrf_saadc_resistor_t)prCfg.resistorP;

    lErrCode = nrf_drv_saadc_channel_init( prCfg.channelNo, &channelConfig );
    if ( lErrCode != NRF_SUCCESS ) return false;

    return true;
}

/**********************************************************************************/

bool nRF52_SAADC::init( const adc_cfg_t* ppCfg, const adc_channel_cfg_t* ppChannel, uint32_t pChannelCount )
{
    mpCfg = ppCfg;
    mpChannelCfg = ppChannel;
    mChannelCount = pChannelCount;

    return true;
}

/**********************************************************************************/

void nRF52_SAADC::read()
{
    nrf_drv_saadc_sample();
}

/**********************************************************************************/

bool nRF52_SAADC::read( uint8_t pChannel, int16_t& prValue )
{
    return nrf_drv_saadc_sample_convert( pChannel, &prValue );
}

/**********************************************************************************/


