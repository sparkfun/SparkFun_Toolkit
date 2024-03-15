

// sfeTkBusSPI.h - Defines the Arduino SPI interface for the SparkFun Toolkit SDK

/*

The MIT License (MIT)

Copyright (c) 2023 SparkFun Electronics

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
#include <sfeTk/sfeTkISPI.h>

/*
  @brief This class implements the IBus interface for an SPI Implementation on Arduino
 */
class sfeTkArdSPI : public sfeTkISPI
{
  public:
    /*
        @brief Constructor
    */
    sfeTkArdSPI(void) : _spiPort(nullptr)
    {
    }

    sfeTkArdSPI(uint8_t csPin) : sfeTkISPI(csPin)
    {
    }
    // copy constructor
    sfeTkArdSPI(sfeTkArdSPI const &rhs) : sfeTkISPI(), _spiPort{rhs._spiPort}, _sfeSPISettings{rhs._sfeSPISettings}
    {
    }

    // Copy assignment
    sfeTkArdSPI &operator=(const sfeTkArdSPI &rhs)
    {
        _spiPort = rhs._spiPort;
        _sfeSPISettings = rhs._sfeSPISettings;
        return *this;
    }

    /*--------------------------------------------------------------------------
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param bInit This flag tracks whether the bus has been initialized.

        @retval sfeTkError_t -  kSTkErrOk on success
    */
    sfeTkError_t init(bool bInit = false);

    sfeTkError_t init(uint8_t csPin, bool bInit = false);

    /*--------------------------------------------------------------------------
        @brief Method sets up the required SPI settings.

        @param spiPort Port for SPI communication.
        @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
        @param bInit This flag tracks whether the bus has been initialized.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit = false);

    /*--------------------------------------------------------------------------
        @brief Write a single byte to the device

        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeByte(uint8_t data);

    /*--------------------------------------------------------------------------
        @brief Write a single byte to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterByte(uint8_t devReg, uint8_t data);

    /*--------------------------------------------------------------------------
        @brief Write a single word to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterWord(uint8_t devReg, uint16_t data);

    /*--------------------------------------------------------------------------
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /*--------------------------------------------------------------------------
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length);

    /*--------------------------------------------------------------------------
        @brief Read a single byte from the given register

        @param devReg The device's register's address.
        @param data Data to read.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t readRegisterByte(uint8_t devReg, uint8_t &data);

    /*--------------------------------------------------------------------------
        @brief read a single word to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - true on success
    */
    sfeTkError_t readRegisterWord(uint8_t devReg, uint16_t &data);

    /*--------------------------------------------------------------------------
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param numBytes - length of data
        @param[out] readBytes - Number of bytes read

        @retval sfeTkError_t - true on success
    */
    virtual sfeTkError_t readRegisterRegion(uint8_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /*--------------------------------------------------------------------------
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's 16 bit address.
        @param data Data to write.
        @param numBytes - length of data
        @param[out] readBytes - Number of bytes read

        @retval sfeTkError_t - true on success
    */
    virtual sfeTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

  protected:
    // note: The instance data is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //

    SPIClass *_spiPort;

    // Settings are used for every transaction.
    SPISettings _sfeSPISettings;
};
