// #include "secrets.h"
// #include TOOLKIT_PATH
#include <SparkFun_Toolkit.h>
#include "BME280_SFEBus.h"

SFEBusArduinoI2C myBus;
SFEBusDevSettingsI2C myBusSettings;
BME280SFEBus mySensor;

void setup() {
  Serial.begin(115200);
  while(!Serial) {delay(100);};
  Serial.println("Connected! Testing Bus Arduino I2C library with a modified BME280 library.");
  
  // Start the bus.
  int8_t result = myBus.begin();

  // Start the sensor.
  if(!mySensor.begin(&myBus))
  {
    Serial.println("The sensor did not respond. Please check wiring. Spinning...");
    while(1);
  }

}

void loop() {
  Serial.print("Temp: ");
  Serial.print(mySensor.readTempC(), 2);
  Serial.println(" degC");

  delay(100);
}