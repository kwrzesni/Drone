//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file CallbackTimer.h 
//!  @brief CallbackTimer class definition
//!
//!  @author Nate Taylor 

//!  Contact: nate@rtelectronix.com
//!  @copyright (C) 2023  Nate Taylor - All Rights Reserved.
//
//      |-----------------------------------------------------------------------------------------|
//      |                                                                                         |
//      |                    MMMMMMMMMMMMMMMMMMMMMM   NNNNNNNNNNNNNNNNNN                          |
//      |                    MMMMMMMMMMMMMMMMMMMMMM   NNNNNNNNNNNNNNNNNN                          |
//      |                   MMMMMMMMM    MMMMMMMMMM       NNNNNMNNN                               |
//      |                   MMMMMMMM:    MMMMMMMMMM       NNNNNNNN                                |
//      |                  MMMMMMMMMMMMMMMMMMMMMMM       NNNNNNNNN                                |
//      |                 MMMMMMMMMMMMMMMMMMMMMM         NNNNNNNN                                 |
//      |                 MMMMMMMM     MMMMMMM          NNNNNNNN                                  |
//      |                MMMMMMMMM    MMMMMMMM         NNNNNNNNN                                  |
//      |                MMMMMMMM     MMMMMMM          NNNNNNNN                                   |
//      |               MMMMMMMM     MMMMMMM          NNNNNNNNN                                   |
//      |                           MMMMMMMM        NNNNNNNNNN                                    |
//      |                          MMMMMMMMM       NNNNNNNNNNN                                    |
//      |                          MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                |
//      |                        MMMMMMM      E L E C T R O N I X         MMMMMM                  |
//      |                         MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                    |
//      |                                                                                         |
//      |-----------------------------------------------------------------------------------------|
//
//      |-----------------------------------------------------------------------------------------|
//      |                                                                                         |
//      |      [MIT License]                                                                      |
//      |                                                                                         |
//      |      Copyright (c) 2023 Nathaniel Taylor                                                |
//      |                                                                                         |
//      |      Permission is hereby granted, free of charge, to any person obtaining a copy       |
//      |      of this software and associated documentation files (the "Software"), to deal      |
//      |      in the Software without restriction, including without limitation the rights       |
//      |      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell          |
//      |      copies of the Software, and to permit persons to whom the Software is              |
//      |      furnished to do so, subject to the following conditions:                           |
//      |                                                                                         |
//      |      The above copyright notice and this permission notice shall be included in all     |
//      |      copies or substantial portions of the Software.                                    |
//      |                                                                                         |
//      |      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR         |
//      |      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,           |
//      |      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE        |
//      |      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER             |
//      |      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,      |
//      |      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE      |
//      |      SOFTWARE.                                                                          |
//      |                                                                                         |
//      |-----------------------------------------------------------------------------------------|
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _BASIC_TIMER_CALLBACK_TIMER_H
#define _BASIC_TIMER_CALLBACK_TIMER_H

#include "./BasicTimer.h"

enum TimerRunMode: uint8_t {
    TIMER_RUN_MODE_ONE_SHOT = 0,
    TIMER_RUN_MODE_CONTINUOUS = 1
};

/**
 * @brief Class for A Basic Timer That Provides a callback on expire
 * 
 */
class CallbackTimer: public BasicTimer
{
    public:
        /**
         * @brief onExpire callback handler type
         */
        typedef void(*OnExpireFunction)();

        /**
         * @brief Construct a new BasicCallbackTimer object with the provided timeout and callback
         * 
         * @param timeout The timeout in milliseconds
         * @param callback The onExpire callback function
         */
        CallbackTimer(unsigned long timeout = 500, 
                           OnExpireFunction callback = nullptr, 
                           TimerRunMode mode = TIMER_RUN_MODE_ONE_SHOT): 
                                BasicTimer(timeout), 
                                expiredCallback(callback),
                                stateFlags(static_cast<uint8_t>(mode) & TimerModeMask)
                                {};

        /**
         * @brief Prepares the timer for use by setting the timeout and optionally 
         *        the timer mode
         * 
         * @param timeout The timeout in milliseconds
         * @param mode The timer mode - defaults to TIMER_RUN_MODE_ONE_SHOT
         */
        void prepare(unsigned long timeout, TimerRunMode mode = TIMER_RUN_MODE_ONE_SHOT) {
            setTimeout(timeout);
            setMode(mode);
            this->reset();
        }

