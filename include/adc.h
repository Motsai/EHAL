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

#ifndef ADC_H
#define ADC_H

/**********************************************************************************/

#include <stdint.h>

/**********************************************************************************/

typedef void (*adc_event_handler_t)( const int16_t* const ppValue, uint8_t pSize );

typedef struct {
    uint32_t resolution;
    uint32_t oversample;
    uint32_t priority;
    adc_event_handler_t event_handler;
} adc_cfg_t;

typedef struct {
    uint8_t channelNo;
    uint32_t acquisitionTime;
    uint32_t resistorP;
    uint32_t resistorN;
    uint32_t gain;
    uint32_t reference;
    uint32_t mode;
    uint32_t burst;
    uint32_t pinP;
    uint32_t pinN;
} adc_channel_cfg_t;

/**********************************************************************************/

#ifdef __cplusplus

class ADC
{
public:
    virtual void disable() = 0;
    virtual void enable() = 0;

    virtual bool init( const adc_cfg_t* ppCfg, const adc_channel_cfg_t* ppChannel, uint32_t pChannelCount ) = 0;

    /// Asynchronous non-blocking read.
    /// When read is complete, ADCEventHandler callback is called.
    virtual void read() = 0;

    /// Synchronuous blocking read.
    virtual bool read( uint8_t pChannel, int16_t& prValue ) = 0;
};

#endif // __cplusplus

/**********************************************************************************/

#endif // ADC_H
