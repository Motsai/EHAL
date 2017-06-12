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

#ifndef NRF5X_TIMER_H
#define NRF5X_TIMER_H

/**********************************************************************************/

#include "timer.h"

// Nordic
#include "app_timer.h"

/**********************************************************************************/

#define APP_TIMER_PRESCALER              0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE          4                                           /**< Size of timer operation queues. */

#ifdef __cplusplus

class NRF5XTimer : public Timer
{
public:
    /// Base constructor
    NRF5XTimer();

    /// Retrieve difference between two timer counter.
    ///
    /// \param  pCurrent        Current timer counter
    /// \param  pOld            Old timer counter
    ///
    /// \return Timer counter difference.
    virtual uint32_t getDiff( uint32_t pCurrent, uint32_t pOld ) override;

    /// Get timer frequency
    ///
    /// \return Frequency in Hz
    virtual uint32_t getFreq() override;

    /// Reset timer
    virtual void reset() override;

    /// Start timer
    virtual void start() override;

    /// Stop timer
    virtual void stop() override;

protected:
    /// Create the timer.
    ///
    /// \return True, if successful. False, otherwise.
    virtual bool create() override;

    /// Retrieve counter from derived class.
    virtual uint32_t getCounter() override;

private:
    app_timer_t mTimer;
    app_timer_t* mpTimer;       // Pointer to mTimer. (Nordic...)
    uint32_t mTicks;

    void init();
};

#endif // __cplusplus

/**********************************************************************************/

#endif // NRF5X_TIMER_H
