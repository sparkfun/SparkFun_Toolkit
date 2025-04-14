/**
 * @file sfTkArdSerial.h
 * @brief header file for the SparkFun Toolkit Arduino Serial driver.
 *
 * This file contains the Arduino Serial header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Arduino.h>

// clang-format off
#include "sfTk/sfTkError.h"
#include "sfTk/sfTkISerial.h"
#include "sfTkArduino.h"
// clang-format on

class sfTkArdSerial : public sfTkISerial

{
  public:
    /**
     * @brief Default constructor for the sfTkArdSerial class.
     *        Initializes the object with no hardware stream.
     */
    sfTkArdSerial() : sfTkISerial(), _hwStream{nullptr}
    {
    }

    /**
     * @brief Constructor for the sfTkArdSerial class.
     *        Initializes the object with a given hardware stream.
     *
     * @param hwStream Reference to a Stream object to be used for communication.
     */
    sfTkArdSerial(Stream &hwStream) : sfTkISerial(), _hwStream{&hwStream}
    {
    }

    /**
     * @brief Copy Constructor for the sfTkArdSerial class.
     *
     * @param rhs Reference to another sfTkArdSerial object to copy from.
     */
    sfTkArdSerial(sfTkArdSerial const &rhs) : sfTkISerial(), _hwStream(rhs._hwStream)
    {
    }

    /**
     * @brief Assignment operator for the sfTkArdSerial class.
     *
     * @param rhs Reference to another sfTkArdSerial object to assign from.
     * @return value of the left hand side of the assignment
     */
    sfTkArdSerial &operator=(const sfTkArdSerial &rhs)
    {
        if (this != &rhs)
        {
            _hwStream = rhs._hwStream;
        }
        return *this;
    }

    /**
     * @brief Initializes the sfTkArdSerial object with a hardware stream.
     *
     * @note hwStream is not initialized here, it must be initialized external to this class.
     *
     * @param hwStream Reference to a Stream object to be used for communication.
     * @return sfTkError_t Error code indicating the result of the initialization.
     */
    sfTkError_t init(Stream &hwStream);

    /**
     * @brief Initializes the sfTkArdSerial object with the default hardware stream.
     *
     * @note The stream object is not initialized here, it must be initialized external to this class.
     *
     * @return sfTkError_t Error code indicating the result of the initialization.
     */
    sfTkError_t init();

    /**
     * @brief Writes data to the hardware stream.
     *
     * @param data Pointer to the data buffer to be written.
     * @param length Number of bytes to write from the data buffer.
     * @return sfTkError_t Error code indicating the result of the write operation.
     */
    sfTkError_t write(const uint8_t *data, size_t length) override;

    /**
     * @brief Reads data from the hardware stream.
     *
     * @param data Pointer to the buffer where the read data will be stored.
     * @param length Number of bytes to read.
     * @param readBytes Reference to a variable where the number of bytes actually read will be stored.
     * @return sfTkError_t Error code indicating the result of the read operation.
     */
    sfTkError_t read(uint8_t *data, size_t length, size_t &readBytes) override;

    /**
     * @brief Gets the number of bytes available to read from the hardware stream.
     *
     * @return int Number of bytes available to read.
     */
    int available() override;

    /**
     * @brief Check if the serial interface is available for writing
     *
     * @return int Returns the number of bytes available to write
     */
    int availableForWrite() override;

    /**
     * @brief Peeks at the next byte in the hardware stream without removing it.
     *
     * @return int The next byte in the stream, or -1 if no data is available.
     */
    int peek() override;

    /**
     * @brief Flushes the hardware stream, ensuring all data is transmitted.
     */
    void flush() override;

  protected:
    Stream *_hwStream = nullptr;
};
