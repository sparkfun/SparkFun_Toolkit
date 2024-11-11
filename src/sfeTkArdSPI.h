

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

#include "sfeTkArduino.h"
#include <SPI.h>
#include <sfeTk/sfeTkISPI.h>

/**
  @brief This class implements the IBus interface for an SPI Implementation on Arduino
 */
class sfeTkArdSPI : public sfeTkISPI
{
  public:
    /**
        @brief Constructor for Arduino SPI bus object of the toolkit
    */
    sfeTkArdSPI(void) : _spiPort(nullptr)
    {
    }

    /**
        @brief Constructor for Arduino SPI bus object of the toolkit

        @param csPin The CS Pin for the device
    */
    sfeTkArdSPI(uint8_t csPin) : sfeTkISPI(csPin)
    {
    }
    /**
        @brief Copy constructor for Arduino SPI bus object of the toolkit

        @param rhs source of the copy operation
    */
    sfeTkArdSPI(sfeTkArdSPI const &rhs) : sfeTkISPI(), _spiPort{rhs._spiPort}, _sfeSPISettings{rhs._sfeSPISettings}
    {
    }

    /**
        @brief Assignment copy operator for Arduino SPI bus object of the toolkit

        @param rhs The right hand side of the assignment.
        @return sfeTkArdSPI& - The left hand side of the assignment.
    */
    sfeTkArdSPI &operator=(const sfeTkArdSPI &rhs)
    {
        _spiPort = rhs._spiPort;
        _sfeSPISettings = rhs._sfeSPISettings;
        return *this;
    }

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param bInit Init the device - default is false.

        @retval sfeTkError_t -  kSTkErrOk on success
    */
    sfeTkError_t init(bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param csPin The CS Pin for the device
        @param bInit Init the device - default is false.

        @retval sfeTkError_t -  kSTkErrOk on success
    */
    sfeTkError_t init(uint8_t csPin, bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.

        @param spiPort Port for SPI communication.
        @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
        @param csPin The CS Pin for the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit = false);

    /**
        @brief Write a single byte to the device

        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeByte(uint8_t data);

    /**
        @brief Write a word to the device without indexing to a register.

        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeWord(uint16_t data);

    /**
        @brief Write an array of data to the device without indexing to a register.

        @param data Data to write
        @param length Length of Data

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegion(const uint8_t *data, size_t length);

    /**
        @brief Write a single byte to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterByte(uint8_t devReg, uint8_t data);

    /**
        @brief Write a single word to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterWord(uint8_t devReg, uint16_t data);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of uint16s starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfeTkError_t - kSTkErrOk on success
        */
    sfeTkError_t writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length);

    /**
        @brief Read a single byte from the given register

        @param devReg The device's register's address.
        @param[out] data Data to read.

        @retval sfeTkError_t - kSTkErrOk on success
    */
    sfeTkError_t readRegisterByte(uint8_t devReg, uint8_t &data);

    /**
        @brief read a single word to the given register

        @param devReg The device's register's address.
        @param[out] data Data to write.

        @retval sfeTkError_t - true on success
    */
    sfeTkError_t readRegisterWord(uint8_t devReg, uint16_t &data);

    /**
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param reg The device's register's address.
        @param[out] data Data buffer to read into
        @param numBytes - length of data/size of data buffer
        @param[out] readBytes - Number of bytes read

        @retval sfeTkError_t - true on success
    */
    virtual sfeTkError_t readRegisterRegion(uint8_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param reg The device's register's 16 bit address.
        @param[out] data Data buffer to read into
        @param numBytes - Length of data to read/size of data buffer
        @param[out] readBytes - Number of bytes read

        @retval sfeTkError_t - true on success
    */
    virtual sfeTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
            @brief Reads a block of data from the given register.
            @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

            @param reg The device's register's 16 bit address.
            @param[out] data Data buffer to read into
            @param numWords - Length of data to read/size of data buffer
            @param[out] readWords - Number of words read

            @retval sfeTkError_t - true on success
        */
    virtual sfeTkError_t readRegister16Region16(uint16_t reg, uint16_t *data, size_t numWords, size_t &readWords);

  protected:
    // note: The instance data is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** Pointer to the spi port being used */
    SPIClass *_spiPort;

    /** This objects spi settings are used for every transaction. */
    SPISettings _sfeSPISettings;
};
