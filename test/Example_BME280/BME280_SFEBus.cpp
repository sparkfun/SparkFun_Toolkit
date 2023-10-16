/*
  SPDX-License-Identifier: MIT

  Copyright (c) 2023 SparkFun Electronics
*/

#include "BME280_SFEBus.h"

BME280SFEBus::BME280SFEBus(): _i2cBus{nullptr}
{
  settings.commInterface = I2C_MODE;

  settings.I2CAddress = 0x77;
  
  settings.runMode = 3;
  settings.tStandby = 0;
  settings.filter = 0;
  settings.tempOverSample = 1;
  settings.pressOverSample = 1;
  settings.humidOverSample = 1;
  settings.tempCorrection = 0.f;
}

void BME280SFEBus::setBus(SFEBusDevSettingsI2C *theDevSettings, SFEBusArduinoI2C *theBus)
{
  _i2cBus = theBus;
  _devSettings = *theDevSettings;
}


bool BME280SFEBus::begin(SFEBusArduinoI2C *theBus)
{
  if (theBus == nullptr)
    theBus = new SFEBusArduinoI2C();

  if (!theBus)
    return false;

  _devSettings.devAddr = settings.I2CAddress;

  setBus(&_devSettings, theBus);

  uint8_t chipID = 0;

  readRegister(BME280_CHIP_ID_REG, &chipID, 1);

}

float BME280SFEBus::readTempC()
{
  
}

bool BME280SFEBus::readRegister(uint8_t regAddr, uint8_t* data, uint32_t numBytes)
{
  _i2cBus->readRegisterBytes((SFEBusDevSettings*)&_devSettings, regAddr, data, numBytes);
}