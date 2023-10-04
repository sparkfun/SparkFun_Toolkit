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

#include "sfe_spi_arduino.h"

int8_t SFE_SPI_Arduino::init(uint8_t csPin)
{
    // Set default SPI port to SPI
    return init(csPin, SPI);
}

int8_t SFE_SPI_Arduino::init(uint8_t csPin, SPIClass &spiPort, bool beginBus)
{
    // Set default SPI settings to 1MHz, MSB first, and Mode 0
    return init(csPin, SPISettings(1000000, MSBFIRST, SPI_MODE0), spiPort);
}

int8_t SFE_SPI_Arduino::init(uint8_t csPin, SPISettings spiSettings, SPIClass &spiPort, bool beginBus)
{
    // Store provided values
    _csPin = csPin;
    _spiSettings = spiSettings;
    _spiPort = &spiPort;
    
    // Null pointer check
    if (!_spiPort)
        return SFE_BUS_E_NULL_PTR;

    // Begin bus if requested
    if(beginBus)
        _spiPort->begin();

    // Ensure CS pin is set to output mode and driving high for now
    // Writing high before setting mode helps ensure signal stays high
    digitalWrite(_csPin, HIGH);
    pinMode(_csPin, OUTPUT);

    // Success
    return SFE_BUS_OK;
}

int8_t SFE_SPI_Arduino::writeRegisters(uint8_t regAddr, const uint8_t *data, uint8_t numBytes)
{
    // Null pointer check
    if (!_spiPort)
        return SFE_BUS_E_NULL_PTR;

    // Start SPI message
    _spiPort->beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    
    // Write register address with write bit
    _spiPort->transfer(regAddr & 0x7F);

    // Write data
    _spiPort->transfer((void*) data, numBytes);

    // End SPI message
    digitalWrite(_csPin, HIGH);
    _spiPort->endTransaction();

    // Success
    return SFE_BUS_OK;
}

int8_t SFE_SPI_Arduino::readRegisters(uint8_t regAddr, uint8_t *data, uint8_t numBytes)
{
    // Null pointer check
    if (!_spiPort)
        return SFE_BUS_E_NULL_PTR;

    // Start SPI message
    _spiPort->beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    
    // Write register address with read bit
    _spiPort->transfer(regAddr | 0x80);

    // Read data into buffer one byte at a time
    for(uint16_t i = 0; i < numBytes; i++)
    {
        data[i] = _spiPort->transfer(0x00);
    }

    // End SPI message
    digitalWrite(_csPin, HIGH);
    _spiPort->endTransaction();

    // Success
    return SFE_BUS_OK;
}
