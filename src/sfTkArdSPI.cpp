/**
 * @file sfTkArdSPI.h
 * @brief Implementation file for the SparkFun Toolkit Arduino SPI driver.
 *
 * This file contains the Arduino SPI driver for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#include "sfTkArdSPI.h"
#include <Arduino.h>

// Note: A leading "1" must be added to transfer with register to indicate a "read"
// Note to our future selves:
//   This works / is required on both the ISM330 and MMC5983,
//   but will cause badness with other SPI devices.
//   We may need to add an alternate method if we ever add another SPI device.
#define kSPIReadBit 0x80

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init. Will take in a defined SPI port/settings
//
sfTkError_t sfTkArdSPI::init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit)
{
    // if we don't have a SPI port already
    if (!_spiPort)
    {
        _spiPort = &spiPort;

        if (bInit)
            _spiPort->begin();
    }

    setCS(csPin);

    // SPI settings are needed for every transaction
    _sfeSPISettings = busSPISettings;

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init.
//
sfTkError_t sfTkArdSPI::init(uint8_t csPin, bool bInit)
{
    // If the transaction settings are not provided by the user they are built here.
    SPISettings spiSettings = SPISettings(3000000, MSBFIRST, SPI_MODE3);

    // In addition of the port is not provided by the user, it defaults to SPI here.
    return init(SPI, spiSettings, csPin, bInit);
}

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init.
//
sfTkError_t sfTkArdSPI::init(bool bInit)
{
    return init(cs(), bInit);
}

//---------------------------------------------------------------------------------
// writeRegion()
//
// Writes an array of data to the device without indexing to a register.
//
// Returns ksfTkErrOk on success
//
sfTkError_t sfTkArdSPI::writeRegion(const uint8_t *dataToWrite, size_t length)
{

    if (!_spiPort)
        return ksfTkErrBusNotInit;

    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(cs(), LOW);

    for (size_t i = 0; i < length; i++)
        _spiPort->transfer(*dataToWrite++);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// writeRegisterRegion()
//
// Writes an array of bytes to a given register on the target address
//
// Returns ksfTkErrOk on success
//
sfTkError_t sfTkArdSPI::writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings before work
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);

    _spiPort->transfer(devReg);

    for (size_t i = 0; i < length; i++)
        _spiPort->transfer(*data++);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return ksfTkErrOk;
}

// 16 bit address version ...
sfTkError_t sfTkArdSPI::writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings before work
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);
    _spiPort->transfer16(devReg);

    for (size_t i = 0; i < length; i++)
        _spiPort->transfer(*data++);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// 16 bit address and data version ...
sfTkError_t sfTkArdSPI::writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings before work
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);
    _spiPort->transfer16(devReg);

    for (size_t i = 0; i < length; i++)
        _spiPort->transfer16(*data++);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// readRegisterRegion()
//
// Reads an array of bytes to a given register on the target address
//
// Returns ksfTkErrOk on success
//
sfTkError_t sfTkArdSPI::readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spiPort->transfer(devReg | kSPIReadBit);

    for (size_t i = 0; i < numBytes; i++)
        *data++ = _spiPort->transfer(0x00);

    // End transaction
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    readBytes = numBytes;

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// readRegister16Region()
//
// Reads an array of bytes to a given a 16 bit register on the target address
//
// Returns ksfTkErrOk on success
//
sfTkError_t sfTkArdSPI::readRegister16Region(uint16_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spiPort->transfer16(devReg | kSPIReadBit);

    for (size_t i = 0; i < numBytes; i++)
        *data++ = _spiPort->transfer(0x00);

    // End transaction
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    readBytes = numBytes;

    return ksfTkErrOk;
}

//---------------------------------------------------------------------------------
// readRegister16Region16()
//
// Reads an array of uint16 to a given a 16 bit register on the target address
//
// Returns ksfTkErrOk on success
//
sfTkError_t sfTkArdSPI::readRegister16Region16(uint16_t devReg, uint16_t *data, size_t numBytes, size_t &readWords)
{
    if (!_spiPort)
        return ksfTkErrBusNotInit;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs(), LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spiPort->transfer16(devReg);

    for (size_t i = 0; i < numBytes; i++)
        *data++ = _spiPort->transfer16(0x00);

    // End transaction
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    readWords = numBytes;

    return ksfTkErrOk;
}