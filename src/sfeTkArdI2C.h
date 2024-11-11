/*
sfeTkArdI2c.h

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

The following classes specify the behavior for communicating
over Inter-Integrated Circuit (I2C) in Arduino

*/

#pragma once

#include <Arduino.h>
#include <Wire.h>

// Include our platform I2C interface definition.
#include "sfeTkArduino.h"
#include <sfeTk/sfeTkII2C.h>

/**
 * @brief The sfeTkArdI2C implements an sfeTkII2C interface, defining the Arduino implementation for I2C in the Toolkit
 */

class sfeTkArdI2C : public sfeTkII2C
{
  public:
    /**
    @brief Constructor
    */

    sfeTkArdI2C(void) : _i2cPort(nullptr), _bufferChunkSize{kDefaultBufferChunk}, _byteOrder{SFTK_LSBFIRST}
    {
    }
    /**
        @brief Constructor

        @param addr The address of the device
    */
    sfeTkArdI2C(uint8_t addr) : sfeTkII2C(addr)
    {
    }

    /**
     * @brief copy constructor
     */
    sfeTkArdI2C(sfeTkArdI2C const &rhs) : sfeTkII2C(), _i2cPort{rhs._i2cPort}
    {
    }

    /**
     * @brief Copy assignment
     *
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfeTkArdI2C &operator=(const sfeTkArdI2C &rhs)
    {
        _i2cPort = rhs._i2cPort;
        return *this;
    }

    /**
        @brief Method sets up the required I2C settings.
        @note This function provides a default I2C Port.

        @retval kSTkErrOk on successful execution.
    */
    sfeTkError_t init();

    /**
        @brief - address version of the init method

        @param addr The address of the device
    */
    sfeTkError_t init(uint8_t addr);

    /**
        @brief Method sets up the required I2C settings.

        @param wirePort Port for I2C communication.
        @param addr The address of the device
        @param bInit This flag tracks whether the bus has been initialized.

        @retval kSTkErrOk on successful execution.
    */
    sfeTkError_t init(TwoWire &wirePort, uint8_t addr, bool bInit = false);

    /**
        @brief A simple ping of the device at the given address.
        @note sfeTkIBus interface method

        @retval kSTkErrOk on success,
    */
    sfeTkError_t ping();

    /**
        @brief Sends a single byte to the device
        @note sfeTkIBus interface method

        @param data Data to write.

        @retval returns  kStkErrOk on success
    */
    sfeTkError_t writeByte(uint8_t data);

    /**
        @brief Sends a word to the device.
        @note sfeTkIBus interface method

        @param data Data to write.

        @retval returns  kStkErrOk on success
    */
    sfeTkError_t writeWord(uint16_t data);

    /**
        @brief Sends a block of data to the device.
        @note sfeTkIBus interface method

        @param data Data to write.
        @param length - length of data

        @retval returns  kStkErrOk on success
    */
    sfeTkError_t writeRegion(const uint8_t *data, size_t length);

    /**
        @brief Write a single byte to the given register
        @note sfeTkIBus interface method

        @param devReg The device's register's address.
        @param data Data to write.

        @retval returns  kStkErrOk on success
    */
    sfeTkError_t writeRegisterByte(uint8_t devReg, uint8_t data);

    /**
        @brief Write a single word to the given register
        @note sfeTkIBus interface method

        @param devReg The device's register's address.
        @param data Data to write.

        @retval returns  kStkErrOk on success
    */
    sfeTkError_t writeRegisterWord(uint8_t devReg, uint16_t data);

    /**
        @brief Writes a number of bytes starting at the given register's address.

        @note sfeTkIBus interface method
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param data Data to write.
        @param length - length of data

        @retval kStkErrOk on success
    */
    sfeTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's 16-bit address.

        @param devReg The device's register's address - 16 bit.
        @param data Data to write.
        @param length - length of data

        @retval sfeTkError_t kSTkErrOk on successful execution

    */
    sfeTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length);

    /**
        @brief Writes a number of bytes starting at the given register's 16-bit address.

        @param devReg The device's register's address - 16 bit.
        @param data Data to write.
        @param length - length of data

        @retval sfeTkError_t kSTkErrOk on successful execution

    */
    sfeTkError_t writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length);

    /**
        @brief Reads a byte of data from the given register.

        @note sfeTkIBus interface method

        @param devReg The device's register's address.
        @param[out] data Data to read.

        @retval  kStkErrOk on success
    */
    sfeTkError_t readRegisterByte(uint8_t devReg, uint8_t &data);

    /**
        @brief Reads a word of data from the given register.

        @note sfeTkIBus interface method

        @param devReg The device's register's address.
        @param[out] data Data to read.

        @retval kSTkErrOk on success
    */
    sfeTkError_t readRegisterWord(uint8_t devReg, uint16_t &data);

    /**
        @brief Reads a block of data from the given register.

        @note sfeTkIBus interface method
        @note This method is virtual to allow it to be overridden to support a device that requires a unique impl

        @param devReg The device's register's address.
        @param[out] data Data buffer to read into
        @param numBytes Number of bytes to read/length of data buffer
        @param[out] readBytes - Number of bytes read


        @retval kSTkErrOk on success
    */
    sfeTkError_t readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given 16-bit register address.

        @param reg The device's 16 bit register's address.
        @param data Data buffer to read into
        @param numBytes - Number of bytes to read/length of data buffer
        @param[out] readBytes - number of bytes read

        @retval int returns kSTkErrOk on success, or kSTkErrFail code

    */
    sfeTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes);

    /**
        @brief Reads a block of data from the given 16-bit register address.

        @param reg The device's 16 bit register's address.
        @param data Data buffer to read into
        @param numWords - Number of words to read/length of data buffer
        @param[out] readWords - number of words read

        @retval int returns kSTkErrOk on success, or kSTkErrFail code

    */
    sfeTkError_t readRegister16Region16(uint16_t reg, uint16_t *data, size_t numWords, size_t &readWords);

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

    /**
     * @brief Set the byte order for multi-byte data transfers
     *
     * @param order The byte order to set - set to either SFTK_MSBFIRST or SFTK_LSBFIRST. The default is SFTK_LSBFIRST
     *
     */
    void setByteOrder(sfeTKByteOrder order)
    {
        _byteOrder = order;
    }

  protected:
    // note: The wire port is protected, allowing access if a sub-class is
    //      created to implement a special read/write routine
    //
    /** The actual Arduino i2c port */
    TwoWire *_i2cPort;

  private:
    sfeTkError_t writeRegisterRegionAddress(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length);

    sfeTkError_t readRegisterRegionAnyAddress(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes,
                                              size_t &readBytes);

    /** Default buffer chunk size*/
    static constexpr size_t kDefaultBufferChunk = 32;

    /** The I2C buffer chunker - chunk size*/
    size_t _bufferChunkSize;

    /** flag to manage byte swapping */
    sfeTKByteOrder _byteOrder;
};
