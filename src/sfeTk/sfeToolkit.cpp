// File: sfeToolkit.cpp
//
// General impl file for the SparkFun Toolkit
/*

The MIT License (MIT)

Copyright (c) 2024 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions: The
above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
"AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "sfeToolkit.h"

// THIS IS A PLACEHOLDER FILE for now
#ifdef ARDUINO
#include <Arduino.h>
#endif
//---------------------------------------------------------------------------------
/**
 * @brief C function - Runtime check for system byte order
 */
sfeTKByteOrder sfeToolkit::systemByteOrder(void)
{
    uint16_t i = 1;
    return *((uint8_t *)&i) == 0 ? sfeTKByteOrder::BigEndian : sfeTKByteOrder::LittleEndian;
}

//---------------------------------------------------------------------------------
/**
 * @brief to catch 8 bit calls for byte swap
 *
 */
uint8_t sfeToolkit::byte_swap(uint8_t i)
{
    return i;
}
//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap a 16 bit value
 */
uint16_t sfeToolkit::byte_swap(uint16_t i)
{
    // Use the fast intrinsic if available
#if defined(__clang__) || defined(__GNUC__)
    return __builtin_bswap16(i);
#else
    return (i << 8) | (i >> 8);
#endif
}

//---------------------------------------------------------------------------------
/**
 * @brief function - Byte swap a 32 bit value
 */
uint32_t sfeToolkit::byte_swap(uint32_t i)
{
#if defined(__clang__) || defined(__GNUC__)
    return __builtin_bswap32(i);
#else
    return ((i << 24) & 0xff000000) | ((i << 8) & 0x00ff0000) | ((i >> 8) & 0x0000ff00) | ((i >> 24) & 0x000000ff);
#endif
}

void sfeToolkit::delay_ms(uint32_t ms)
{
    // right now we use the Arduino delay function - future we need to abstract out a driver

#if defined(ARDUINO)
    delay(ms);
#else
#error // we need to implement a delay function
// we need to implement a delay function
#endif
}

uint32_t sfeToolkit::ticks_ms(void)
{
#if defined(ARDUINO)
    return millis();
#else
#error // we need to implement a delay function
// we need to implement a delay function
#endif
    return 0;
}