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


bool BME280SFEBus::beginI2C(SFEBusArduinoI2C *theBus)
{
  if (theBus == nullptr)
    theBus = new SFEBusArduinoI2C();

  if (!theBus)
    return false;

  _devSettings.devAddr = settings.I2CAddress;

  setBus(&_devSettings, theBus);

  

  uint8_t chipID = begin();

  if(chipID == 0x58) return(true); //Begin normal init with these settings. Should return chip ID of 0x58 for BMP
	if(chipID == 0x60) return(true); //Begin normal init with these settings. Should return chip ID of 0x60 for BME
	return(false);
}

float BME280SFEBus::readTempC()
{
      // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
    // t_fine carries fine temperature as global value

    //get the reading (adc_T);
    uint8_t buffer[3];
    readRegisterRegion(buffer, BME280_TEMPERATURE_MSB_REG, 3);
    int32_t adc_T = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((buffer[2] >> 4) & 0x0F);

    //By datasheet, calibrate
    int64_t var1, var2;

    var1 = ((((adc_T>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((int32_t)calibration.dig_T1)) * ((adc_T>>4) - ((int32_t)calibration.dig_T1))) >> 12) *
    ((int32_t)calibration.dig_T3)) >> 14;
    t_fine = var1 + var2;
    float output = (t_fine * 5 + 128) >> 8;

    output = output / 100 + settings.tempCorrection;
    
    return output;
}

bool BME280SFEBus::readRegisterFull(uint8_t regAddr, uint8_t* data, uint32_t numBytes)
{
  _i2cBus->readRegisterBytes((SFEBusDevSettings*)&_devSettings, regAddr, data, numBytes);

  return true;
}

uint8_t BME280SFEBus::readRegister(uint8_t regAddr)
{
  uint8_t regVal;
  readRegisterFull(regAddr, &regVal, 1);

  return regVal;
}