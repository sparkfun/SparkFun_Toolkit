
// sfeTkBusSPI.cpp - Arduino SPI implementation for the toolkit

/*
The MIT License (MIT)

Copyright (c) 2022 SparkFun Electronics
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

#pragma once

#include "sfeTkBusSPI.h "

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
bool sfeTkBusSPI::init(SPIClass &spiPort, SPISettings &busSPISettings, bool bInit)
{
    // if we don't have a SPI port already
    if (!_spiPort)
    {
        _spiPort = &spiPort;

        if (bInit)
            _spiPort->begin();
    }

    // SPI settings are needed for every transaction
    _sfeSPISettings = busSPISettings;

    return true;
}

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init.
//
bool sfeTkBusSPI::init(bool bInit)
{
    // If the transaction settings are not provided by the user they are built here.
    SPISettings spiSettings = SPISettings(3000000, MSBFIRST, SPI_MODE3);

    // In addition of the port is not provided by the user, it defaults to SPI here.
    return init(SPI, spiSettings, bInit);
}

//---------------------------------------------------------------------------------
// writeRegisterByte()
//
// Writes a byte to a given register.
//
// Returns true on success, false on failure
//
bool sfeTkBusSPI::writeRegisterByte(uint8_t cs, uint8_t devReg, uint8_t dataToWrite)
{

    if (!_spiPort)
        return false;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(cs, LOW);

    _spiPort->transfer(devReg);
    _spiPort->transfer(dataToWrite);

    // End communication
    digitalWrite(cs, HIGH);
    _spiPort->endTransaction();

    return true;
}

//---------------------------------------------------------------------------------
// writeRegisterRegion()
//
// Writes an array of bytes to a given register on the target address
//
// Returns the number of bytes written, < 0 is an error
//
int sfeTkBusSPI::writeRegisterRegion(uint8_t cs, uint8_t devReg, const uint8_t *data, uint16_t length)
{
    if (!_spiPort)
        return -1;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs, LOW);
    _spiPort->transfer(devReg);

    for (int i = 0; i < length; i++)
        _spiPort->transfer(*data++);

    // End communication
    digitalWrite(cs, HIGH);
    _spiPort->endTransaction();

    return length;
}

//---------------------------------------------------------------------------------
// readRegisterRegion()
//
// Reads an array of bytes to a given register on the target address
//
// Returns the number of bytes read, < 0 is an error
//
int sfeTkBusSPI::readRegisterRegion(uint8_t cs, uint8_t devReg, uint8_t *data, uint16_t numBytes)
{
    if (!_spiPort)
        return -1;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);

    // Signal communication start
    digitalWrite(cs, LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spiPort->transfer(devReg | kSPIReadBit);

    for (int i = 0; i < numBytes; i++)
        *data++ = _spiPort->transfer(0x00);

    // End transaction
    digitalWrite(cs, HIGH);
    _spiPort->endTransaction();

    return numByte;
}