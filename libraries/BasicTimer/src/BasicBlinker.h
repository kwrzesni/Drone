//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file BasicBlinker.h 
//!  @brief BasicBlinker class definitions
//!
//!  @author Nate Taylor 

//!  Contact: nate@rtelectronix.com
//!  @copyright (C) 2020  Nate Taylor - All Rights Reserved.
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

#ifndef _BASIC_TIMER_BASIC_BLINKER_H_
#define _BASIC_TIMER_BASIC_BLINKER_H_

#include "./BasicTimer.h"

/**
 * @brief Class that toggles a boolean value at a constant time interval.
 * 
 *    Helpful for blinking LED's at a consistant time (or anything really)
 */
class BasicBlinker
{
    public:
        /**
         * @brief Construct a new BasicBlinker with the default time (500 ms)
         */
        BasicBlinker(): timer(500){};

        /**
         * @brief Construct a new BasicBlinker object with the supplied time
         * 
         * @param timeout The blink time in milliseconds
         */
        BasicBlinker(uint32_t timeout): timer(timeout){};

        /**
         * @brief Set the blink time for the blinker, which is the amount of time
         *        the blinker is on or off (one-half the period).
         * 
         * @param blinkTime The new time in milliseconds
         */
        void setBlinkTime(uint32_t blinkTime)
        {
            timer.setTimeout(blinkTime);
        }


        /**
         * @brief Returns the set blink time, which is the amount of time
         *        the blinker is on or off (one-half the period).
         * 
         * @return uint32_t 
         */
        uint32_t blinkTime() const 
        {
            return timer.timeout();
        }

        /**
         * @brief Update the state of the blinker based on elapsed time.
         *        Should be called at least every loop.
         * 
         */
        void run()
        {
            if (timer.hasExpired())
            {
                timer.reset();
                state = !state;
            }
        }

        /**
         * @brief Updates the state of the blinker based on elapsed time, 
         *        returning the current blink state as a result.
         * 
         * @return The current blinker state (true or false)
         */
        bool update()
        {
            run();
            return state;
        }

        /**
         * @brief Reset the blinker to its initial state (false)
         */
        void reset()
        {
            state = false;
            timer.reset();
        }

        /**
         * @brief Get the current state of the blinker (does not update the blinker)
         * 
         * @return The current blinker state (true or false)
         */
        bool getState() const { return state; };

        /**
         * @brief Implicit casting to boolean gets the blinker state.
         * 
         *  This allows one to do if (BasicBlinker){} or pass it as a boolean argument
         * 
         * @return The current blinker state (true or false)
         */
        operator bool() const { return state; };

        /**
         * @brief Sets the boolean state of the blinker by assignment
         * 
         * @param newState The new boolean state of the blinker
         * @return BasicBlinker& The current object
         */
        BasicBlinker& operator=(bool newState)
        {
            state = newState;
            return *this;
        }
    protected:
        BasicTimer timer;
        bool state;
};

/**
 * @brief Template class that toggles a boolean value at a static time interval determined at compile time.
 * 
 *     Helpful for blinking LED's at a consistant time (or anything really)
 * 
 * @tparam TIMEOUT The blink time in milliseconds.
 */
template<unsigned int TIMEOUT>
class StaticBlinker
{
    public:
        /**
         * @brief Construct a new StaticBlinker
         */
        StaticBlinker<TIMEOUT>(): timer(){};

        /**
         * @brief Update the state of the blinker based on elapsed tiem.
         * 
         */
        void run()
        {
            if (timer.hasExpired())
            {
                timer.reset();

                state = !state;
            }
        }

        /**
         * @brief Update the state of the blinker based on elapsed tiem.
         * 
         * @return The current blinker state (true or false)
         */
        bool update()
        {
            run();
            return state;
        }

        /**
         * @brief Reset the blinker to its initial state (false)
         */
        void reset()
        {
            state = false;
            timer.reset();
        }

        /**
         * @brief Get the current state of the blinker (does not update the blinker)
         * 
         * @return The current blinker state (true or false)
         */
        bool getState() const { return state; };

        /**
         * @brief Implicit casting to boolean gets the blinker state.
         * 
         *  This allows one to do if (BasicBlinker){} or pass it as a boolean argument
         * 
         * @return The current blinker state (true or false)
         */
        operator bool() const { return state; };

        /**
         * @brief Sets the boolean state of the blinker by assignment
         * 
         * @param newState The new boolean state of the blinker
         * @return StaticBlinker& The current object
         */
        StaticBlinker& operator=(bool newState)
        {
            state = newState;
            return *this;
        }

