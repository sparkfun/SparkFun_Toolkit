/**
 * @file sfTkArdUART.cpp
 * @brief Implementation file for the SparkFun Toolkit Arduino UART driver.
 *
 * This file contains the Arduino UART driver for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */
#include <stdarg.h>

#include <Arduino.h>
#include <HardwareSerial.h>

// clang-format off
#include "sfTkArdUART.h"
#include "sfTk/sfTkError.h"
#include "sfTk/sfTkISerial.h"
#include "sfTk/sfTkIUART.h"
// clang-format on

sfTkError_t sfTkArdUART::init(HardwareSerial &hwSerial, sfTkIUART::UARTConfig_t &config, bool bInit)
{
    _hwSerial = &hwSerial; // set the serial port

    _config = config; // set the config

    if (bInit)
        return _start(); // start the port

    return ksfTkErrOk;
}

sfTkError_t sfTkArdUART::init(HardwareSerial &hwSerial, uint32_t baudRate, bool bInit)
{
    _hwSerial = &hwSerial; // set the serial port

    _config.baudRate = baudRate; // set the baud rate

    if (bInit)
        return _start(); // start the port

    return ksfTkErrOk;
}

sfTkError_t sfTkArdUART::init(uint32_t baudRate, bool bInit)
{
    // issues here on some devices - $defineing out for now
#ifdef _THIS_IS_BROKEN
    // if we don't have a port already, use the default Arduino Serial.
    if (!_hwSerial)
        return init(Serial, baudRate, bInit);

    // We already have a UART setup, so it's already initialized. Change the baud rate.
    return setBaudRate(baudRate); // set the baud rate
#else
    return ksfTkErrFail;
#endif
}

sfTkError_t sfTkArdUART::init(sfTkIUART::UARTConfig_t config, bool bInit)
{
// issues here on some devices - $defineing out for now
#ifdef _THIS_IS_BROKEN
    // if we don't have a port already, use the default Arduino Serial.
    if (!_hwSerial)
        return init(Serial, config, bInit);

    if (bInit)
        return _start(); // start the port

    // We already have a UART setup, so it's already initialized.
    return ksfTkErrOk;
#else
    return ksfTkErrFail;
#endif
}

sfTkError_t sfTkArdUART::init()
{
    return init(kDefaultBaudRate, true);
}

sfTkError_t sfTkArdUART::write(const uint8_t *data, size_t len)
{
    if (!_hwSerial)
        return ksfTkErrSerialNotInit;

    return (_hwSerial->write(data, len) == len ? ksfTkErrOk : ksfTkErrSerialUnderRead);
}

sfTkError_t sfTkArdUART::write(const uint8_t data)
{
    if (!_hwSerial)
        return ksfTkErrSerialNotInit;

    return (_hwSerial->write(data) ? ksfTkErrOk : ksfTkErrFail);
}

sfTkError_t sfTkArdUART::read(uint8_t *data, size_t length, size_t &bytesRead)
{
    if (!_hwSerial)
        return ksfTkErrSerialNotInit;

    if (!data)
        return ksfTkErrSerialNullBuffer;

    if (length == 0)
        return ksfTkErrSerialDataTooLong; // nothing to read

    bytesRead = 0; // zero out value

    // #ifdef ARDUINO_ARCH_AVR
    bytesRead = _hwSerial->readBytes(data, length);
    // #else
    //     bytesRead = readBytes(data, length);
    // #endif

    if (bytesRead == 0)
        return ksfTkErrFail;

    return (bytesRead == length) ? ksfTkErrOk : ksfTkErrSerialUnderRead; // return success if all bytes read
}

sfTkError_t sfTkArdUART::read(uint8_t &data)
{
    size_t nRead;
    return read(&data, 1, nRead);
}

sfTkArdUART::operator bool()
{
    return (bool)*_hwSerial;
}

sfTkError_t sfTkArdUART::setBaudRate(const uint32_t baudRate)
{
    if (_config.baudRate != baudRate)
        _config.baudRate = baudRate; // set the baud rate

    return _start(); // start the port again
}

sfTkError_t sfTkArdUART::setStopBits(const sfTkUARTStopBits_t stopBits)
{
    if (_config.stopBits != stopBits)
        _config.stopBits = stopBits; // set the stop bits

    return _start(); // start the port again
}

sfTkError_t sfTkArdUART::setParity(const sfTkUARTParity_t parity)
{
    if (_config.parity != parity)
        _config.parity = parity; // set the baud rate

    return _start(); // start the port again
}

sfTkError_t sfTkArdUART::setDataBits(const sfTkUARTDataBits_t dataBits)
{
    if (_config.dataBits != dataBits)
        _config.dataBits = dataBits; // set the baud rate

    return _start(); // start the port again
}

sfTkError_t sfTkArdUART::setConfig(const uint32_t baudRate, const sfTkUARTDataBits_t dataBits,
                                   const sfTkUARTParity_t parity, const sfTkUARTStopBits_t stopBits)
{
    if (_config.baudRate != baudRate)
        _config.baudRate = baudRate;

    if (_config.dataBits != dataBits)
        _config.dataBits = dataBits;

    if (_config.parity != parity)
        _config.parity = parity;

    if (_config.stopBits != stopBits)
        _config.stopBits = stopBits;

    return _start(); // start the port again
}

