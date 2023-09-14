/*
sfe_spi.h

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

#include "sfe_spi.h"
#define SPI_READ 0x80

bool SfeSPI::init(SPIClass &spiPort, SPISettings &busSPISettings, uint8_t cs, bool bInit)
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

    // The chip select pin can vary from platform to platform and project to project
    // and so it must be given by the user.
    if (!cs)
        return false;

    _cs = cs;

    return true;
}

bool SfeSPI::init(uint8_t cs, bool bInit)
{

    // If the transaction settings are not provided by the user they are built here.
    SPISettings spiSettings = SPISettings(3000000, MSB_FIRST, SPI_MODE3);

    // In addition of the port is not provided by the user, it defaults to SPI here.
    return init(SPI, spiSettings, cs, bInit);
}

bool SfeSPI::ping(uint8_t devAddr)
{
    return true;
}

bool SfeSPI::writeRegisterByte(uint8_t devAddr, uint8_t devReg, uint8_t dataToWrite)
{

    if (!_spiPort)
        return false;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(_cs, LOW);

    _spiPort->transfer(devReg);
    _spiPort->transfer(dataToWrite);

    // End communcation
    digitalWrite(_cs, HIGH);
    _spiPort->endTransaction();

    return true;
}

int SfeSPI::writeRegisterRegion(uint8_t devAddr, uint8_t devReg, const uint8_t *data, uint16_t length)
{

    int i;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(_cs, LOW);
    _spiPort->transfer(devReg);

    for (i = 0; i < length; i++)
    {
        _spiPort->transfer(*data++);
    }

    // End communication
    digitalWrite(_cs, HIGH);
    _spiPort->endTransaction();
    return 0;
}

int SfeSPI::readRegisterRegion(uint8_t devAddr, uint8_t devReg, uint8_t *data, uint16_t numBytes)
{
    if (!_spiPort)
        return -1;

    int i;

    // Apply settings
    _spiPort->beginTransaction(_sfeSPISettings);
    // Signal communication start
    digitalWrite(_cs, LOW);
    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    devReg = (devReg | SPI_READ);
    _spiPort->transfer(devReg);

    for (i = 0; i < numBytes; i++)
    {
        *data++ = _spiPort->transfer(0x00);
    }

    // End transaction
    digitalWrite(_cs, HIGH);
    _spiPort->endTransaction();
    return 0;
}
