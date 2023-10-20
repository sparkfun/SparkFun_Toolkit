#include <SparkFun_Toolkit.h>
#include "OPT4048_SFEBus.h"

SFEBusArduinoI2C myBus;
sfeOPT4048ArduinoI2C myColor;

void setup()
{
  Serial.begin(115200);
  while(!Serial){delay(100);};
  Serial.println("OPT4048 Example using SFE Bus");

  myBus.begin();

  if(!myColor.begin(&myBus))
  {
    Serial.println("OPT4048 not detected - check wiring or that your I2C address is correct");
    while(1);
  }

  bool setup = myColor.setBasicSetup();

  Serial.print("Setup returned: ");
  Serial.println(setup);

  Serial.println("Ready to go!");
}

void loop()
{
  Serial.print("CIEx: ");
  Serial.print(myColor.getCIEx());
  Serial.print(" CIEy: ");
  Serial.println(myColor.getCIEy());

  delay(200);
}