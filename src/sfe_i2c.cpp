/*
sfe_i2c.cpp

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

#include "sfe_i2c.h"

#define maxI2CBufferLength 32
// What we use for transfer chunk size
#define buffSize maxI2CBufferLength;

bool SfeI2C::init(TwoWire &wirePort, bool bInit)
{

    // if we don't have a wire port already
    if (!_i2cPort)
    {
        _i2cPort = &wirePort;

        if (bInit)
            _i2cPort->begin();
    }

    return true;
}

bool SfeI2C::init()
{
    if (!_i2cPort)
        return init(Wire);
    else
        return false;
}

bool SfeI2C::ping(uint8_t devAddr)
{

    if (!_i2cPort)
        return false;

    _i2cPort->beginTransmission(devAddr);
    return _i2cPort->endTransmission() == 0;
}

bool SfeI2C::writeRegisterByte(uint8_t devAddr, uint8_t devReg, uint8_t dataToWrite)
{

    if (!_i2cPort)
        return false;

    _i2cPort->beginTransmission(devAddr);
    _i2cPort->write(devReg);
    _i2cPort->write(dataToWrite);
    return _i2cPort->endTransmission() == 0;
}

int SfeI2C::writeRegisterRegion(uint8_t devAddr, uint8_t devReg, const uint8_t *data, uint16_t length)
{

    _i2cPort->beginTransmission(devAddr);
    _i2cPort->write(devReg);
    _i2cPort->write(data, (int)length);

    return _i2cPort->endTransmission() ? -1 : 0; // -1 = error, 0 = success
}

int SfeI2C::readRegisterRegion(uint8_t devAddr, uint8_t devReg, uint8_t *data, uint16_t numBytes)
{
    uint8_t nChunk;
    uint16_t nReturned;

    if (!_i2cPort)
        return -1;

    int i;                   // counter in loop
    bool bFirstInter = true; // Flag for first iteration - used to send devRegister

    while (numBytes > 0)
    {
        _i2cPort->beginTransmission(devAddr);

        if (bFirstInter)
        {
            _i2cPort->write(devReg);
            bFirstInter = false;
        }

        if (_i2cPort->endTransmission() != 0)
            return -1; // error with the end transmission

        // We're chunking in data - keeping the max chunk to kMaxI2CBufferLength
        nChunk = numBytes > buffSize ? buffSize : numBytes;

        nReturned = _i2cPort->requestFrom((int)devAddr, (int)nChunk, (int)true);

        // No data returned, no dice
        if (nReturned == 0)
            return -1; // error

        // Copy the retrieved data chunk to the current index in the data segment
        for (i = 0; i < nReturned; i++)
        {
            *data++ = _i2cPort->read();
        }

        // Decrement the amount of data recieved from the overall data request amount
        numBytes = numBytes - nReturned;

    } // end while

    return 0; // Success
}
