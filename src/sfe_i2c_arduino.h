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

#include "sfe_i2c.h"
#include <Wire.h>

/// @brief An I2C communication bus implementation for Arduino
class SFE_I2C_Arduino : public SFE_I2C
{
  public:
    /// @brief Initialize I2C parameters.
    /// @param devAddr I2C address of device.
    /// @return 0 for success, negative for failure, positive for warning.
    int8_t init(uint8_t devAddr);

    /// @brief Initialize I2C parameters.
    /// @param devAddr I2C address of device.
    /// @param i2cPort Arduino Wire port to use.
    /// @param beginBus Whether to initialize the I2C bus.
    /// @return 0 for success, negative for failure, positive for warning.
    int8_t init(uint8_t devAddr, TwoWire& i2cPort, bool beginBus = false);

    /// @brief Pings I2C device and looks for an ACK response.
    /// @return 0 for success, negative for failure, positive for warning.
    virtual int8_t ping();

    /// @brief Writes a number of bytes starting at the given register address.
    /// @param regAddr The first register address to write to.
    /// @param data Data buffer to write to registers.
    /// @param numBytes Number of bytes to write.
    /// @return 0 for success, negative for failure, positive for warning.
    virtual int8_t writeRegisters(uint8_t regAddr, const uint8_t *data, uint8_t numBytes);

    /// @brief Reads a number of bytes starting at the given register address.
    /// @param regAddr The first register address to read from.
    /// @param data Data buffer to read from registers.
    /// @param numBytes Number of bytes to read.
    /// @return 0 for success, negative for failure, positive for warning.
    virtual int8_t readRegisters(uint8_t regAddr, uint8_t *data, uint8_t numBytes);

  protected:
    TwoWire* _i2cPort;
};
