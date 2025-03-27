

// Just a simple compile test for the Bus objects

#include "SparkFun_Toolkit.h"

sfTkArdI2C myI2C;
sfTkArdSPI mySPI;
sfTkArdUART myUART;
sfTkArdUARTBus myUARTBus(myUART);

void setup()
{
}

void loop()
{
    sftk_delay_ms(1000);
}
