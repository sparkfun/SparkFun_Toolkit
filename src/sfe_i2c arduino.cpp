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

#include "sfe_i2c_arduino.h"

int8_t SFE_I2C_Arduino::init(uint8_t devAddr)
{
    // Set default I2C port to Wire
    return init(devAddr, Wire);
}

int8_t SFE_I2C_Arduino::init(uint8_t devAddr, TwoWire& i2cPort, bool beginBus)
{
    // Store provided values
    _devAddr = devAddr;
    _i2cPort = &i2cPort;
    
    // Null pointer check
    if (!_i2cPort)
        return SFE_BUS_E_NULL_PTR;

    // Begin bus if requested
    if(beginBus)
        _i2cPort->begin();

    // Success
    return SFE_BUS_OK;
}

int8_t SFE_I2C_Arduino::ping()
{
    // Null pointer check
    if (!_i2cPort)
        return SFE_BUS_E_NULL_PTR;

    // Begin and end transmission to check for ACK response
    _i2cPort->beginTransmission(_devAddr);
    uint8_t result = _i2cPort->endTransmission();

    // Check result
    if(result == 0)
        return SFE_BUS_OK;
    else if(result == 1)
        return SFE_BUS_E_DATA_TOO_LONG;
    else if((result == 2) || (result == 3))
        return SFE_BUS_E_NO_RESPONSE;
    else if(result == 5)
        return SFE_BUS_E_TIMEOUT;
    else
        return SFE_BUS_E_UNKNOWN;
}

int8_t SFE_I2C_Arduino::writeRegisters(uint8_t regAddr, const uint8_t *data, uint8_t numBytes)
{
    // Null pointer check
    if (!_i2cPort)
        return SFE_BUS_E_NULL_PTR;

    // Start I2C message
    _i2cPort->beginTransmission(_devAddr);
    
    // Write register address
    _i2cPort->write(regAddr);

    // Write data
    _i2cPort->write(data, numBytes);
    
    // End I2C message
    uint8_t result = _i2cPort->endTransmission();

    // Check result
    if(result == 0)
        return SFE_BUS_OK;
    else if(result == 1)
        return SFE_BUS_E_DATA_TOO_LONG;
    else if((result == 2) || (result == 3))
        return SFE_BUS_E_NO_RESPONSE;
    else if(result == 5)
        return SFE_BUS_E_TIMEOUT;
    else
        return SFE_BUS_E_UNKNOWN;
}

int8_t SFE_I2C_Arduino::readRegisters(uint8_t regAddr, uint8_t *data, uint8_t numBytes)
{
    // Null pointer check
    if (!_i2cPort)
        return SFE_BUS_E_NULL_PTR;

    // Write desired register address
    _i2cPort->beginTransmission(_devAddr);
    _i2cPort->write(regAddr);
    uint8_t result = _i2cPort->endTransmission();

    // Check result
    if(result == 1)
        return SFE_BUS_E_DATA_TOO_LONG;
    else if((result == 2) || (result == 3))
        return SFE_BUS_E_NO_RESPONSE;
    else if(result == 5)
        return SFE_BUS_E_TIMEOUT;
    else if(result != 0)
        return SFE_BUS_E_UNKNOWN;

    // Read data from device
    uint8_t numRead = _i2cPort->requestFrom(_devAddr, numBytes);

    // Read data into buffer one byte at a time
    for(uint16_t i = 0; i < numRead; i++)
    {
        data[i] = _i2cPort->read();
    }

    // Check number of bytes read
    if(numRead < numBytes)
        return SFE_BUS_W_UNDER_READ;
    else
        return SFE_BUS_OK;
}
