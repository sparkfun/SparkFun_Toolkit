
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

#include <climits>
#include <cstdint>
#include <type_traits>
/**
    @brief Common include file for the core of the SparkFun Electronics Toolkit
*/
#include "sfeTkError.h"

// byte order types/enum
enum class sfeTKByteOrder : uint8_t
{
    BigEndian = 0x01,
    LittleEndian = 0x02
};

// Use a namespace for the toolkit "utilities and helpers"
namespace sfeToolkit
{
// Function to determine the byte order of the system
sfeTKByteOrder systemByteOrder(void);

// Method to swap the byte order of any unsigned integer - you pick the size. Uses constexpr so it's a compile time
// operation/inline/optimized
//
// from
// https://stackoverflow.com/questions/36936584/how-to-write-constexpr-swap-function-to-change-endianess-of-an-integer
//
template <class T>
constexpr typename std::enable_if<std::is_unsigned<T>::value, T>::type byte_swap(T i, T j = 0u, std::size_t n = 0u)
{
    return n == sizeof(T) ? j : byte_swap<T>(i >> CHAR_BIT, (j << CHAR_BIT) | (i & (T)(unsigned char)(-1)), n + 1);
}

}; // namespace sfeToolkit