/*
The MIT License (MIT)

Copyright (c) 2022 SparkFun Electronics
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

// Error and warning codes
#define SFE_BUS_OK 0
#define SFE_BUS_E_UNKNOWN -1
#define SFE_BUS_E_NULL_PTR -2
#define SFE_BUS_E_TIMEOUT -3
#define SFE_BUS_E_NO_RESPONSE -4
#define SFE_BUS_E_DATA_TOO_LONG -5
#define SFE_BUS_W_UNKNOWN 1
#define SFE_BUS_W_UNDER_READ 1

/// @brief An abstract interface for a communication bus
class SFE_Bus
{
  public:
    /// @brief Writes a number of bytes starting at the given register address.
    /// @param regAddr The first register address to write to.
    /// @param data Data buffer to write to registers.
    /// @param numBytes Number of bytes to write.
    /// @return 0 for success, negative for failure, positive for warning.
    virtual int8_t writeRegisters(uint8_t regAddr, const uint8_t *data, uint8_t numBytes) = 0;

    /// @brief Reads a number of bytes starting at the given register address.
    /// @param regAddr The first register address to read from.
    /// @param data Data buffer to read from registers.
    /// @param numBytes Number of bytes to read.
    /// @return 0 for success, negative for failure, positive for warning.
    virtual int8_t readRegisters(uint8_t regAddr, uint8_t *data, uint8_t numBytes) = 0;
};
