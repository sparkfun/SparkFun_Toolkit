/*
sfeTkArdI2c.h

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

// Include our platform I2C interface definition.
#include <sfeTk/sfeTkII2C.h>

/// @brief
/// The sfeTkArdI2C implements an sfeTkII2C interface, defining the Arduino implementation for I2C in the Toolkit
///
class sfeTkArdI2C : public sfeTkII2C
{
  public:
    ///
    /// @brief Constructor
    ///
    sfeTkArdI2C(void) : _i2cPort(nullptr)
    {
    }

    sfeTkArdI2C(uint8_t addr) : sfeTkII2C(addr)
    {
    }

    // copy constructor
    sfeTkArdI2C(sfeTkArdI2C const &rhs) : sfeTkII2C(), _i2cPort{rhs._i2cPort}
    {
    }

    // Copy assignment
    sfeTkArdI2C &operator=(const sfeTkArdI2C &rhs)
    {
        _i2cPort = rhs._i2cPort;
        return *this;
    }

    /// @brief Method sets up the required I2C settings.
    /// @note This function provides a default I2C Port.
    ///
    /// @retval True on successful execution.
    ///
    bool init();

    // @brief - address version of the init method
    bool init(uint8_t addr);

    /// @brief Method sets up the required I2C settings.
    ///
    /// @param wirePort Port for I2C communication.
    /// @param bInit This flag tracks whether the bus has been initialized.
    ///
    /// @retval True on successful execution.
    ///
    bool init(TwoWire &wirePort, uint8_t addr, bool bInit = false);

    /// @brief A simple ping of the device at the given address.
    /// @note sfeTkIBus interface method
    ///
    /// @retval bool - true on success, false on error
    ///
    bool ping();

    /// @brief Write a single byte to the given register
    /// @note sfeTkIBus interface method
    ///
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval returns true on successful false on failure.
    bool writeRegisterByte(uint8_t devReg, uint8_t data);

    bool writeRegisterWord(uint8_t devReg, uint16_t data);

    /// @brief Writes a number of bytes starting at the given register's address.
    /// @note sfeTkIBus interface method
    /// @note This method is virtual to allow it to be overridden to support a device that requires a unique impl
    ///
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval returns number of bytes written, < 0 is an error code
    ///
    virtual int writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /// @brief Reads a byte of data from the given register.
    /// @note sfeTkIBus interface method
    ///
    /// @param devReg The device's register's address.
    /// @param data Data to read.
    ///
    /// @retval true on success, false on  error
    ///
    bool readRegisterByte(uint8_t devReg, uint8_t &data);

    /// @brief Reads a word of data from the given register.
    /// @note sfeTkIBus interface method
    ///
    /// @param devReg The device's register's address.
    /// @param data Data to read.
    ///
    /// @retval true on success, false on  error
    ///
    bool readRegisterWord(uint8_t devReg, uint16_t &data);

    /// @brief Reads a block of data from the given register.
    /// @note sfeTkIBus interface method
    /// @note This method is virtual to allow it to be overridden to support a device that requires a unique impl
    ///
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval 0 on success, < 0 on error - see error code
    ///
    virtual int readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes);

  private:
    // The actual Arduino i2c port
    TwoWire *_i2cPort;
};
