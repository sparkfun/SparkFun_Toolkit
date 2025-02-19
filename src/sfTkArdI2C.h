/**
 * @file sfTkArdI2C.cpp
 * @brief header file for the SparkFun Toolkit Arduino I2C driver.
 *
 * This file contains the Arduino I2C header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

// Include our platform I2C interface definition.
#include "sfTkArduino.h"
#include <sfTk/sfTkII2C.h>

/**
 * @brief The sfTkArdI2C implements an sfTkII2C interface, defining the Arduino implementation for I2C in the Toolkit
 */

class sfTkArdI2C : public sfTkII2C
{
  public:
    /**
    @brief Constructor
    */

    sfTkArdI2C(void) : _i2cPort(nullptr), _bufferChunkSize{kDefaultBufferChunk}
    {
    }
    /**
        @brief Constructor

        @param addr The address of the device
    */
    sfTkArdI2C(uint8_t addr) : sfTkII2C(addr)
    {
    }

    /**
     * @brief copy constructor
     */
    sfTkArdI2C(sfTkArdI2C const &rhs) : sfTkII2C(), _i2cPort{rhs._i2cPort}
    {
    }

    /**
     * @brief Copy assignment
     *
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfTkArdI2C &operator=(const sfTkArdI2C &rhs)
    {
        _i2cPort = rhs._i2cPort;
        return *this;
    }

    /**
        @brief Method sets up the required I2C settings.
        @note This function provides a default I2C Port.

        @retval ksfTkErrOk on successful execution.
    */
    sfTkError_t init();

    /**
        @brief - address version of the init method

        @param addr The address of the device
    */
    sfTkError_t init(uint8_t addr);

    /**
        @brief Method sets up the required I2C settings.

        @param wirePort Port for I2C communication.
        @param addr The address of the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval ksfTkErrOk on successful execution.
    */
    sfTkError_t init(TwoWire &wirePort, uint8_t addr, bool bInit = false);

    /**
        @brief A simple ping of the device at the given address.
        @note sfTkIBus interface method

        @retval ksfTkErrOk on success,
    */
    sfTkError_t ping();

    /**
        @brief Sends a block of data to the device.
        @note sfTkIBus interface method

        @param data Data to write.
        @param length - length of data

        @retval returns  ksfTkErrOk on success
    */
    sfTkError_t writeRegion(const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's address.

        @note sfTkIBus interface method
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval ksfTkErrOk on success
    */
    sfTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's 16-bit address.

        @param devReg The device's register's address - 16 bit.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t ksfTkErrOk on successful execution

    */
    sfTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's 16-bit address.

        @param devReg The device's register's address - 16 bit.
        @param data Data to write.
        @param length - length of data

        @retval sfTkError_t ksfTkErrOk on successful execution

    */
    sfTkError_t writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length);

    /**
        @brief Reads a block of data from the given register.

        @note sfTkIBus interface method
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param[out] data Data buffer to read into
        @param numBytes Number of bytes to read/length of data buffer
        @param[out] readBytes - Number of bytes read


        @retval ksfTkErrOk on success
    */
    sfTkError_t readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given 16-bit register address.

        @param reg The device's 16 bit register's address.
        @param data Data buffer to read into
        @param numBytes - Number of bytes to read/length of data buffer
        @param[out] readBytes - number of bytes read

        @retval int returns ksfTkErrOk on success, or ksfTkErrFail code

    */
    sfTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given 16-bit register address.

        @param reg The device's 16 bit register's address.
        @param data Data buffer to read into
        @param numBytes - Number of bytes to read/length of data buffer
        @param[out] readBytes - number of bytes read

        @retval int returns ksfTkErrOk on success, or ksfTkErrFail code

    */
    sfTkError_t readRegister16Region16(uint16_t reg, uint16_t *data, size_t numBytes, size_t &readBytes);

    // Buffer size chunk getter/setter
    /**
        @brief set the buffer chunk size

        @note default size is 32

        @param theChunk the new size  - must be > 0

    */
    void setBufferChunkSize(size_t theChunk)
    {
        if (theChunk > 0)
            _bufferChunkSize = theChunk;
    }

    /**
        @brief set the buffer chunk size

        @retval The current chunk size

    */
    size_t bufferChunkSize(void)
    {
        return _bufferChunkSize;
    }

  protected:
    // note: The wire port is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** The actual Arduino i2c port */
    TwoWire *_i2cPort;

  private:
    sfTkError_t writeRegisterRegionAddress(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length);

    sfTkError_t readRegisterRegionAnyAddress(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes,
                                             size_t &readBytes);

    /** Default buffer chunk size*/
    static constexpr size_t kDefaultBufferChunk = 32;

    /** The I2C buffer chunker - chunk size*/
    size_t _bufferChunkSize;
};
