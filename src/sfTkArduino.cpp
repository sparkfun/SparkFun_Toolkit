/**
 * @file sfTkArduino.cpp
 * @brief Implementation file for the SparkFun Toolkit Arduino layer
 *
 * This file contains the implementation for the SparkFun Toolkit library Arduino layer.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>

// Implements the sfToolkit functions for the Arduino platform

void sftk_delay_ms(uint32_t ms)
{
    if (ms == 0)
        return;
    delay(ms);
}

// published max us delay for delayMicroseconds() on Arduino
static const uint32_t MAX_DELAY_US = 16383;

void sftk_delay_us(uint32_t us)
{
    // no tick, not dice
    if (us == 0)
        return;

    // Can the built in just handle it?
    if (us <= MAX_DELAY_US)
        delayMicroseconds((unsigned int)us);
    else
    {
        // break up microseconds into milliseconds and the remaining microseconds
        delay(us / 1000);              // whole ms
        unsigned int rest = us % 1000; // always < 1000, safe cast
        if (rest)
            delayMicroseconds(rest);
    }
}

uint32_t sftk_ticks_ms(void)
{
    return millis();
}