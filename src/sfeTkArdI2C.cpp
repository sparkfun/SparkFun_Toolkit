/*
sfeTkArdI2C.cpp
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

#include "sfeTkArdI2C.h"

//---------------------------------------------------------------------------------
// init()
//
// Arduino version of init - pass in already setup wire port ...
//
sfeTkError_t sfeTkArdI2C::init(TwoWire &wirePort, uint8_t addr, bool bInit)
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
    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// init()
//
// no parameters version of init. Setups a a wire port if needed.
//
sfeTkError_t sfeTkArdI2C::init(uint8_t addr)
{
    // no port yet, do the default version of it
    if (!_i2cPort)
        return init(Wire, addr);

    // We have a port, so arcady init'd - right?
    return kSTkErrOk;
}

//---------------------------------------------------------------------------------
// init()
//
// no parameters version of init. Setups a a wire port if needed.
//
sfeTkError_t sfeTkArdI2C::init(void)
{
    // call with our currently set address ...
    return init(address());
}
//---------------------------------------------------------------------------------
// ping()
//
// Ping an I2C address to see if something is there.
//
sfeTkError_t sfeTkArdI2C::ping()
{
    // no port, no
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    _i2cPort->beginTransmission(address());
    return _i2cPort->endTransmission() == 0 ? kSTkErrOk : kSTkErrFail;
}

//---------------------------------------------------------------------------------
// writeByte()
//
// Writes a single byte to the device, without indexing to a register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::writeByte(uint8_t dataToWrite)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    // do the Arduino I2C work
    _i2cPort->beginTransmission(address());
    _i2cPort->write(dataToWrite);
    return _i2cPort->endTransmission() == 0 ? kSTkErrOk : kSTkErrFail;
}

//---------------------------------------------------------------------------------
// writeWord()
//
// Writes a word to the device, without indexing to a register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::writeWord(uint16_t dataToWrite)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    return writeRegion((uint8_t *)&dataToWrite, sizeof(uint16_t));
}

//---------------------------------------------------------------------------------
// writeRegion()
//
// Writes a word to the device, without indexing to a register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::writeRegion(const uint8_t *data, size_t length)
{
    return writeRegisterRegionAddress(nullptr, 0, data, length) == 0 ? kSTkErrOk : kSTkErrFail;
}

//---------------------------------------------------------------------------------
// writeRegisterByte()
//
// Writes a byte to a given register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::writeRegisterByte(uint8_t devReg, uint8_t dataToWrite)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    // do the Arduino I2C work
    _i2cPort->beginTransmission(address());
    _i2cPort->write(devReg);
    _i2cPort->write(dataToWrite);
    return _i2cPort->endTransmission() == 0 ? kSTkErrOk : kSTkErrFail;
}

//---------------------------------------------------------------------------------
// writeRegisterWord()
//
// Writes a word to a given register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::writeRegisterWord(uint8_t devReg, uint16_t dataToWrite)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    return writeRegisterRegion(devReg, (uint8_t *)&dataToWrite, sizeof(uint16_t));
}

/**
 * @brief Writes an array of bytes to a register on the target address. Supports any address size
 *
 * @param devReg The device's register's address - can be any size
 * @param regLength The length of the register address
 * @param data The data to write
 * @param length The length of the data buffer
 * @return sfeTkError_t Returns kSTkErrOk on success, or kSTkErrFail code
 */
sfeTkError_t sfeTkArdI2C::writeRegisterRegionAddress(uint8_t *devReg, size_t regLength, const uint8_t *data,
                                                     size_t length)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    _i2cPort->beginTransmission(address());

    if (devReg != nullptr && regLength > 0)
        _i2cPort->write(devReg, regLength);

    _i2cPort->write(data, (int)length);

    return _i2cPort->endTransmission() ? kSTkErrFail : kSTkErrOk;
}

//---------------------------------------------------------------------------------
// writeRegisterRegion()
//
// Writes an array of bytes to a given register on the target address
//
// Returns the number of bytes written, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::writeRegisterRegion(uint8_t devReg, const uint8_t *data, size_t length)
{
    return writeRegisterRegionAddress(&devReg, 1, data, length);
}

//---------------------------------------------------------------------------------
// write16BitRegisterRegion()
//
// Writes an array of bytes to a given 16-bit register on the target address
//
// Returns the number of bytes written, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::writeRegister16Region(uint16_t devReg, const uint8_t *data, size_t length)
{
    if (sfeToolkit::systemByteOrder() != _byteOrder)
        devReg = sfeToolkit::byte_swap(devReg);
    return writeRegisterRegionAddress((uint8_t *)&devReg, 2, data, length);
}

//---------------------------------------------------------------------------------
// write16BitRegisterRegion16()
//
// Writes an array of bytes to a given 16-bit register on the target address
//
// Returns the number of bytes written, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::writeRegister16Region16(uint16_t devReg, const uint16_t *data, size_t length)
{
    // if the system byte order is the same as the desired order, just send the buffer
    if (sfeToolkit::systemByteOrder() == _byteOrder)
        return writeRegisterRegionAddress((uint8_t *)&devReg, 2, (uint8_t *)data, length * 2);

    // okay, we need to swap
    devReg = sfeToolkit::byte_swap(devReg);
    uint16_t data16[length];
    for (size_t i = 0; i < length; i++)
        data16[i] = ((data[i] << 8) & 0xff00) | ((data[i] >> 8) & 0x00ff);

    return writeRegisterRegionAddress((uint8_t *)&devReg, 2, (uint8_t *)data16, length * 2);
}

