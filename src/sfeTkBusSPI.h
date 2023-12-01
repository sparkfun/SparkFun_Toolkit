

// sfeTkBusSPI.h - Defines the Arduino SPI interface for the SparkFun Toolkit SDK

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

#include <SPI.h>
#include <sfeTk/sfeTkIBus.h>

/// @brief This class implements the IBus interface for an SPI Implementation on Arduino
///
class sfeTkBusSPI : public sfeTkIBus
{
  public:
    ///
    /// @brief Constructor
    ///
    sfeTkBusSPI(void) : _spiPort(nullptr){};

    /// @brief Method sets up the required SPI settings.
    /// @note This function provides a default SPI Port.
    ///
    /// @param bInit This flag tracks whether the bus has been initialized.
    ///
    /// @retval bool -  True on successful, false on error
    ///
    bool init(bool bInit = false);

    /// @brief Method sets up the required SPI settings.
    ///
    /// @param spiPort Port for SPI communication.
    /// @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
    /// @param bInit This flag tracks whether the bus has been initialized.
    ///
    /// @retval bool - True on successful, false on error
    ///
    bool init(SPIClass &spiPort, SPISettings &busSPISettings, bool bInit = false);

    /// @brief Is a device connected? j
    /// @note The SPI ping is not relevant but is defined here to keep consistency with I2C class i.e. provided for the
    /// abstract interface.
    ///
    /// @param devAddr Address of the device (unused)
    ///
    /// @retval - returns true on success, false on error
    ///
    bool ping(uint8_t devAddr = 0)
    {
        return true; // always true
    }

    /// @brief Write a single byte to the given register
    ///
    /// @param cs The device chip select pin
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval bool - true on success, false on error
    ///
    bool writeRegisterByte(uint8_t cs, uint8_t devReg, uint8_t data);

    /// @brief Writes a number of bytes starting at the given register's address.
    ///
    /// @param cs The chip select pin for the device
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval int - number of bytes written, < 0 = error value
    ///
    int writeRegisterRegion(uint8_t cs, uint8_t devReg, const uint8_t *data, uint16_t length);

    /// @brief Reads a block of data from the given register.
    ///
    /// @param cs The chip select pin for the device
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    ///
    /// @retval int returns the number of bytes read, < 0 on error
    ///
    int readRegisterRegion(uint8_t cs, uint8_t reg, uint8_t *data, uint16_t numBytes);

  private:
    SPIClass *_spiPort;

    // Settings are used for every transaction.
    SPISettings _sfeSPISettings;
};
