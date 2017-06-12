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

#ifndef NRF52_ESB_H
#define NRF52_ESB_H

/**********************************************************************************/

#include <stdint.h>

/**********************************************************************************/

typedef void ( *ESBEventHandler )( void );

typedef enum {
    ESB_MODE_RX,
    ESB_MODE_TX
} esb_mode_t;

typedef enum {
    ESB_BITRATE_2MBPS,
    ESB_BITRATE_1MBPS,
    ESB_BITRATE_250KBPS
} esb_bitrate_t;

typedef enum {
    ESB_TX_POWER_4DBM,
    ESB_TX_POWER_0DBM,
    ESB_TX_POWER_NEG4DBM,
    ESB_TX_POWER_NEG8DBM,
    ESB_TX_POWER_NEG12DBM,
    ESB_TX_POWER_NEG16DBM,
    ESB_TX_POWER_NEG20DBM,
    ESB_TX_POWER_NEG30DBM
} esb_tx_power_t;

typedef struct {
    bool acknowledge;
    uint8_t address_base[8];
    uint8_t address_prefix[8];
    esb_bitrate_t   bitrate;
    esb_mode_t  mode;
    esb_tx_power_t tx_power;
} esb_cfg_t;

typedef struct {
    ESBEventHandler handler;
} esb_dev_t;

/**********************************************************************************/

#ifdef __cplusplus

class nRF52_ESB
{
public:
    bool init( const esb_cfg_t& prCfg, const esb_dev_t& prDev );
    void uninit();

    uint32_t read( uint8_t* ppData, uint32_t pSize );
    uint32_t write( const uint8_t* const ppData, uint32_t pSize );

    void flushRx();
    void flushTx();

private:
    esb_dev_t mDev;
};

#endif // __cplusplus

/**********************************************************************************/

#endif // NRF52_ESB_H