/**
 * @brief Reads an array of bytes to a register on the target address. Supports any address size
 *
 * @param devReg The device's register's address - can be any size
 * @param regLength The length of the register address
 * @param data The data to buffer to read into
 * @param numBytes The length of the data buffer
 * @param readBytes[out] The number of bytes read
 * @return sfeTkError_t Returns kSTkErrOk on success, or kSTkErrFail code
 */
sfeTkError_t sfeTkArdI2C::readRegisterRegionAnyAddress(uint8_t *devReg, size_t regLength, uint8_t *data,
                                                       size_t numBytes, size_t &readBytes)
{

    // got port
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    // Buffer valid?
    if (!data)
        return kSTkErrBusNullBuffer;

    readBytes = 0;

    uint16_t nOrig = numBytes; // original number of bytes.
    uint8_t nChunk;
    uint16_t nReturned;
    uint16_t i;              // counter in loop
    bool bFirstInter = true; // Flag for first iteration - used to send devRegister

    while (numBytes > 0)
    {
        if (bFirstInter)
        {
            _i2cPort->beginTransmission(address());

            _i2cPort->write(devReg, regLength);

            if (_i2cPort->endTransmission(stop()) != 0)
                return kSTkErrFail; // error with the end transmission

            bFirstInter = false;
        }

        // We're chunking in data - keeping the max chunk to kMaxI2CBufferLength
        nChunk = numBytes > _bufferChunkSize ? _bufferChunkSize : numBytes;

        // Request the bytes. If this is the last chunk, always send a stop
        nReturned = _i2cPort->requestFrom((int)address(), (int)nChunk, (int)(nChunk == numBytes ? true : stop()));

        // No data returned, no dice
        if (nReturned == 0)
            return kSTkErrBusUnderRead; // error

        // Copy the retrieved data chunk to the current index in the data segment
        for (i = 0; i < nReturned; i++)
            *data++ = _i2cPort->read();

        // Decrement the amount of data received from the overall data request amount
        numBytes = numBytes - nReturned;

    } // end while

    readBytes = nOrig - numBytes; // Bytes read.

    return (readBytes == nOrig) ? kSTkErrOk : kSTkErrBusUnderRead; // Success
}

//---------------------------------------------------------------------------------
// readRegisterByte()
//
// Reads a byte to a given register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::readRegisterByte(uint8_t devReg, uint8_t &dataToRead)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    // Return value
    uint8_t result = 0;

    int nData = 0;

    _i2cPort->beginTransmission(address());
    _i2cPort->write(devReg);
    _i2cPort->endTransmission(stop());
    _i2cPort->requestFrom(address(), (uint8_t)1);

    while (_i2cPort->available()) // slave may send less than requested
    {
        result = _i2cPort->read(); // receive a byte as a proper uint8_t
        nData++;
    }

    if (nData == sizeof(uint8_t)) // Only update outputPointer if a single byte was returned
        dataToRead = result;

    return (nData == sizeof(uint8_t) ? kSTkErrOk : kSTkErrFail);
}

//---------------------------------------------------------------------------------
// readRegisterWord()
//
// Reads a word to a given register.
//
// Returns true on success, false on failure
//
sfeTkError_t sfeTkArdI2C::readRegisterWord(uint8_t devReg, uint16_t &dataToRead)
{
    if (!_i2cPort)
        return kSTkErrBusNotInit;

    size_t nRead;
    sfeTkError_t retval = readRegisterRegion(devReg, (uint8_t *)&dataToRead, sizeof(uint16_t), nRead);

    return (retval == kSTkErrOk && nRead == sizeof(uint16_t) ? kSTkErrOk : retval);
}

//---------------------------------------------------------------------------------
// readRegisterRegion()
//
// Reads an array of bytes to a given register on the target address
//
// Returns the number of bytes read, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::readRegisterRegion(uint8_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    return readRegisterRegionAnyAddress(&devReg, 1, data, numBytes, readBytes);
}

//---------------------------------------------------------------------------------
// read16BitRegisterRegion()
//
// Reads an array of bytes to a given 16-bit register on the target address
//
// Returns the number of bytes read, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::readRegister16Region(uint16_t devReg, uint8_t *data, size_t numBytes, size_t &readBytes)
{
    if (sfeToolkit::systemByteOrder() != _byteOrder)
        devReg = sfeToolkit::byte_swap(devReg);    
    return readRegisterRegionAnyAddress((uint8_t *)&devReg, 2, data, numBytes, readBytes);
}
//---------------------------------------------------------------------------------
// read16BitRegisterRegion16()
//
// Reads an array of bytes to a given 16-bit register on the target address
//
// Returns the number of bytes read, < 0 is an error
//
sfeTkError_t sfeTkArdI2C::readRegister16Region16(uint16_t devReg, uint16_t *data, size_t numWords, size_t &readWords)
{
    // if the system byte order is the same as the desired order, flip the address
    if (sfeToolkit::systemByteOrder() != _byteOrder)
        devReg = sfeToolkit::byte_swap(devReg);

    size_t readBytes; // variable to store I2C bytes written

    sfeTkError_t status = readRegisterRegionAnyAddress((uint8_t *)&devReg, 2, (uint8_t *)data, numWords * 2, readBytes);

    readWords = readBytes / 2; // convert bytes to words

    // Do we need to flip the byte order?
    if (status == kSTkErrOk && sfeToolkit::systemByteOrder() != _byteOrder)
    {
        for (size_t i = 0; i < numWords; i++)
            data[i] = sfeToolkit::byte_swap(data[i]); 
    }
    return status;
}