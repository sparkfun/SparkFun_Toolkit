/*
sfeTkBusI2c.h

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
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

The following classes specify the behavior for communicating
over Inter-Integrated Circuit (I2C) in Arduino

*/

#pragma once

#include <Wire.h>

// Include our Bus interface definition.
#include <sfeTk/sfeTkIBus.h>

/// @brief
/// The sfeTkBusI2C implements an sfeTkIBus interface, defining the Arduino implementation for I2C in the Toolkit
///
class sfeTkBusI2C : public sfeTkIBus
{
  public:
    ///
    /// @brief Constructor
    ///
    sfeTkBusI2C(void) : _i2cPort(nullptr){};

    /// @brief Method sets up the required I2C settings.
    /// @note This function provides a default I2C Port.
    ///
    /// @retval True on successful execution.
    ///
    bool init();

    /// @brief Method sets up the required I2C settings.
    ///
    /// @param wirePort Port for I2C communication.
    /// @param bInit This flag tracks whether the bus has been initialized.
    ///
    /// @retval True on successful execution.
    ///
    bool init(TwoWire &wirePort, bool bInit = false);

    /// @brief A simple ping of the device at the given address.
    /// @note sfeTkIBus interface method
    ///
    /// @param devAddr Address of the device
    ///
    /// @retval bool - true on success, false on error
    ///
    bool ping(uint8_t devAddr);

    /// @brief Write a single byte to the given register
    /// @note sfeTkIBus interface method
    ///
    /// @param devAddr The device's I2C address.
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval returns true on successful false on failure.
    bool writeRegisterByte(uint8_t devAddr, uint8_t devReg, uint8_t data);

    /// @brief Writes a number of bytes starting at the given register's address.
    /// @note sfeTkIBus interface method
    ///
    /// @param devAddr The device's I2C address.
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval returns number of bytes written, < 0 is an error code
    ///
    int writeRegisterRegion(uint8_t devAddr, uint8_t devReg, const uint8_t *data, uint16_t length);

    /// @brief Reads a block of data from the given register.
    /// @note sfeTkIBus interface method
    ///
    /// @param devAddr The device's I2C address.
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval 0 on success, < 0 on error - see error code
    ///
    int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t numBytes);

  private:
    // The actual Arduino i2c port
    TwoWire *_i2cPort;
};
