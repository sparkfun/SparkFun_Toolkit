// sfe_bus.cpp
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// The following classes specify the behavior for communicating
// over the respective data buses: Inter-Integrated Circuit (I2C)
// and Serial Peripheral Interface (SPI). For ease of implementation
// an abstract interface (QwIDeviceBus) is used. 

#pragma once



#include <Wire.h>
#include <SPI.h>

// To repeatedly use this bus toolkit, it will need its own namespace
//namespace sfe_XXX {

// The following abstract class is used an interface for upstream implementation.
class QwIDeviceBus 
{
	public: 

		virtual bool ping(uint8_t i2cAddr) = 0;

		virtual	bool writeRegisterByte(uint8_t i2cAddr, uint8_t devReg, uint8_t data) = 0;

		virtual int writeRegisterRegion(uint8_t i2cAddr, uint8_t devReg, const uint8_t* data, uint16_t length) = 0;

		virtual int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes) = 0;

};

// The QwI2C device defines behavior for I2C implementation based around the TwoWire class (Wire).
// This is Arduino specific. 
class QwI2C : public QwIDeviceBus
{
	public: 

		QwI2C(void);

		bool init();

		bool init(TwoWire& wirePort, bool bInit=false);

		bool ping(uint8_t i2cAddr);

		bool writeRegisterByte(uint8_t i2cAddr, uint8_t devReg, uint8_t data);

		int writeRegisterRegion(uint8_t i2cAddr, uint8_t devReg, const uint8_t* data, uint16_t length);

		int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes);

	private: 

    TwoWire* _i2cPort;
};

// The SfeSPI class defines behavior for SPI implementation based around the SPIClass class (SPI).
// This is Arduino specific. 
// Paramaters like "i2cAddr" are kept although irrelevant to SPI due to the use of the abstract class
// as interface, QwIDeviceBus.
class SfeSPI : public QwIDeviceBus
{
	public:

		SfeSPI(void);

		bool init(uint8_t cs, bool bInit=false);

		bool init(SPIClass& spiPort, SPISettings& ismSPISettings, uint8_t cs,  bool bInit=false);

		bool ping(uint8_t i2cAddr);

		bool writeRegisterByte(uint8_t i2cAddr, uint8_t devReg, uint8_t data);

		int writeRegisterRegion(uint8_t i2cAddr, uint8_t devReg, const uint8_t* data, uint16_t length);

		int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes);

	private:

		SPIClass* _spiPort; 
		// Settings are used for every transaction.
		SPISettings _sfeSPISettings;
		uint8_t _cs; 
};

//};
