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

#include "saadc_nrf52.h"

extern "C" {
#include "nrf_drv_saadc.h"
}

/**********************************************************************************/

static adc_dev_t gsDev = { 0, };

static int16_t gsBuffer[2];

/**********************************************************************************/

void nRF52_SAADC_EventHandler( nrf_drv_saadc_evt_t const* ppEvent )
{
    switch ( ppEvent->type )
    {
    case NRF_DRV_SAADC_EVT_DONE:
        if ( gsDev.handler != nullptr )
        {
            gsDev.handler( *ppEvent->data.done.p_buffer );
        }
        break;
    }
}

/**********************************************************************************/

bool nRF52_SAADC::init( const adc_cfg_t& prCfg, const adc_dev_t& prDev )
{
    uint8_t i;
    uint32_t errCode;

    nrf_drv_saadc_config_t config;
    config.resolution = (nrf_saadc_resolution_t)prCfg.resolution;
    config.oversample = (nrf_saadc_oversample_t)prCfg.oversample;
    config.interrupt_priority = (uint8_t)prCfg.priority;

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

    errCode = nrf_drv_saadc_init( &config, nRF52_SAADC_EventHandler );
    if ( errCode != NRF_SUCCESS ) return false;

    errCode = nrf_drv_saadc_channel_init( prDev.channelNo, &channelConfig );
    if ( errCode != NRF_SUCCESS ) return false;

    for ( i = 0; i < 2; i++ )
    {
        errCode = nrf_drv_saadc_buffer_convert( &gsBuffer[i], 1 );
        if ( errCode != NRF_SUCCESS ) return false;
    }

    gsDev = prDev;

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

void nRF52_SAADC::uninit()
{
    nrf_drv_saadc_uninit();
    NRF_SAADC->INTENCLR = ( SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos );
    NVIC_ClearPendingIRQ( SAADC_IRQn );
}

/**********************************************************************************/


