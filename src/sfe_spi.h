/*
sfe_spi.h

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

The following class specifies the behavior for communicating
over Serial Peripheral Interface (SPI).

*/

#pragma once

#include "sfe_bus.h"
#include <SPI.h>

/*
The SfeSPI class defines behavior for SPI implementation based around the SPIClass class (SPI).
This is Arduino specific.
Paramaters like "devAddr" are kept although irrelevant to SPI due to the use of the abstract class
as interface, SfeBus.
*/
class SfeSPI : public SfeBus
{
  public:
    /// @brief Constructor
    SfeSPI(void) : _spiPort(nullptr) {};

    /// @brief Method sets up the required SPI settings. This function
    /// provides a default SPI Port.
    /// @param cs The chip select pin.
    /// @param bInit This flag tracks whether the bus has been initialized.
    /// @return True on successful execution.
    bool init(uint8_t cs, bool bInit = false);

    /// @brief Method sets up the required SPI settings.
    /// @param spiPort Port for SPI communcation.
    /// @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
    /// @param cs The chip select pin.
    /// @param bInit This flag tracks whether the bus has been initialized.
    /// @return True on successful execution.
    bool init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t cs, bool bInit = false);

    /// @brief Is a device connected? The SPI ping is not relevant but is defined here to
    /// keep consistency with I2C class i.e. provided for the abstract interface.
    /// @param devAddr Address of the device (unused)
    bool ping(uint8_t devAddr = 0);

    /// @brief Write a single byte to the given register
    /// @param devAddr The device's I2C address (ignored).
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    /// @brief returns true on successful execution.
    bool writeRegisterByte(uint8_t devAddr, uint8_t devReg, uint8_t data);

    /// @brief Writes a number of bytes starting at the given register's address.
    /// @param devAddr The device's I2C address (ignored).
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    /// @brief returns true on successful execution.
    int writeRegisterRegion(uint8_t devAddr, uint8_t devReg, const uint8_t *data, uint16_t length);

    /// @brief Reads a block of data from the given register.
    /// @param devAddr The device's I2C address (ignored).
    /// @param devReg The device's register's address.
    /// @param data Data to write.
    /// @brief returns true on successful execution.
    int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t numBytes);

  private:
    SPIClass *_spiPort;
    // Settings are used for every transaction.
    SPISettings _sfeSPISettings;
    uint8_t _cs;
};
