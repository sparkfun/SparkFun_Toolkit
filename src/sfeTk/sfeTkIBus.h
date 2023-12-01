
// sfeTkIBus.h
//
// Defines the communication bus interface for the SparkFun Electronics Toolkit -> sfeTk
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

#include <cstdint>

class sfeTkBus
{
  public:
    ///
    /// @brief A simple ping of the device at the given address.
    /// @param devAddr Address/Pin of the device
    ///
    /// @retval bool - true on success, false on failure
    ///
    virtual bool ping(uint8_t devAddr) = 0;

    /// @brief Write a single byte to the given register
    ///
    /// @param devAddr The device's address/pin -- depending on implementation
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval bool -  true on successful execution.
    ///
    virtual bool writeRegisterByte(uint8_t devAddr, uint8_t devReg, uint8_t data) = 0;

    /// @brief Writes a number of bytes starting at the given register's address.
    ///
    /// @param devAddr The device's address/pin
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval int returns the number of bytes written, < 0 on error
    ///
    virtual int writeRegisterRegion(uint8_t devAddr, uint8_t devReg, const uint8_t *data, uint16_t length) = 0;

    /// @brief Reads a block of data from the given register.
    ///
    /// @param devAddr The device's I2C address.
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval int returns 0 on success, or error code
    ///
    virtual int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t numBytes) = 0;
};

//};
