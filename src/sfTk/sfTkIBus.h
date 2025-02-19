/**
 * @file sfTkIBus.h
 * @brief Header file for the SparkFun Toolkit Device Bus Interface Definition.
 *
 * This file contains the interface declaration for the Communication Bus Interface
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "sfToolkit.h"
#include <stddef.h>

/**
 * @brief Define our error codes for the bus. Note Errors are negative, warnings/info positive,
 * but keep the same increment on the base.
 *
 */

/**
 * @brief Error code for when a bus system is not initalized.
 */
const sfTkError_t ksfTkErrBusNotInit = ksfTkErrFail * (ksfTkErrBaseBus + 1);

/**
 * @brief Returned when a bus system times out.
 */
const sfTkError_t ksfTkErrBusTimeout = ksfTkErrFail * (ksfTkErrBaseBus + 2);

/**
 * @brief Returned when a bus system does not respond.
 */
const sfTkError_t ksfTkErrBusNoResponse = ksfTkErrFail * (ksfTkErrBaseBus + 3);

/**
 * @brief Returned when the data to be sent is too long or received is too short.
 */
const sfTkError_t ksfTkErrBusDataTooLong = ksfTkErrFail * (ksfTkErrBaseBus + 4);

/**
 * @brief Returned when the bus settings are null, invalid or on set/initialised
 */
const sfTkError_t ksfTkErrBusNullSettings = ksfTkErrFail * (ksfTkErrBaseBus + 5);

/**
 * @brief Returned when the buffer is null or invalid.
 */
const sfTkError_t ksfTkErrBusNullBuffer = ksfTkErrFail * (ksfTkErrBaseBus + 6);

/**
 * @brief Returned when the bus is under read. Warning
 */
const sfTkError_t ksfTkErrBusUnderRead = ksfTkErrBaseBus + 7;

/**
 * @brief Returned when the bus is not enabled. Warning
 */
const sfTkError_t ksfTkErrBusNotEnabled = ksfTkErrBaseBus + 8;

/**
 * @brief Interface that defines the communication bus for the SparkFun Electronics Toolkit.
 *
 * The bus interface defines the basic methods for reading and writing data to a device. Specific
 * bus implementations will extend this interface to provide the necessary functionality for the
 * desired bus type.
 */
