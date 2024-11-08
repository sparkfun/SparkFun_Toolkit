
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

/**
    @brief Common include file for the core of the SparkFun Electronics Toolkit
*/
#include "sfeTkError.h"

/**
    @brief The byte order of the system
*/
typedef enum
{
    SFETK_BIG_ENDIAN = 0x01,
    SFETK_LITTLE_ENDIAN = 0x00
} sfeTKByteOrder_t;

// Export our byte order function as a C function
#ifdef __cplusplus
extern "C"
{
#endif
    // Runtime check for system byte order
    sfeTKByteOrder_t systemByteOrder(void);
#ifdef __cplusplus
}
#endif
