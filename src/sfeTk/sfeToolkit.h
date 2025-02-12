
// sfeToolkit.h
//
// General header file for the SparkFun Toolkit
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

#pragma once

#include <stdint.h>

/**
    @brief Common include file for the core of the SparkFun Electronics Toolkit
*/
#include "sfeTkError.h"
#include "sfeTkIBus.h"

// byte order types/enum
enum class sfeTKByteOrder : uint8_t
{
    BigEndian = 0x01,
    LittleEndian = 0x02
};

// Note - toolkit *functions* start with sftk_ to avoid name collisions

// Function to determine the byte order of the system
sfeTKByteOrder sftk_system_byteorder(void);

uint8_t sftk_byte_swap(uint8_t i);
uint16_t sftk_byte_swap(uint16_t i);
uint32_t sftk_byte_swap(uint32_t i);


// Area for platform specific implementations. The interface/functions are 
// defined here, with the expectation that the platform provides the implementation.

// delay in milliseconds
void sftk_delay_ms(uint32_t ms);

// ticks in milliseconds
uint32_t sftk_ticks_ms(void);