        /**
         * @brief Returns the set blink time, which is the amount of time
         *        the blinker is on or off (one-half the period).
         * 
         * @return uint32_t 
         */
        static constexpr uint32_t blinkTime()
        {
            return TIMEOUT;
        }
    protected:
        /**
         * @brief The blinkers underlying timer
         */
        StaticTimer<TIMEOUT> timer;
        /**
         * @brief The stored blinker state
         */
        bool state;
};

/**
 * @brief Extension of BasicBlinker that can be turned on and off.
 *        A disabled blinker will always be set to true.
 * 
 * @see BasicBlinker
 */
class SwitchableBlinker: public BasicBlinker 
{
    public:
        /**
         * @brief Construct a new SwitchableBlinker with the supplied timeout (default = 500)
         * 
         * @param blinkTime The new blink time in milliseconds.
         */
        SwitchableBlinker(uint32_t blinkTime = 500): BasicBlinker(blinkTime){};

        /**
         * @brief Check if the blinker is enabled.
         * 
         * @return true if enabled/
         * @return false if disabled.
         */
        bool isEnabled() const { return enabled; };

        /**
         * @brief Enables the blinker (blinker will toggle)
         */
        void enable(bool shouldEnable = true){ enabled = shouldEnable; };

        /**
         * @brief Disables the blinker (blinker will always be true)
         */
        void disable(){ enable(false); };


        /**
         * @brief Toggles the blinker's enabled state (if enabled, disables it
         *        and vice-versa).
         */
        void toggleEnabled() { enable(!enabled); };

        /**
         * @brief Get the current state of the blinker (does not update the blinker)
         * 
         * @return The current blinker state (true or false)
         */
        bool getState() const
        {
            if (enabled) return state;
            else return true;
        }

        /**
         * @brief Implicit casting to boolean gets the blinker state.
         * 
         *  This allows one to do if (BasicBlinker){} or pass it as a boolean argument
         * 
         * @return The current blinker state (true or false)
         */
        operator bool() const { return getState(); };

        /**
         * @brief Turns the blinker on and off by assignment
         * 
         * @param newState The new boolean state of the blinker
         * @return SwitchableBlinker& The current object
         */
        SwitchableBlinker& operator=(bool newState)
        {
            enabled = newState;
            return *this;
        }
    protected:
        /**
         * @brief Blinker enabled state memory
         */
        bool enabled;
};

/**
 * @brief Extension of StaticBlinker that can be turned on and off.
 *        A disabled blinker will awalys be set to true.
 * 
 * @see StaticBlinker
 * @tparam TIMEOUT The fixed blink time in milliseconds.
 */
template<unsigned int TIMEOUT>
class StaticSwitchableBlinker: public StaticBlinker<TIMEOUT> 
{
    public:
        /**
         * @brief Construct a new SwitchableBlinker with the supplied timeout (default = 500)
         * 
         * @param blinkTime The new blink time in milliseconds.
         */
        StaticSwitchableBlinker(): StaticBlinker<TIMEOUT>(){};

        /**
         * @brief Check if the blinker is enabled.
         * 
         * @return true if enabled/
         * @return false if disabled.
         */
        bool isEnabled(){ return enabled; };

        /**
         * @brief Enables the blinker (blinker will toggle)
         */
        void enable(){ enabled = true; };

        /**
         * @brief Disables the blinker (blinker will always be true)
         */
        void disabled(){ enabled = false; };

        /**
         * @brief Get the current state of the blinker (does not update the blinker)
         * 
         * @return The current blinker state (true or false)
         */
        bool getState()
        {
            if (enabled) return this->state;
            else return true;
        }

        /**
         * @brief Implicit casting to boolean gets the blinker state.
         * 
         *  This allows one to do if (BasicBlinker){} or pass it as a boolean argument
         * 
         * @return The current blinker state (true or false)
         */
        operator bool(){ return getState(); };

        /**
         * @brief Turns the blinker on and off by assignment
         * 
         * @param newState The new boolean state of the blinker
         * @return SwitchableBlinker& The current object
         */
        StaticSwitchableBlinker& operator=(bool newState)
        {
            enabled = newState;
        }
    protected:
        /**
         * @brief Blinker enabled state memory
         */
        bool enabled;
};


#endif /* _BASIC_TIMER_BASIC_BLINKER_H_ */