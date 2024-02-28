
// sfeTkArdSPI.cpp - Arduino SPI implementation for the toolkit

/*
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

*/

#include "sfeTkArdSPI.h"
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
sfeTkError_t sfeTkArdSPI::init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t csPin, bool bInit)
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

    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init.
//
sfeTkError_t sfeTkArdSPI::init(uint8_t csPin, bool bInit)
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
sfeTkError_t sfeTkArdSPI::init(bool bInit)
{
    return init(cs(), bInit);
}

//---------------------------------------------------------------------------------
// writeRegisterByte()
//
// Writes a single byte to the device.
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::writeByte(uint8_t dataToWrite)
{

    if (!_spiPort)
        return kSTkErrBusNotInit;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(cs(), LOW);

    _spiPort->transfer(dataToWrite);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// writeRegisterByte()
//
// Writes a byte to a given register.
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::writeRegisterByte(uint8_t devReg, uint8_t dataToWrite)
{

    if (!_spiPort)
        return kSTkErrBusNotInit;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(cs(), LOW);

    _spiPort->transfer(devReg);
    _spiPort->transfer(dataToWrite);

    // End communication
    digitalWrite(cs(), HIGH);
    _spiPort->endTransaction();

    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// writeRegisterWord()
//
// Writes a world to a given register.
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::writeRegisterWord(uint8_t devReg, uint16_t dataToWrite)
{
    return writeRegisterRegion(devReg, (uint8_t *)&dataToWrite, sizeof(uint8_t)) > 0;
}
//---------------------------------------------------------------------------------
// writeRegisterRegion()
//
// Writes an array of bytes to a given register on the target address
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length)
{
    if (!_spiPort)
        return kSTkErrBusNotInit;

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

    return kSTkErrOk;
}

// 16 bit address version ...
sfeTkError_t sfeTkArdSPI::writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length)
{
    if (!_spiPort)
        return kSTkErrBusNotInit;

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

    return kSTkErrOk;
}

sfeTkError_t sfeTkArdSPI::readRegisterByte(uint8_t devReg, uint8_t &data)
{
    size_t nRead;
    sfeTkError_t retval = readRegisterRegion(devReg, (uint8_t *)&data, sizeof(uint8_t), nRead);

    return (retval == kSTkErrOk && nRead == sizeof(uint8_t) ? kSTkErrOk : retval);
}

sfeTkError_t sfeTkArdSPI::readRegisterWord(uint8_t devReg, uint16_t &data)
{
    size_t nRead;
    sfeTkError_t retval = readRegisterRegion(devReg, (uint8_t *)&data, sizeof(uint16_t), nRead);

    return (retval == kSTkErrOk && nRead == sizeof(uint16_t) ? kSTkErrOk : retval);
}
//---------------------------------------------------------------------------------
// readRegisterRegion()
//
// Reads an array of bytes to a given register on the target address
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    if (!_spiPort)
        return kSTkErrBusNotInit;

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

    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// readRegister16Region()
//
// Reads an array of bytes to a given a 16 bit register on the target address
//
// Returns kSTkErrOk on success
//
sfeTkError_t sfeTkArdSPI::readRegister16Region(uint16_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    if (!_spiPort)
        return kSTkErrBusNotInit;

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

    return kSTkErrOk;
}