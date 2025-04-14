/**
 * @file sfTkISerial.h
 * @brief Header file for the SparkFun Toolkit Base Serial Interface Definition.
 *
 * This file contains the interface declaration for basic serial read/write.
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
// clang-format on

/**
 * @brief A base value for serial errors. All serial errors are greater than this value, in the 2000 range
 *
 */
const sfTkError_t ksfTkErrBaseSerial = 0x2000;

/**
 * @brief Error code for when a serial system is not initalized.
 */
const sfTkError_t ksfTkErrSerialNotInit = ksfTkErrFail * (ksfTkErrBaseSerial + 1);

/**
 * @brief Returned when a serial interface times out.
 *
 */
const sfTkError_t ksfTkErrSerialTimeout = ksfTkErrFail * (ksfTkErrBaseSerial + 2);

/**
 * @brief Returned when a serial interface does not respond.
 *
 */
const sfTkError_t ksfTkErrSerialNoResponse = ksfTkErrFail * (ksfTkErrBaseSerial + 3);

/**
 * @brief Returned when the data to be sent is too long or received is too short.
 *
 */
const sfTkError_t ksfTkErrSerialDataTooLong = ksfTkErrFail * (ksfTkErrBaseSerial + 4);

/**
 * @brief Returned when the serial settings are null, invalid, or on set/initialized.
 *
 */
const sfTkError_t ksfTkErrSerialNullSettings = ksfTkErrFail * (ksfTkErrBaseSerial + 5);

/**
 * @brief Returned when the buffer is null or invalid.
 *
 */
const sfTkError_t ksfTkErrSerialNullBuffer = ksfTkErrFail * (ksfTkErrBaseSerial + 6);

/**
 * @brief Returned when the serial is under read. Warning.
 *
 */
const sfTkError_t ksfTkErrSerialUnderRead = ksfTkErrBaseSerial + 7;

/**
 * @brief Returned when the serial is not enabled. Warning
 */
const sfTkError_t ksfTkErrSerialNotEnabled = ksfTkErrBaseSerial + 8;

/**
 * @brief Returned when the serial data is bad or corrupted.
 */
const sfTkError_t ksfTkErrSerialBadData = ksfTkErrFail * (ksfTkErrBaseSerial + 9);

class sfTkISerial
{
  public:
    sfTkISerial() = default;
    virtual ~sfTkISerial() = default;

    /**
     * @brief Writes an array of bytes to the serial interface.
     *
     * @param data The data to write
     * @param length The length of the data buffer
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t write(const uint8_t *data, size_t length) = 0;

    /**
     * @brief Writes a single byte to the serial interface.
     *
     * @param data The data to write
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t write(const uint8_t data)
    {
        return write(&data, sizeof(data));
    }

    /**
     * @brief Reads an array of bytes from the serial interface
     *
     * @param data The data buffer to read into
     * @param length The length of the data buffer
     * @param readBytes[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t read(uint8_t *data, size_t length, size_t &readBytes) = 0;

    /**
     * @brief Read a single byte from the serial interface
     *
     * @param data Byte to be read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t read(uint8_t &data)
    {
        size_t nRead;

        return read(&data, sizeof(data), nRead);
    }

    /**
     * @brief Check if there are bytes available to read
     *
     * @return int Returns the number of bytes available to read in the read buffer
     */
    virtual int available() = 0;

    /**
     * @brief Check if the serial interface is available for writing
     *
     * @return int Returns the number of bytes available to write
     */
    virtual int availableForWrite() = 0;

    /**
     * @brief Peek at the next byte available to read without removing it from the buffer
     *
     * @return int Returns the next byte available to read, or -1 if no bytes are available
     */
    virtual int peek() = 0;

    /**
     * @brief Flush the serial interface's write buffer
     *
     */
    virtual void flush() = 0;
};
