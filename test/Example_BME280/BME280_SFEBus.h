#pragma once

#include "secrets.h"
#include TOOLKIT_PATH
#include <SparkFunBME280.h>

class BME280SFEBus: public BME280
{
  public:
    BME280SFEBus();

    void setBus(SFEBusDevSettingsI2C *theDevSettings, SFEBusArduinoI2C *theBus);

    bool begin(SFEBusArduinoI2C *theBus = nullptr);

    float readTempC();

  private:
    bool readRegister(uint8_t regAddr, uint8_t* data, uint32_t numBytes);
    SFEBusArduinoI2C *_i2cBus;
    SFEBusDevSettingsI2C _devSettings;
};