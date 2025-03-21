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

#include "sfTkError.h"
#include "sfTkISerial.h"
#include <cstdint>

// Enums follow the Arduino Serial API constants.

typedef enum _sfTkUARTParity
{
    kUARTParityEven         = 0x1ul,
    kUARTParityOdd          = 0x2ul,
    kUARTParityNone         = 0x3ul,
    kUARTParityMark         = 0x4ul,
    kUARTParitySpace        = 0x5ul
} sfTkUARTParity_t;

inline const char* parityToString(sfTkUARTParity_t parity) {
    switch(parity) {
        case kUARTParityEven:  return "Even";
        case kUARTParityOdd:   return "Odd";
        case kUARTParityNone:  return "None";
        case kUARTParityMark:  return "Mark";
        case kUARTParitySpace: return "Space";
        default:               return "Unknown";
    }
}

typedef enum _sfTkUARTStopBits
{
    kUARTStopBitsOne        = 0x10ul,
    kUARTStopBitsOneAndHalf = 0x20ul,
    kUARTStopBitsTwo        = 0x30ul
} sfTkUARTStopBits_t;

inline const char* stopBitsToString(sfTkUARTStopBits_t stopBits) {
    switch(stopBits) {
        case kUARTStopBitsOne:        return "One";
        case kUARTStopBitsOneAndHalf: return "OneAndHalf";
        case kUARTStopBitsTwo:        return "Two";
        default:                      return "Unknown";
    }
}

typedef enum _sfTkUARTDataBits
{
    kUARTDataBitsFive       = 0x100ul,
    kUARTDataBitsSix        = 0x200ul,
    kUARTDataBitsSeven      = 0x300ul,
    kUARTDataBitsEight      = 0x400ul,
} sfTkUARTDataBits_t;

inline const uint8_t dataBitsToValue(sfTkUARTDataBits_t dataBits) {
    switch(dataBits) {
        case kUARTDataBitsFive:   return 5;
        case kUARTDataBitsSix:    return 6;
        case kUARTDataBitsSeven:  return 7;
        case kUARTDataBitsEight:  return 8;
        default:                  return 0; // Invalid data bits
    }
}

typedef struct _sfTkUARTConfig
{
    uint32_t baudRate;
    sfTkUARTDataBits_t dataBits;
    sfTkUARTParity_t parity;
    sfTkUARTStopBits_t stopBits;
} sfTkUARTConfig_t;

class sfTkIUART : public sfTkISerial
{
public:
    /**
     *  @brief Constructor for the UART bus
     * 
     */
    sfTkIUART(void)
    {
        _config.baudRate = kDefaultBaudRate;
        _config.parity = kUARTParityNone;
        _config.stopBits = kUARTStopBitsOne;
        _config.dataBits = kUARTDataBitsEight;
    }

    /**
     * @brief Constructor for the UART bus with the baud rate passed in
     * 
     * @param baudRate
     */
    sfTkIUART(uint32_t baudRate)
    {
        _config.baudRate = baudRate;
        _config.parity = kUARTParityNone;
        _config.stopBits = kUARTStopBitsOne;
        _config.dataBits = kUARTDataBitsEight;
    }

    /**
     * @brief Constructor for the UART bus with a configuration passed in
     * 
     * @param config
     */
    sfTkIUART(sfTkUARTConfig_t config) : _config(config)
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
        if(_config.baudRate != baudRate)
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
        if(_config.stopBits != stopBits)
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
        if(_config.parity != parity)
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
        if(_config.dataBits != dataBits)
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
     * @param config The config struct to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    virtual sfTkError_t setConfig(const sfTkUARTConfig_t config)
    {
        if(_config.baudRate != config.baudRate || 
            _config.stopBits != config.stopBits || 
            _config.parity != config.parity || 
            _config.dataBits != config.dataBits)
                _config = config;

        return ksfTkErrOk;
    }

    /**
     * @brief getter for the internal config object
     * 
     * @return sfTkUARTConfig_t 
     */
    virtual sfTkUARTConfig_t config(void) const
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
    sfTkUARTConfig_t _config;
};