        /**
         * @brief Starts or restarts the timer
         * 
         */
        void start() {
            this->reset();
            bitSet(stateFlags, StartFlagBit);
        }

        /**
         * @brief Prepares the timer for use by setting the timeout and optionally 
         *        the timer mode, then starts the timer
         * 
         * @param timeout The timeout in milliseconds
         * @param mode The timer mode - defaults to TIMER_RUN_MODE_ONE_SHOT
         */
        void begin(unsigned long timeout, TimerRunMode mode = TIMER_RUN_MODE_ONE_SHOT) {
            prepare(timeout, mode);
            start();
        }

        /**
         * @brief Starts the timer with previously set mode and timeout.  
         *        
         *        Provided as an alias to start() for Arduino library
         *        style compliance.
         */
        void begin() {
            start();
        }

        /**
         * @brief Assign a callback function to be called on time expire
         * 
         * @param callback The onExpire callback function
         */
        void onExpire(OnExpireFunction callback){ expiredCallback = callback; }

        /**
         * @brief Run the timer.
         * 
         *      This function should be called from the main loop 
         *      and it will automatically call the callback function
         *      when the timer expires.
         */
        void run()
        {
            if (hasExpired())
            {
                if (!hasPreviouslyExpired()) {
                    bitSet(stateFlags, ExpireFlagBit);
                    if (mode() == TIMER_RUN_MODE_CONTINUOUS) {
                        this->reset();
                    }
                    if (expiredCallback != nullptr) {
                        expiredCallback();
                    }
                } 
            }
        }

        /**
         * @brief Resets the timer.  
         * 
         *        The timeout period will start again with the callback
         *        called when it next expires
         * 
         */
        void reset() {
            BasicTimer::reset();
            bitClear(stateFlags, ExpireFlagBit);
        }

        /**
         * @brief Sets the timer mode.  
         * 
         * @param mode The TimerRunMode
         * @see TimerRunMode
         */
        void setMode(TimerRunMode mode) {
            uint8_t currentState = stateFlags & (~TimerModeMask);
            uint8_t newMode = mode & TimerModeMask;
            stateFlags = currentState | newMode;
        }

        /**
         * @brief Gets if the time has been started
         * 
         * @return bool
         */
        bool hasStarted() const {
            return (stateFlags & (1 << StartFlagBit));
        }

        /**
         * @brief Gets the current timer mode
         * 
         * @return TimerRunMode 
         */
        TimerRunMode mode() const {
            return static_cast<TimerRunMode>(stateFlags & TimerModeMask);
        }

        /**
         * @brief Stops/disables the timer
         * 
         */
        void stop() {
            bitClear(stateFlags, StartFlagBit);
        }

        /**
         * @brief Stops/disables the timer
         *        Alias for stop() provided for Arduino library 
         *        style compliance.
         */
        void end() {
            stop();
        }

        /**
         * @brief Checks if the time has expired previously
         * 
         * @return true if the timeout has reached expiration
         * @return false if not
         */
        bool hasPreviouslyExpired() const {
            return (stateFlags & (1 << ExpireFlagBit));
        }
    protected:
        /**
         * @brief Constexpr mask for getting/setting the timer mode from 
         *        stateFlags
         */
        static constexpr uint8_t TimerModeMask = 0x1;

        /**
         * @brief Bit position in stateFlags for the start flag
         */
        static constexpr uint8_t StartFlagBit = 6;

        /**
         * @brief Bit position in stateFlags for the expired flag
         */
        static constexpr uint8_t ExpireFlagBit = 7;

        /**
         * @brief State storage as bit flags to reduce memory footprint
         * 
         */
        uint8_t stateFlags;

        /**
         * @brief On expire callback pointer storage
         */
        OnExpireFunction expiredCallback;

        
        /**
         * @brief Checks if the timer has expired.  The timer must
         *        have been started to have expired.
         * 
         * @return bool True if expired, false if not
         * 
         */
        bool hasExpired() {
            if (hasStarted()) {
                return BasicTimer::hasExpired();
            } else {
                return false;
            }
        }
};

#endif /* _BASIC_TIMER_CALLBACK_TIMER_H */
