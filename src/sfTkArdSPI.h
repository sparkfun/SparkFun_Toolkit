/**
 * @file sfTkArdSPI.cpp
 * @brief header file for the SparkFun Toolkit Arduino SPI driver.
 *
 * This file contains the Arduino SPI header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "sfTkArduino.h"
#include <SPI.h>
#include <sfTk/sfTkISPI.h>

/**
  @brief This class implements the IBus interface for an SPI Implementation on Arduino
 */
class sfTkArdSPI : public sfTkISPI
{
  public:
    /**
        @brief Constructor for Arduino SPI bus object of the toolkit
    */
    sfTkArdSPI(void) : _spiPort(nullptr)
    {
    }

    /**
        @brief Constructor for Arduino SPI bus object of the toolkit

        @param csPin The CS Pin for the device
    */
    sfTkArdSPI(uint8_t csPin) : sfTkISPI(csPin)
    {
    }
    /**
        @brief Copy constructor for Arduino SPI bus object of the toolkit

        @param rhs source of the copy operation
    */
    sfTkArdSPI(sfTkArdSPI const &rhs) : sfTkISPI(), _spiPort{rhs._spiPort}, _sfeSPISettings{rhs._sfeSPISettings}
    {
    }

    /**
        @brief Assignment copy operator for Arduino SPI bus object of the toolkit

        @param rhs The right hand side of the assignment.
        @return sfTkArdSPI& - The left hand side of the assignment.
    */
    sfTkArdSPI &operator=(const sfTkArdSPI &rhs)
    {
        _spiPort = rhs._spiPort;
        _sfeSPISettings = rhs._sfeSPISettings;
        return *this;
    }

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param bInit Init the device - default is false.

        @retval sfTkError_t -  ksfTkErrOk on success
    */
    sfTkError_t init(bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.
        @note This function provides a default SPI Port.

        @param csPin The CS Pin for the device
        @param bInit Init the device - default is false.

        @retval sfTkError_t -  ksfTkErrOk on success
    */
    sfTkError_t init(uint8_t csPin, bool bInit = false);

    /**
        @brief Method sets up the required SPI settings.

        @param spiPort Port for SPI communication.
        @param busSPISettings Settings for speed, endianness, and spi mode of the SPI bus.
        @param csPin The CS Pin for the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit = false);

    /**
        @brief Write a single byte to the device

        @param data Data to write.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeByte(uint8_t data);

    /**
        @brief Write a word to the device without indexing to a register.

        @param data Data to write.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeWord(uint16_t data);

    /**
        @brief Write an array of data to the device without indexing to a register.

        @param data Data to write
        @param length Length of Data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegion(const uint8_t *data, size_t length);

    /**
        @brief Write a single byte to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegisterByte(uint8_t devReg, uint8_t data);

    /**
        @brief Write a single word to the given register

        @param devReg The device's register's address.
        @param data Data to write.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegisterWord(uint8_t devReg, uint16_t data);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of uint16s starting at the given register's address.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t - ksfTkErrOk on success
        */
    sfTkError_t writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length);

    /**
        @brief Read a single byte from the given register

        @param devReg The device's register's address.
        @param[out] data Data to read.

        @retval sfTkError_t - ksfTkErrOk on success
    */
    sfTkError_t readRegisterByte(uint8_t devReg, uint8_t &data);

    /**
        @brief read a single word to the given register

        @param devReg The device's register's address.
        @param[out] data Data to write.

        @retval sfTkError_t - true on success
    */
    sfTkError_t readRegisterWord(uint8_t devReg, uint16_t &data);

    /**
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param reg The device's register's address.
        @param[out] data Data buffer to read into
        @param numBytes - length of data/size of data buffer
        @param[out] readBytes - Number of bytes read

        @retval sfTkError_t - true on success
    */
    virtual sfTkError_t readRegisterRegion(uint8_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given register.
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param reg The device's register's 16 bit address.
        @param[out] data Data buffer to read into
        @param numBytes - Length of data to read/size of data buffer
        @param[out] readBytes - Number of bytes read

        @retval sfTkError_t - true on success
    */
    virtual sfTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
            @brief Reads a block of data from the given register.
            @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

            @param reg The device's register's 16 bit address.
            @param[out] data Data buffer to read into
            @param numBytes - Length of data to read/size of data buffer
            @param[out] readBytes - Number of bytes read

            @retval sfTkError_t - true on success
        */
    virtual sfTkError_t readRegister16Region16(uint16_t reg, uint16_t *data, size_t numBytes, size_t &readBytes);

  protected:
    // note: The instance data is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** Pointer to the spi port being used */
    SPIClass *_spiPort;

    /** This objects spi settings are used for every transaction. */
    SPISettings _sfeSPISettings;
};
