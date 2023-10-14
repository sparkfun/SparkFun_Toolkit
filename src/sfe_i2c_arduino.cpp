/*
  SPDX-License-Identifier: MIT

  Copyright (c) 2023 SparkFun Electronics
*/

#include "sfe_i2c_arduino.h"

int8_t SFEBusArduinoI2C::begin(void)
{
  if(!_i2cBus)
    _i2cBus = &Wire;

  return begin(_i2cBus);
}

int8_t SFEBusArduinoI2C::begin(TwoWire &wirePort)
{
  _i2cBus = wirePort;

  if(!_i2cBus)
    return SFE_BUS_E_NULL_PTR;

  _i2cBus->begin();

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::end(void)
{
  if(!_i2cBus)
    return SFE_BUS_E_NULL_PTR;

  _i2cBus->end();

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::ping(const uint8_t *devAddr)
{
  // Null pointer check
  if (!_i2cBus)
    return SFE_BUS_E_NULL_PTR;

  // Begin and end transmission to check for ACK response
  _i2cBus->beginTransmission(*devAddr);

  return _mapError(_i2cBus->endTransmission());
}

int8_t SFEBusArduinoI2C::ping(const SFEBusDevSettingsI2C *devSettings)
{
  return ping(&devSettings->devAddr);
}

int8_t SFEBusArduinoI2C::writeRegisterBytes(const SFEBusDevSettingsI2C *devSettings, const uint8_t regAddr, const uint8_t *data, const uint32_t numBytes)
{
  // TODO: Implement
  return SFE_BUS_E_UNKNOWN;
}

int8_t SFEBusArduinoI2C::readRegisterBytes(const SFEBusDevSettingsI2C *devSettings, const uint8_t regAddr, uint8_t *data, const uint32_t numBytes)
{
  // TODO: Implement
  return SFE_BUS_E_UNKNOWN;
}

int8_t SFEBusArduinoI2C::writeBytes(const SFEBusDevSettingsI2C *devSettings, const uint8_t *data, uint32_t numBytes)
{
  // TODO: Implement
  return SFE_BUS_E_UNKNOWN;
}

int8_t SFEBusArduinoI2C::readBytes(const SFEBusDevSettingsI2C *devSettings, uint8_t *data, uint32_t numBytes)
{
  // TODO: Implement
  return SFE_BUS_E_UNKNOWN;
}

int8_t SFEBusArduinoI2C::setBufferSize(const uint32_t bufferSize)
{
  _i2cBufferSize = bufferSize;

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::getBufferSize(uint32_t *bufferSize)
{
  *bufferSize = _i2cBufferSize;

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::setBusFrequency(const uint32_t frequency)
{
  if (!_i2cBus)
    return SFE_BUS_E_NULL_PTR;
  
  _i2cBus->setClock(frequency);
  _busFrequency = frequency;

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::getBusFrequency(uint32_t *frequency)
{
  *frequency = _busFrequency;

  return SFE_BUS_OK;
}

int8_t SFEBusArduinoI2C::_mapError(const uint8_t error)
{
  if(!error)
      return SFE_BUS_OK;
  else if(error == 1)
      return SFE_BUS_E_DATA_TOO_LONG;
  else if((error == 2) || (error == 3))
      return SFE_BUS_E_NO_RESPONSE;
  else if(error == 5)
      return SFE_BUS_E_TIMEOUT;
  else
      return SFE_BUS_E_UNKNOWN;
}

// int8_t SFE_I2C_Arduino::writeRegisters(uint8_t regAddr, const uint8_t *data, uint8_t numBytes)
// {
//     // Null pointer check
//     if (!_i2cPort)
//         return SFE_BUS_E_NULL_PTR;

//     // Start I2C message
//     _i2cPort->beginTransmission(_devAddr);
    
//     // Write register address
//     _i2cPort->write(regAddr);

//     // Write data
//     _i2cPort->write(data, numBytes);
    
//     // End I2C message
//     uint8_t result = _i2cPort->endTransmission();

//     // Check result
//     if(result == 0)
//         return SFE_BUS_OK;
//     else if(result == 1)
//         return SFE_BUS_E_DATA_TOO_LONG;
//     else if((result == 2) || (result == 3))
//         return SFE_BUS_E_NO_RESPONSE;
//     else if(result == 5)
//         return SFE_BUS_E_TIMEOUT;
//     else
//         return SFE_BUS_E_UNKNOWN;
// }

// int8_t SFE_I2C_Arduino::readRegisters(uint8_t regAddr, uint8_t *data, uint8_t numBytes)
// {
//     // Null pointer check
//     if (!_i2cPort)
//         return SFE_BUS_E_NULL_PTR;

//     // Write desired register address
//     _i2cPort->beginTransmission(_devAddr);
//     _i2cPort->write(regAddr);
//     // uint8_t result = _i2cPort->endTransmission();
//     int8_t result = _mapError(_i2cPort->endTransmission());
//     // Check result
//     if(result == 1)
//         return SFE_BUS_E_DATA_TOO_LONG;
//     else if((result == 2) || (result == 3))
//         return SFE_BUS_E_NO_RESPONSE;
//     else if(result == 5)
//         return SFE_BUS_E_TIMEOUT;
//     else if(result != 0)
//         return SFE_BUS_E_UNKNOWN;

//     // Read data from device
//     uint8_t numRead = _i2cPort->requestFrom(_devAddr, numBytes);

//     // Read data into buffer one byte at a time
//     for(uint16_t i = 0; i < numRead; i++)
//     {
//         data[i] = _i2cPort->read();
//     }

//     // Check number of bytes read
//     if(numRead < numBytes)
//         return SFE_BUS_W_UNDER_READ;
//     else
//         return SFE_BUS_OK;
// }
