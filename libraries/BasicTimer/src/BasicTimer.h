//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file  BasicTimer.h
//!  @brief Basic Timer Library Class Definitions
//!
//!  @author Nate Taylor \n
//!  Contact: nate@rtelectronix.com
//!  @copyright (C) 2020 Nate Taylor - All Rights Reserved.
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
//      |      Copyright (c) 2020 Nathaniel Taylor                                                |
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

#ifndef _BASIC_TIMERS_BASIC_TIMER_H_
#define _BASIC_TIMERS_BASIC_TIMER_H_

#include <Arduino.h>

/**
 * @brief Class that wraps millis() based timers for easier use.
 * 
 *  A BasicTimer makes it easy to perform an action after a certain amount of milliseconds has passed.
 */
class BasicTimer
{
    public:
        /**
         * @brief Construct a new TBasicTimer 
         * 
         * @param timeout The timers timeout in milliseconds
         */
        BasicTimer(unsigned long timeout = 500):  lastReset(0), storedTimeout(timeout){};

        /**
         * @brief Copy Constructor
         */
        BasicTimer(const BasicTimer& other): lastReset(other.lastReset),
            storedTimeout(other.storedTimeout){};


        /**
         *  @brief Prepares the timer for use, equivalent to reset()
         *  @see reset()
         */
        void begin(){ reset(); };

        /**
         *  @brief Prepares the timer for use and sets the timeout to the 
         *         supplied value
         * 
         *  @see reset()
         *  @see setTimeout()
         */
        void begin(unsigned long timeout) { 
            setTimeout(timeout);
            reset(); 
        };

        /**
         * @brief Resets the timer (so that it is no longer expired)
         */
        void reset(){  lastReset = now(); };

        /**
         * @brief Check if the timer has expired
         * 
         * @return true If the timer has expired
         * @return false If it has yet to expire
         */
        bool hasExpired() const
        {
            auto current = now();
            if (current -  lastReset > storedTimeout) return true;
            else return false;
        }

        /**
         * @brief Returns the timer's stored timeout time in milliseconds
         * 
         * @return unsigned long 
         */
        unsigned long timeout() const { return storedTimeout; };
        
        /**
         * @brief Get the current time in milliseconds
         * 
         * @return unsigned long The current time in milliseconds
         */
        static unsigned long now()
        {
            return millis();
        }

        /**
         * @brief The amount of time that has elapsed since the timer
         *        was last reset in milliseconds.
         * 
         * @return unsigned long The elapsed time in milliseconds
         */
        unsigned long elapsedTime() const {
            return (now() - lastReset);
        }

        /**
         * @brief Set the timer's timeout to the supplied value
         * 
         * @param timeout The new timer period in milliseconds
         */
        void setTimeout(unsigned long timeout){ storedTimeout = timeout; };

        /**
         * @brief Runs the timer, executing the supplied function after each timeout
         * 
         * @param callback The function to execute when the timer expires
         * @param resetOnExpire If true, the timer will reset every time 
         *                      (executing callback after every timeout period)
         */
        void whenExpired(void(*callback)(), bool resetOnExpire = false)
        {
            if(hasExpired())
            {
                reset();
                callback();
            }
        }

        /**
         * @brief Runs the timer, executing the supplied function after each timeout
         * 
         * @tparam ArgumentType Type of argument for the callback 
         *          (for template deduction)
         * @param callback  The function to execute when the timer expires
         * @param callbackArg Argument that will be supplied to the 
         *                    function when it expires
         */
        template<typename ArgumentType>
        void whenExpired(void(*callback)(ArgumentType arg), 
                         ArgumentType callbackArg)
        {
            if(hasExpired())
            {
                reset();
                callback(callbackArg);
            }
        }
    protected:
        unsigned long  lastReset; //!< The last timestamp at which the timer was reset
        unsigned long storedTimeout;//!< The timeout value in milliseconds
};

/**
 * @brief Template class for a timer with a static, constant Timeout value.
 *        This reduces the memory footprint of the class if you never want the
 *        timers time to change.
 * 
 * @tparam TIMEOUT The timeout in milliseconds
 */
template <unsigned long TIMEOUT>
class StaticTimer
{
    public:
        /**
         * @brief onExpire callback handler type
         */
        typedef void(*OnExpireFunction)();

        /**
         * @brief Construct a new TStaticTimer object
         * 
         */
        StaticTimer():  lastReset(0){};

        /**
         * @brief Reset the timer, setting its value to the current time
         * 
         */
        void reset(){  lastReset = now(); };

        /**
         * @brief Checks it the timer has expired
         * 
         * @return true If the timeout has passed
         * @return false If not.
         */
        bool hasExpired() const
        {
            if (elapsedTime() > TIMEOUT) return true;
            else return false;
        }

        /**
         * @brief Gets the currenet timestamp in milliseconds
         * 
         * @return unsigned long The current timestamp in milliseconds
         */
        unsigned long now()
        {
            return millis();
        }

        /**
         * @brief The amount of time that has elapsed since the timer
         *        was last reset in milliseconds.
         * 
         * @return unsigned long The elapsed time in milliseconds
         */
        unsigned long elapsedTime() const {
            return (now() - lastReset);
        }

        /**
         * @brief Runs the timer, executing the supplied function if the timer has expired.
         * 
         * @param callback The function to execute
         * @param resetOnExpire If true, the timer will reset every time 
         *                      (executing callback after every timeout period)
         */
        void whenExpired(OnExpireFunction callback, bool resetOnExpire = false)
        {
            if(hasExpired())
            {
                if (resetOnExpire) reset();
                callback();
            }
        }

        /**
         * @brief Runs the timer, executing the supplied function if the timer has expired.
         * 
         * @param callback The function to execute
         * @param resetOnExpire If true, the timer will reset every time 
         *                      (executing callback after every timeout period)
         */
        template<typename CallbackArgumentType>
        void whenExpired(OnExpireFunction callback, bool resetOnExpire = false)
        {
            if(hasExpired())
            {
                if (resetOnExpire) reset();
                callback();
            }
        }



    protected:
        unsigned long  lastReset;
};



#include "./BasicBlinker.h"
#include "./SwitchableTimer.h"
#include "./CallbackTimer.h"

#endif /* _BASIC_TIMERS_BASIC_TIMER_H_*/