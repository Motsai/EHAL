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

#include "esb_nrf52.h"

extern "C" {
#include "nrf.h"
#include "nrf_error.h"
#include "nrf_esb.h"
#include "nrf_esb_error_codes.h"
}

#include <assert.h>

/**********************************************************************************/



/**********************************************************************************/

bool nRF52_ESB::init( const esb_cfg_t& prCfg, const esb_dev_t& prDev )
{
    uint32_t lErrCode;

    mDev = prDev;

    nrf_esb_config_t lConfig = NRF_ESB_DEFAULT_CONFIG;

    switch ( prCfg.tx_power ) {
    case ESB_TX_POWER_4DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_4DBM;
        break;
    case ESB_TX_POWER_0DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_0DBM;
        break;
    case ESB_TX_POWER_NEG4DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG4DBM;
        break;
    case ESB_TX_POWER_NEG8DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG8DBM;
        break;
    case ESB_TX_POWER_NEG12DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG12DBM;
        break;
    case ESB_TX_POWER_NEG16DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG16DBM;
        break;
    case ESB_TX_POWER_NEG20DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG20DBM;
        break;
    case ESB_TX_POWER_NEG30DBM:
        lConfig.tx_output_power = NRF_ESB_TX_POWER_NEG30DBM;
        break;
    default:
        assert( false );
        break;
    }

    switch ( prCfg.bitrate ) {
    case ESB_BITRATE_2MBPS:
        lConfig.bitrate = NRF_ESB_BITRATE_2MBPS;
        break;
    case ESB_BITRATE_1MBPS:
        lConfig.bitrate = NRF_ESB_BITRATE_1MBPS;
        break;
    case ESB_BITRATE_250KBPS:
        lConfig.bitrate = NRF_ESB_BITRATE_250KBPS;
        break;
    default:
        assert( false );
        break;
    }

    switch ( prCfg.mode ) {
    case ESB_MODE_RX:
        lConfig.mode = NRF_ESB_MODE_PRX;
        break;
    case ESB_MODE_TX:
        lConfig.mode = NRF_ESB_MODE_PTX;
        break;
    default:
        assert( false );
        break;
    }

    lErrCode = nrf_esb_init( &lConfig );
    if ( lErrCode != NRF_SUCCESS ) return false;

    lErrCode = nrf_esb_set_base_address_0( &prCfg.address_base[0] );
    if ( lErrCode != NRF_SUCCESS ) return false;

    lErrCode = nrf_esb_set_base_address_1( &prCfg.address_base[4] );
    if ( lErrCode != NRF_SUCCESS ) return false;

    lErrCode = nrf_esb_set_prefixes( prCfg.address_prefix, 8);
    if ( lErrCode != NRF_SUCCESS ) return false;

    return true;
}

/**********************************************************************************/


