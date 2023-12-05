/*
sfeTkArdI2C.cpp

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

#include "sfeTkArdI2C.h"

#define kMaxI2CBufferLength 32

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init - pass in already setup wire port ...
//
bool sfeTkArdI2C::init(TwoWire &wirePort, uint8_t addr, bool bInit)
{
    // if we don't have a wire port already
    if (!_i2cPort)
    {
        // use the pass in port
        _i2cPort = &wirePort;

        if (bInit)
            _i2cPort->begin();
    }

    setAddress(addr);
    return true;
}

//---------------------------------------------------------------------------------
// init()
//
// no parameters version of init. Setups a a wire port if needed.
//
bool sfeTkArdI2C::init(uint8_t addr)
{
    // no port yet, do the default version of it
    if (!_i2cPort)
        return init(Wire, addr);

    // We have a port, so arcady init'd - right?
    return true;
}

//---------------------------------------------------------------------------------
// init()
//
// no parameters version of init. Setups a a wire port if needed.
//
bool sfeTkArdI2C::init(void)
{
    // call with our currently set address ...
    return init(address());
}
//---------------------------------------------------------------------------------
// ping()
//
// Ping an I2C address to see if something is there.
//
bool sfeTkArdI2C::ping()
{
    // no port, no
    if (!_i2cPort)
        return false;

    _i2cPort->beginTransmission(address());
    return _i2cPort->endTransmission() == 0;
}

//---------------------------------------------------------------------------------
// writeRegisterByte()
//
// Writes a byte to a given register.
//
// Returns true on success, false on failure
//
bool sfeTkArdI2C::writeRegisterByte(uint8_t devReg, uint8_t dataToWrite)
{
    if (!_i2cPort)
        return false;

    // do the Arduino I2C work
    _i2cPort->beginTransmission(address());
    _i2cPort->write(devReg);
    _i2cPort->write(dataToWrite);
    return _i2cPort->endTransmission() == 0;
}
//---------------------------------------------------------------------------------
// writeRegisterWord()
//
// Writes a world to a given register.
//
// Returns true on success, false on failure
//
bool sfeTkArdI2C::writeRegisterWord(uint8_t devReg, uint16_t dataToWrite)
{
    if (!_i2cPort)
        return false;

    return writeRegisterRegion(devReg, (uint8_t *)&dataToWrite, sizeof(u_int16_t)) == 0;
}

//---------------------------------------------------------------------------------
// writeRegisterRegion()
//
// Writes an array of bytes to a given register on the target address
//
// Returns the number of bytes written, < 0 is an error
//
int sfeTkArdI2C::writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length)
{
    if (!_i2cPort)
        return -1;

    _i2cPort->beginTransmission(address());
    _i2cPort->write(devReg);
    _i2cPort->write(data, (int)length);

    return _i2cPort->endTransmission() ? -1 : 0; // -1 = error, 0 = success
}

//---------------------------------------------------------------------------------
// readRegisterByte()
//
// Reads a byte to a given register.
//
// Returns true on success, false on failure
//
bool sfeTkArdI2C::readRegisterByte(uint8_t devReg, uint8_t &dataToRead)
{
    if (!_i2cPort)
        return false;

    // Return value
    uint8_t result = 0;

    int nData = 0;

    _i2cPort->beginTransmission(address());
    _i2cPort->write(devReg);
    _i2cPort->endTransmission();
    _i2cPort->requestFrom(address(), (uint8_t)1);

    while (_i2cPort->available()) // slave may send less than requested
    {
        result = _i2cPort->read(); // receive a byte as a proper uint8_t
        nData++;
    }

    if (nData == 1) // Only update outputPointer if a single byte was returned
        dataToRead = result;

    return (nData == 1);
}
//---------------------------------------------------------------------------------
// readRegisterWord()
//
// Reads a world to a given register.
//
// Returns true on success, false on failure
//
bool sfeTkArdI2C::readRegisterWord(uint8_t devReg, uint16_t &dataToRead)
{
    if (!_i2cPort)
        return false;

    return readRegisterRegion(devReg, (uint8_t *)&dataToRead, sizeof(uint16_t)) == 2;
}

//---------------------------------------------------------------------------------
// readRegisterRegion()
//
// Reads an array of bytes to a given register on the target address
//
// Returns the number of bytes read, < 0 is an error
//
int sfeTkArdI2C::readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes)
{
    // got port
    if (!_i2cPort)
        return -1;

    uint16_t nOrig = numBytes; // original number of bytes.
    uint8_t nChunk;
    uint16_t nReturned;
    int i;                   // counter in loop
    bool bFirstInter = true; // Flag for first iteration - used to send devRegister

    while (numBytes > 0)
    {
        _i2cPort->beginTransmission(address());

        if (bFirstInter)
        {
            _i2cPort->write(devReg);
            bFirstInter = false;
        }

        if (_i2cPort->endTransmission() != 0)
            return -1; // error with the end transmission

        // We're chunking in data - keeping the max chunk to kMaxI2CBufferLength
        nChunk = numBytes > kMaxI2CBufferLength ? kMaxI2CBufferLength : numBytes;

        nReturned = _i2cPort->requestFrom((int)address(), (int)nChunk, (int)true);

        // No data returned, no dice
        if (nReturned == 0)
            return -1; // error

        // Copy the retrieved data chunk to the current index in the data segment
        for (i = 0; i < nReturned; i++)
            *data++ = _i2cPort->read();

        // Decrement the amount of data received from the overall data request amount
        numBytes = numBytes - nReturned;

    } // end while

    return nOrig - numBytes; // Success
}