class sfTkIBus
{
  public:
    /**
     * @brief Constructor
     */
    sfTkIBus()
    {
        _byteOrder = sftk_system_byteorder();
    }
    /**--------------------------------------------------------------------------
     *  @brief Send a single byte to the device*
     *  @param data Data to write.
     *
     *  @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeByte(uint8_t data) = 0;

    /**--------------------------------------------------------------------------
     *  @brief Send a word to the device.
     *  @param data Data to write.
     *
     *  @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeWord(uint16_t data) = 0;

    /**--------------------------------------------------------------------------
     *  @brief Send an array of data to the device.
     *  @param data Data to write.
     *  @param length - length of data.
     *
     *  @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeRegion(const uint8_t *data, size_t length) = 0;

    /**--------------------------------------------------------------------------
     *  @brief Write a single byte to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeRegisterByte(uint8_t devReg, uint8_t data) = 0;

    // Overload version
    sfTkError_t writeRegister(uint8_t devReg, uint8_t data)
    {
        return writeRegisterByte(devReg, data);
    }

    /**--------------------------------------------------------------------------
     * @brief Write a single word (16 bit) to the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to write.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t writeRegisterWord(uint8_t devReg, uint16_t data) = 0;

    // Overload version
    sfTkError_t writeRegister(uint8_t devReg, uint16_t data)
    {
        return writeRegisterWord(devReg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Writes a number of bytes starting at the given register's address.
     *
     *  @param devReg The device's register's address.
     *  @param data Data to write.
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length) = 0;

    // Overload version
    sfTkError_t writeRegister(uint8_t devReg, const uint8_t *data, size_t length)
    {
        return writeRegisterRegion(devReg, data, length);
    }

    /**--------------------------------------------------------------------------
     *  @brief Writes a number of bytes starting at the given register's 16-bit address.
     *
     *  @param devReg The device's register's address.
     *  @param data Data to write.
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length) = 0;

    // Overload version
    sfTkError_t writeRegister(uint16_t devReg, const uint8_t *data, size_t length)
    {
        return writeRegister16Region(devReg, data, length);
    }

    /**--------------------------------------------------------------------------
     *  @brief Writes a number of uint16's starting at the given register's 16-bit address.
     *
     *  @param devReg The device's register's address.
     *  @param data Data to write.
     *  @param length - length of data
     *
     *   @retval sfTkError_t ksfTkErrOk on successful execution
     *
     */
    virtual sfTkError_t writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length) = 0;

    // Overload version
    sfTkError_t writeRegister(uint16_t devReg, const uint16_t *data, size_t length)
    {
        return writeRegister16Region16(devReg, data, length);
    }
    /**--------------------------------------------------------------------------
     *  @brief Read a single byte from the given register
     *
     *  @param devReg The device's register's address.
     *  @param data Data to read.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     *
     */
    virtual sfTkError_t readRegisterByte(uint8_t devReg, uint8_t &data) = 0;

    // Overload version
    sfTkError_t readRegister(uint8_t devReg, uint8_t &data)
    {
        return readRegisterByte(devReg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Read a single word (16 bit) from the given register
     *
     *   @param devReg The device's register's address.
     *   @param data Data to read.
     *
     *   @retval sfTkError_t -  ksfTkErrOk on successful execution.
     */
    virtual sfTkError_t readRegisterWord(uint8_t devReg, uint16_t &data) = 0;

    // Overload version
    sfTkError_t readRegister(uint8_t devReg, uint16_t &data)
    {
        return readRegisterWord(devReg, data);
    }

    /**--------------------------------------------------------------------------
     *  @brief Reads a block of data from the given register.
     *
     *   @param reg The device's register's address.
     *   @param data Data to write.
     *   @param numBytes - length of data
     *   @param[out] readBytes - number of bytes read
     *
     *   @retval int returns ksfTkErrOk on success, or ksfTkErrFail code
     *
     */
    virtual sfTkError_t readRegisterRegion(uint8_t reg, uint8_t *data, size_t numBytes, size_t &readBytes) = 0;

    // Overload version
    sfTkError_t readRegister(uint8_t reg, uint8_t *data, size_t numBytes, size_t &readBytes)
    {
        return readRegisterRegion(reg, data, numBytes, readBytes);
    }

    /**
     * @brief Overloaded function to read a 32-bit value from the given 16-bit register address.
     *
     * This function reads a 32-bit value from the specified 16-bit register address.
     *
     * @param reg The device's 16-bit register's address.
     * @param[out] value The 32-bit value read from the register.
     *
     * @return sfTkError_t Returns ksfTkErrOk on success, or an error code on failure.
     */
    virtual sfTkError_t readRegister(uint16_t reg, uint32_t &value)
    {
        size_t readBytes = 0;

        // this is a reg 16 address - so call the method to manage this. Note - it manages the
        // byte swapping of the address.

        sfTkError_t retValue = readRegister16Region(reg, (uint8_t *)&value, sizeof(uint32_t), readBytes);

        // The data is a uint32 - byte swap the result?
        if (retValue == ksfTkErrOk && sftk_system_byteorder() != _byteOrder)
            value = sftk_byte_swap(value);

        return retValue;
    }
    /**--------------------------------------------------------------------------
     *  @brief Reads a block of data from the given 16-bit register address.
     *
     *   @param reg The device's 16 bit register's address.
     *   @param data Data to write.
     *   @param numBytes - length of data
     *   @param[out] readBytes - number of bytes read
     *
     *   @retval int returns ksfTkErrOk on success, or ksfTkErrFail code
     *
     */
    virtual sfTkError_t readRegister16Region(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes) = 0;

    // Overload version
    sfTkError_t readRegister(uint16_t reg, uint8_t *data, size_t numBytes, size_t &readBytes)
    {
        return readRegister16Region(reg, data, numBytes, readBytes);
    }
    /**--------------------------------------------------------------------------
     *  @brief Reads a block of data from the given 16-bit register address.
     *
     *   @param reg The device's 16 bit register's address.
     *   @param data Data to write.
     *   @param numBytes - length of data
     *   @param[out] readBytes - number of bytes read
     *
     *   @retval int returns ksfTkErrOk on success, or ksfTkErrFail code
     *
     */
    virtual sfTkError_t readRegister16Region16(uint16_t reg, uint16_t *data, size_t numBytes, size_t &readBytes) = 0;

    // Overload version
    sfTkError_t readRegister(uint16_t reg, uint16_t *data, size_t numBytes, size_t &readBytes)
    {
        return readRegister16Region16(reg, data, numBytes, readBytes);
    }

    virtual uint8_t type(void)
    {
        return 0;
    }
    /**
     * @brief Set the byte order for multi-byte data transfers
     *
     * @param order The byte order to set - set to either SFTK_MSBFIRST or SFTK_LSBFIRST. The default is SFTK_LSBFIRST
     *
     */
    void setByteOrder(sfTkByteOrder order)
    {
        _byteOrder = order;
    }

    /**
     * @brief Get the current byte order
     *
     * @retval The current byte order
     */
    sfTkByteOrder byteOrder(void)
    {
        return _byteOrder;
    }

  protected:
    /** flag to manage byte swapping */
    sfTkByteOrder _byteOrder;
};
