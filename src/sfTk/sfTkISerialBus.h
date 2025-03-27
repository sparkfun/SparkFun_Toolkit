/**
 * @file sfTkISerialBus.h
 * @brief Header file for the SparkFun Toolkit Base Serial Bus Interface Definition.
 *
 * This file contains the interface declaration for connecting sfTkISerial to the sfTkIBus.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

// clang-format off
#include "sfTkError.h"
#include "sfTkIBus.h"
#include "sfTkISerial.h"
// clang-format on

const uint8_t ksfTkBusTypeSerialBus = 0x03;

class sfTkISerialBus : sfTkIBus
{
  public:
    /**
     * @brief Constructor for the serial bus
     *
     */
    sfTkISerialBus()
    {
    }

    virtual ~sfTkISerialBus() = default;

    /**
     * @brief Writes an array of bytes to the serial interface.
     *
     * @param data The data to write
     * @param length The length of the data buffer
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t write(const uint8_t *data, size_t length) = 0;

    /**
     * @brief Writes an array of bytes to a register on the target address. Supports any address size
     *
     * @param devReg The device's register's address - can be any size, If nullptr, address is not sent
     * @param regLength The length of the register address. If 0, address is not sent
     * @param data The data to write
     * @param length The length of the data buffer
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t writeRegister(uint8_t *devReg, size_t regLength, const uint8_t *data, size_t length) override
    {

        // Do we have a register? If so write it, else skip.
        if (devReg != nullptr && regLength > 0)
            write(devReg, regLength);

        // Write the data.
        return write(data, length);
    }

    /**
     * @brief Read an array of bytes from the serial interface
     *
     * @param data The data buffer to read into
     * @param length The length of the data buffer
     * @param readBytes[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t read(uint8_t *data, size_t length, size_t &readBytes) = 0;

    /**
     * @brief Reads an array of bytes to a register on the target address. Supports any address size
     *
     * @param devReg The device's register's address - can be any size
     * @param regLength The length of the register address
     * @param data The data to buffer to read into
     * @param numBytes The length of the data buffer
     * @param readBytes[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t readRegister(uint8_t *devReg, size_t regLength, uint8_t *data, size_t numBytes,
                                     size_t &readBytes) override
    {
        // Buffer valid?
        if (!data)
            return ksfTkErrBusNullBuffer;

        sfTkError_t retVal = ksfTkErrOk;

        // Do we have a register? If so, write it, else skip.
        if (devReg != nullptr && regLength > 0)
            retVal = write(devReg, regLength);

        if (retVal != ksfTkErrOk)
            return retVal;

        // Read the data.
        retVal = read(data, numBytes, readBytes);

        return retVal;
    }

    /**
     * @brief Get the type of the object
     *
     * @return uint8_t The type of the object
     */
    virtual uint8_t type(void) override
    {
        return ksfTkBusTypeSerialBus;
    }
};