sfTkError_t sfTkArdUART::_start(void)
{
    if (!_hwSerial)
        return ksfTkErrSerialNotInit;
    if (_running)
        end(); // close the port if already running
    // set the config
#ifdef ARDUINO_ARCH_ESP8266
    // ESP8266 does not support setting stop bits, parity, and data bits in a stanard manner.
    _hwSerial->begin(_config.baudRate);
#else
    _hwSerial->begin(_config.baudRate,
                     (uint32_t)_config.stopBits | (uint32_t)_config.parity | (uint32_t)_config.dataBits);
#endif
    if (!availableForWrite())
        return ksfTkErrSerialNotInit; // check if the port is available
    // set the running flag to true
    _running = true;

    return ksfTkErrOk;
}

void sfTkArdUART::end(void)
{
    _running = false; // set the running flag to false
    _hwSerial->end();
}

int sfTkArdUART::available(void)
{
    if (!_hwSerial)
        return 0;
    return _hwSerial->available();
}

int sfTkArdUART::availableForWrite(void)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->availableForWrite();
}

int sfTkArdUART::peek(void)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->peek();
}

void sfTkArdUART::flush(void)
{
    if (!_hwSerial)
        return;

    _hwSerial->flush();
}

void sfTkArdUART::setTimeout(unsigned long timeout)
{
    if (!_hwSerial)
        return;

    _hwSerial->setTimeout(timeout);
}

unsigned long sfTkArdUART::getTimeout()
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->getTimeout();
}

#if !defined(ARDUINO_ARCH_AVR) && !defined(ARDUINO_ARCH_ESP8266)
bool sfTkArdUART::find(CONSTVAR char *target)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->find(target);
}

bool sfTkArdUART::find(CONSTVAR uint8_t *target)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->find(target);
}

bool sfTkArdUART::find(CONSTVAR char *target, size_t length)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->find(target, length);
}

bool sfTkArdUART::find(CONSTVAR uint8_t *target, size_t length)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->find(target, length);
}

bool sfTkArdUART::find(char target)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->find(target);
}

bool sfTkArdUART::findUntil(CONSTVAR char *target, CONSTVAR char *terminator)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->findUntil(target, terminator);
}

bool sfTkArdUART::findUntil(CONSTVAR uint8_t *target, CONSTVAR char *terminator)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->findUntil(target, terminator);
}

bool sfTkArdUART::findUntil(CONSTVAR char *target, size_t targetLen, CONSTVAR char *terminate, size_t termLen)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->findUntil(target, targetLen, terminate, termLen);
}

bool sfTkArdUART::findUntil(CONSTVAR uint8_t *target, size_t targetLen, CONSTVAR char *terminate, size_t termLen)
{
    if (!_hwSerial)
        return false;

    return _hwSerial->findUntil(target, targetLen, terminate, termLen);
}

#ifdef ARDUINO_ARCH_ESP8266
long sfTkArdUART::parseInt()
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->parseInt();
}

float sfTkArdUART::parseFloat()
{
    if (!_hwSerial)
        return 0.0f;

    return _hwSerial->parseFloat();
}
#else
long sfTkArdUART::parseInt(LookaheadMode lookahead, char ignore)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->parseInt(lookahead, ignore);
}

float sfTkArdUART::parseFloat(LookaheadMode lookahead, char ignore)
{
    if (!_hwSerial)
        return 0.0f;

    return _hwSerial->parseFloat(lookahead, ignore);
}
#endif

size_t sfTkArdUART::readBytes(char *buffer, size_t length)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->readBytes(buffer, length);
}

size_t sfTkArdUART::readBytes(uint8_t *buffer, size_t length)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->readBytes(buffer, length);
}

size_t sfTkArdUART::readBytesUntil(char terminator, char *buffer, size_t length)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->readBytesUntil(terminator, buffer, length);
}

size_t sfTkArdUART::readBytesUntil(char terminator, uint8_t *buffer, size_t length)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->readBytesUntil(terminator, buffer, length);
}

String sfTkArdUART::readString()
{
    if (!_hwSerial)
        return String("");

    return _hwSerial->readString();
}

String sfTkArdUART::readStringUntil(char terminator)
{
    if (!_hwSerial)
        return String("");

    return _hwSerial->readStringUntil(terminator);
}

/**
 * @brief Print mappings
 *
 */
size_t sfTkArdUART::print(const __FlashStringHelper *ifsh)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(ifsh);
}

size_t sfTkArdUART::print(const String &s)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(s);
}

size_t sfTkArdUART::print(const char str[])
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(str);
}

size_t sfTkArdUART::print(char c)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(c);
}

size_t sfTkArdUART::print(unsigned char b, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(b, base);
}

size_t sfTkArdUART::print(int n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(unsigned int n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(unsigned long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(long long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(unsigned long long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, base);
}

size_t sfTkArdUART::print(double n, int digits)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(n, digits);
}

size_t sfTkArdUART::print(const Printable &x)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->print(x);
}

size_t sfTkArdUART::println(const __FlashStringHelper *ifsh)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(ifsh);
}

size_t sfTkArdUART::println(const String &s)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(s);
}

size_t sfTkArdUART::println(const char c[])
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(c);
}

size_t sfTkArdUART::println(char c)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(c);
}

size_t sfTkArdUART::println(unsigned char b, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(b, base);
}

size_t sfTkArdUART::println(int n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(unsigned int n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(unsigned long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(long long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(unsigned long long n, int base)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, base);
}

size_t sfTkArdUART::println(double n, int digits)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(n, digits);
}

size_t sfTkArdUART::println(const Printable &x)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println(x);
}

size_t sfTkArdUART::println(void)
{
    if (!_hwSerial)
        return 0;

    return _hwSerial->println();
}
#endif
