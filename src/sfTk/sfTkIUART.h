/**
 * @file sfTkIUART.h
 * @brief Header file for the SparkFun Toolkit UART Interface Definition.
 *
 * This file contains the interface declaration for UART configuration.
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
#include "sfTkISerial.h"
// clang-format on

// Enums follow the Arduino Serial API constants.

typedef enum _sfTkUARTParity
{
    kUARTParityEven = 0x1ul,
    kUARTParityOdd = 0x2ul,
    kUARTParityNone = 0x3ul,
    kUARTParityMark = 0x4ul,
    kUARTParitySpace = 0x5ul
} sfTkUARTParity_t;

inline const char *parityToString(sfTkUARTParity_t parity)
{
    static const char *parityArray[] = {"Even", "Odd", "None", "Mark", "Space"};

    // if the parity is out of bounds, return "Unknown"
    if (parity < kUARTParityEven || parity > kUARTParitySpace)
        return "Unknown";

    // return the parity string
    return parityArray[((uint8_t)parity) - 1];
}

typedef enum _sfTkUARTStopBits
{
    kUARTStopBitsOne = 0x10ul,
    kUARTStopBitsOneAndHalf = 0x20ul,
    kUARTStopBitsTwo = 0x30ul
} sfTkUARTStopBits_t;

inline const char *stopBitsToString(sfTkUARTStopBits_t stopBits)
{
    static const char *stopBitsArray[] = {"One", "OneAndHalf", "Two"};

    // Return "Unknown" if index is out of bounds (less than 0 or greater than 2)
    if (stopBits < kUARTStopBitsOne || stopBits > kUARTStopBitsTwo)
        return "Unknown";

    // Return the stop bits string
    return stopBitsArray[(((uint8_t)stopBits) >> 4) - 1];
}

typedef enum _sfTkUARTDataBits
{
    kUARTDataBitsFive = 0x100ul,
    kUARTDataBitsSix = 0x200ul,
    kUARTDataBitsSeven = 0x300ul,
    kUARTDataBitsEight = 0x400ul,
} sfTkUARTDataBits_t;

inline uint8_t dataBitsToValue(sfTkUARTDataBits_t dataBits)
{
    static const uint8_t dataBitsArray[] = {5, 6, 7, 8};

    // Check if data bits are within valid range
    if (dataBits < kUARTDataBitsFive || dataBits > kUARTDataBitsEight)
        return 0;

    // Extract index using bit shift (removing first 8 bits) and subtract 1
    return dataBitsArray[(((uint16_t)dataBits) >> 8) - 1];
}

class sfTkIUART : public sfTkISerial
{
  public:
    typedef struct _UARTConfig
    {
        uint32_t baudRate;
        sfTkUARTDataBits_t dataBits;
        sfTkUARTParity_t parity;
        sfTkUARTStopBits_t stopBits;
    } UARTConfig_t;
    /**
     * @brief Default constructor for the UART bus
     *
     * @param baudRate
     */
    sfTkIUART(uint32_t baudRate = kDefaultBaudRate)
        : _config{kDefaultBaudRate, kUARTDataBitsEight, kUARTParityNone, kUARTStopBitsOne}
    {
    }

    /**
     * @brief Constructor for the UART bus with a configuration passed in
     *
     * @param config
     */
    sfTkIUART(UARTConfig_t config) : _config(config)
    {
    }

    /**
     * @brief setter for UART baud rate
     *
     * @param baudRate The baud rate to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setBaudRate(const uint32_t baudRate)
    {
        if (_config.baudRate != baudRate)
            _config.baudRate = baudRate; // set the baud rate

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the baud rate
     *
     * @retval uint32_t returns the baud rate
     */
    virtual uint32_t baudRate(void) const
    {
        return _config.baudRate;
    }

    /**
     * @brief setter for the stop bits
     *
     * @param stopBits The stop bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setStopBits(const sfTkUARTStopBits_t stopBits)
    {
        if (_config.stopBits != stopBits)
            _config.stopBits = stopBits;

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the stop bits
     *
     * @retval sfTkUARTStopBits_t returns the stop bits
     */
    virtual sfTkUARTStopBits_t stopBits(void) const
    {
        return _config.stopBits;
    }

    /**
     * @brief setter for the parity
     *
     * @param parity The parity to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setParity(const sfTkUARTParity_t parity)
    {
        if (_config.parity != parity)
            _config.parity = parity;

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the parity
     *
     * @retval sfTkUARTParity_t returns the parity
     */
    virtual sfTkUARTParity_t parity(void) const
    {
        return _config.parity;
    }

    /**
     * @brief setter for the data bits
     *
     * @param dataBits The data bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setDataBits(const sfTkUARTDataBits_t dataBits)
    {
        if (_config.dataBits != dataBits)
            _config.dataBits = dataBits;

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the data bits
     *
     * @retval uint8_t returns the data bits
     */
    virtual sfTkUARTDataBits_t dataBits(void) const
    {
        return _config.dataBits;
    }

    /**
     * @brief setter for the internal config object
     *
     * @param baudRate The baud rate to set
     * @param dataBits The data bits to set
     * @param parity The parity to set
     * @param stopBits The stop bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setConfig(const uint32_t baudRate = kDefaultBaudRate,
                                  const sfTkUARTDataBits_t dataBits = kDefaultDataBits,
                                  const sfTkUARTParity_t parity = kDefaultParity,
                                  const sfTkUARTStopBits_t stopBits = kDefaultStopBits)
    {
        if (_config.baudRate != baudRate)
            _config.baudRate = baudRate;

        if (_config.dataBits != dataBits)
            _config.dataBits = dataBits;

        if (_config.parity != parity)
            _config.parity = parity;

        if (_config.stopBits != stopBits)
            _config.stopBits = stopBits;

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the internal config object
     *
     * @return sfTkUARTConfig_t
     */
    virtual UARTConfig_t config(void) const
    {
        return _config;
    }

    /**
     * @brief kDefaultBaudRate - Default baud rate for the UART bus
     *
     */
    static constexpr uint32_t kDefaultBaudRate = 115200;

    /**
     * @brief kDefaultStopBits - Default stop bits for the UART bus
     *
     */
    static constexpr sfTkUARTStopBits_t kDefaultStopBits = kUARTStopBitsOne;

    /**
     * @brief kDefaultParity - Default parity for the UART bus
     *
     */
    static constexpr sfTkUARTParity_t kDefaultParity = kUARTParityNone;

    /**
     * @brief kDefaultDataBits - Default data bits for the UART bus
     *
     */
    static constexpr sfTkUARTDataBits_t kDefaultDataBits = kUARTDataBitsEight;

  protected:
    /** The internal storage of the UART config */
    UARTConfig_t _config;
};
