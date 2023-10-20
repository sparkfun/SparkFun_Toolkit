#pragma once

#include <SparkFun_Toolkit.h>
#include <SparkFunBME280.h>

class BME280SFEBus: public BME280
{
  public:
    BME280SFEBus();

    void setBus(SFEBusDevSettingsI2C *theDevSettings, SFEBusArduinoI2C *theBus);

    bool beginI2C(SFEBusArduinoI2C *theBus = nullptr);

    float readTempC();

  private:
    bool readRegisterFull(uint8_t regAddr, uint8_t* data, uint32_t numBytes);
    uint8_t BME280SFEBus::readRegister(uint8_t regAddr);
    SFEBusArduinoI2C *_i2cBus;
    SFEBusDevSettingsI2C _devSettings;
};