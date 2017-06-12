/***********************************************************************************
* Copyright (c) 2010 - 2016, Motsai
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

#include "nrf5x_timer.h"

#include "nrf_drv_clock.h"
#include "app_timer.h"

#include <assert.h>

/**********************************************************************************/

NRF5XTimer::NRF5XTimer():
    mTimer(),
    mpTimer( &mTimer )
{

}

/**********************************************************************************/

bool NRF5XTimer::create()
{
    uint32_t lErrCode;

    init();

    app_timer_t* lpTimer = &mTimer;
    if ( getConfig().mode == TIMER_MODE_SINGLE_SHOT )
    {
        lErrCode = app_timer_create( &mpTimer,
                                     APP_TIMER_MODE_SINGLE_SHOT,
                                     getConfig().callback );
    }
    else if ( getConfig().mode == TIMER_MODE_REPEATED )
    {
        lErrCode = app_timer_create( &mpTimer,
                                     APP_TIMER_MODE_REPEATED,
                                     getConfig().callback );
    }
    else
    {
        assert( false );
    }

    mTicks = (uint64_t)getConfig().period * (uint64_t)getFreq() / ( ( APP_TIMER_PRESCALER + 1 ) * 1000000 );
    setTickPeriod( getConfig().period / mTicks );

    return ( lErrCode == NRF_SUCCESS );
}

/**********************************************************************************/

uint32_t NRF5XTimer::getCounter()
{
    return app_timer_cnt_get();
}

/**********************************************************************************/

uint32_t NRF5XTimer::getDiff( uint32_t pCurrent, uint32_t pOld )
{
    uint32_t lDiff;
    lDiff = app_timer_cnt_diff_compute( pCurrent, pOld);
    return lDiff;
}

/**********************************************************************************/

uint32_t NRF5XTimer::getFreq()
{
    return APP_TIMER_CLOCK_FREQ;
}

/**********************************************************************************/

void NRF5XTimer::init()
{
    uint32_t err_code = nrf_drv_clock_init();
    err_code = app_timer_init();
    //APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, nullptr);
}

/**********************************************************************************/

void NRF5XTimer::reset()
{
    stop();
    init();
    start();
}

/**********************************************************************************/

void NRF5XTimer::start()
{
    Timer::start();
    uint32_t lErrCode = app_timer_start( mpTimer, mTicks, getConfig().context );
}

/**********************************************************************************/

void NRF5XTimer::stop()
{
    uint32_t lErrCode = app_timer_stop( mpTimer );
}

/**********************************************************************************/


