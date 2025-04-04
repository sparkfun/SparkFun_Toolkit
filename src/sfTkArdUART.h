/**
 * @file sfTkArdUART.h
 * @brief header file for the SparkFun Toolkit Arduino UART driver.
 *
 * This file contains the Arduino UART header for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Arduino.h>
#include <Print.h>
#include <Stream.h>

// clang-format off
// Include our platform UART interface definitions.
#include "sfTk/sfTkError.h"
#include "sfTk/sfTkISerial.h"
#include "sfTkArduino.h"
#include <sfTk/sfTkIUART.h>
#include <sfTk/sfTkISerialBus.h>
// clang-format on

#define NO_IGNORE_CHAR '\x01' // a char not found in a valid ASCII numeric field

#ifdef ARDUINO_ARCH_AVR
#define CONSTVAR
#else
#define CONSTVAR const
#endif

/**
 * @brief The sfTkArdUART implements an sfTkIUART interface, defining the Arduino implementation for UART in the Toolkit
 */
template <class SerialType = HardwareSerial> class sfTkArdUART : public sfTkIUART
{
  protected:
    /** The actual Arduino hardware port */
    Stream *_hwStream;
    SerialType *_hwSerial;

  private:
    bool _running = false; // Flag to track if the bus is running

    sfTkError_t _start() // Start the connection to the UART port
    {
        if (!_hwSerial)
            return ksfTkErrSerialNotInit;
        if (_running)
            end(); // close the port if already running
                   // set the config
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
        // ESP32 does not support setting stop bits, parity, and data bits in a stanard manner.
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

  public:
    /**
     * @brief Constructor
     */
    sfTkArdUART(void) : sfTkIUART(), _hwSerial{nullptr}, _hwStream{nullptr}
    {
    }

    /**
     * @brief Constructor
     *
     * @param baudRate The baud rate to set
     */
    sfTkArdUART(uint32_t baudRate) : sfTkIUART(baudRate), _hwSerial{nullptr}
    {
    }

    /**
     * @brief Constructor
     *
     * @param config The UART configuration settings.
     */
    sfTkArdUART(UARTConfig_t config) : sfTkIUART(config), _hwSerial{nullptr}
    {
    }

    /**
     * @brief Constructor
     *
     * @param uartPort Port for UART communication.
     */
    sfTkArdUART(SerialType &hwSerial) : sfTkIUART(), _hwSerial{&hwSerial}
    {
    }

    /**
     * @brief Copy Constructor
     */
    sfTkArdUART(sfTkArdUART const &rhs) : sfTkIUART(rhs._config), _hwSerial{rhs._hwSerial}
    {
    }

    /**
     * @brief Copy assignment
     *
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfTkArdUART &operator=(const sfTkArdUART &rhs)
    {
        if (this != &rhs)
        {
            sfTkIUART::operator=(rhs);
            _hwSerial = rhs._hwSerial;
        }
        return *this;
    }

    /**
     * @brief Method sets up the required UART settings.
     * @note This function provides a default UART Port.
     *
     * @retval ksfTkErrOk on successful execution.
     */
    sfTkError_t init(void)
    {
        return init(kDefaultBaudRate, true);
    }

    /**
     * @brief - address version of the init method
     *
     * @param baudRate The baud rate to set
     */
    sfTkError_t init(uint32_t baudRate, bool bInit = false)
    {
        // #ifdef _THIS__NOT_IS_BROKEN
        // if we don't have a port already, use the default Arduino Serial.
        if (!_hwSerial)
            return init(Serial, baudRate, bInit);

        // We already have a UART setup, so it's already initialized. Change the baud rate.
        return setBaudRate(baudRate); // set the baud rate
        // #else
        //     return ksfTkErrFail;
        // #endif
    }

    /**
     * @brief config version of the init method
     *
     * @param config The UART configuration settings.
     */
    sfTkError_t init(UARTConfig_t config, bool bInit = false)
    {
        // #ifdef _THIS__NOT_IS_BROKEN
        // if we don't have a port already, use the default Arduino Serial.
        if (!_hwSerial)
            return init(Serial, config, bInit);

        if (bInit)
            return _start(); // start the port

        // We already have a UART setup, so it's already initialized.
        return ksfTkErrOk;
        // #else
        //     return ksfTkErrFail;
        // #endif
    }

    /**
     * @brief Method sets up the required UART settings.
     *
     * @param uartPort Port for UART communication.
     * @param baudRate The baud rate to set.
     * @param bInit This flag tracks whether the bus has been initialized.
     *
     * @retval ksfTkErrOk on successful execution.
     */
    sfTkError_t init(Stream &hwStream, uint32_t baudRate, bool bInit = false)
    {
        _hwStream = &hwStream; // set the serial port
        _hwSerial = static_cast<SerialType *>(_hwStream);

        _config.baudRate = baudRate; // set the baud rate

        if (bInit)
            return _start(); // start the port

        return ksfTkErrOk;
    }

    /**
     * @brief Method sets up the required UART settings.
     *
     * @param uartPort Port for UART communication.
     * @param config The UART configuration settings.
     * @param bInit This flag tracks whether the bus has been initialized.
     *
     * @retval ksftkErrOk on successful execution.
     */
    sfTkError_t init(Stream &hwStream, sfTkIUART::UARTConfig_t &config, bool bInit = false)
    {
        _hwStream = &hwStream; // set the serial port
        _hwSerial = static_cast<SerialType *>(_hwStream);

        _config = config; // set the config

        if (bInit)
            return _start(); // start the port

        return ksfTkErrOk;
    }

    /**
     * @brief Write `len` bytes to the UART TX buffer.
     *
     * @param data Data buffer to write.
     * @param len Number of bytes to write.
     * @return sfTkError_t - Returns ksfTkErrOk on success, or ksfTkErrFail code.
     */
    sfTkError_t write(const uint8_t *data, size_t len) override
    {
        if (!_hwSerial)
            return ksfTkErrSerialNotInit;

        return (_hwSerial->write(data, len) == len ? ksfTkErrOk : ksfTkErrSerialUnderRead);
    }

    /**
     * @brief Write one byte to the UART TX buffer.
     *
     * @param data Byte to write.
     * @return sfTkError_t - Returns ksfTkErrOk on success, or ksfTkErrFail code.
     */
    sfTkError_t write(const uint8_t data) override
    {
        if (!_hwSerial)
            return ksfTkErrSerialNotInit;

        return (_hwSerial->write(data) ? ksfTkErrOk : ksfTkErrFail);
    }

    /**
     * @brief Reads an array of bytes from the serial interface
     *
     * @param data The data buffer to read into
     * @param length The length of the data buffer
     * @param bytesRead[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t read(uint8_t *data, size_t length, size_t &bytesRead) override
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

    /**
     * @brief Reads a single byte from the serial interface
     *
     * @param data[out] Byte to be read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t read(uint8_t &data) override
    {
        size_t nRead;
        return read(&data, 1, nRead);
    }

    /**
     * @brief Checks if UART is running.
     *
     * @return true - UART is running.
     * @return false - UART is not running.
     */
    operator bool()
    {
        return (bool)*_hwSerial;
    }

    /**
     * @brief setter for UART baud rate
     *
     * @param baudRate The baud rate to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setBaudRate(const uint32_t baudRate) override
    {
        if (_config.baudRate != baudRate)
            _config.baudRate = baudRate; // set the baud rate

        return _start(); // start the port again
    }

    /**
     * @brief setter for the stop bits
     *
     * @param stopBits The stop bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setStopBits(const sfTkUARTStopBits_t stopBits) override
    {
        if (_config.stopBits != stopBits)
            _config.stopBits = stopBits; // set the stop bits

        return _start(); // start the port again
    }

    /**
     * @brief setter for the parity
     *
     * @param parity The parity to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setParity(const sfTkUARTParity_t parity) override
    {
        if (_config.parity != parity)
            _config.parity = parity; // set the baud rate

        return _start(); // start the port again
    }

    /**
     * @brief setter for the data bits
     *
     * @param dataBits The data bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setDataBits(const sfTkUARTDataBits_t dataBits) override
    {
        if (_config.dataBits != dataBits)
            _config.dataBits = dataBits; // set the baud rate

        return _start(); // start the port again
    }

    /**
     * @brief setter for the internal config object
     *
     * @param config The config struct to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setConfig(const uint32_t baudRate = kDefaultBaudRate,
                          const sfTkUARTDataBits_t dataBits = kDefaultDataBits,
                          const sfTkUARTParity_t parity = kDefaultParity,
                          const sfTkUARTStopBits_t stopBits = kDefaultStopBits) override
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

    /**
     * @brief Arduino HardwareSerial functionality mappings.
     *
     */
    void end()
    {
        _running = false; // set the running flag to false
        _hwSerial->end();
    }

    int available()
    {
        if (!_hwSerial)
            return 0;
        return _hwSerial->available();
    }

    int availableForWrite()
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->availableForWrite();
    }

    int peek()
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->peek();
    }

    void flush()
    {
        if (!_hwSerial)
            return;

        _hwSerial->flush();
    }

    /**
     * @brief Arduino Stream functionality mappings.
     *
     */
    void setTimeout(unsigned long timeout)
    {
        if (!_hwSerial)
            return;

        _hwSerial->setTimeout(timeout);
    }

    unsigned long getTimeout()
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->getTimeout();
    }

#if !defined(ARDUINO_ARCH_AVR) && !defined(ARDUINO_ARCH_ESP8266)
    bool find(CONSTVAR char *target)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->find(target);
    }

    bool find(CONSTVAR uint8_t *target)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->find(target);
    }

    bool find(CONSTVAR char *target, size_t length)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->find(target, length);
    }

    bool find(CONSTVAR uint8_t *target, size_t length)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->find(target, length);
    }

    bool find(char target)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->find(target);
    }

    bool findUntil(CONSTVAR char *target, CONSTVAR char *terminator)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->findUntil(target, terminator);
    }

    bool findUntil(CONSTVAR uint8_t *target, CONSTVAR char *terminator)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->findUntil(target, terminator);
    }

    bool findUntil(CONSTVAR char *target, size_t targetLen, CONSTVAR char *terminate, size_t termLen)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->findUntil(target, targetLen, terminate, termLen);
    }

    bool findUntil(CONSTVAR uint8_t *target, size_t targetLen, CONSTVAR char *terminate, size_t termLen)
    {
        if (!_hwSerial)
            return false;

        return _hwSerial->findUntil(target, targetLen, terminate, termLen);
    }

#ifdef ARDUINO_ARCH_ESP8266
    long parseInt()
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->parseInt();
    }

    float parseFloat()
    {
        if (!_hwSerial)
            return 0.0f;

        return _hwSerial->parseFloat();
    }
#else
    long parseInt(LookaheadMode lookahead = LookaheadMode::SKIP_ALL, char ignore = NO_IGNORE_CHAR)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->parseInt(lookahead, ignore);
    }

    float parseFloat(LookaheadMode lookahead = LookaheadMode::SKIP_ALL, char ignore = NO_IGNORE_CHAR)
    {
        if (!_hwSerial)
            return 0.0f;

        return _hwSerial->parseFloat(lookahead, ignore);
    }
#endif

    size_t readBytes(char *buffer, size_t length)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->readBytes(buffer, length);
    }

    size_t readBytes(uint8_t *buffer, size_t length)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->readBytes(buffer, length);
    }

    size_t readBytesUntil(char terminator, char *buffer, size_t length)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->readBytesUntil(terminator, buffer, length);
    }

    size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->readBytesUntil(terminator, buffer, length);
    }

    String readString()
    {
        if (!_hwSerial)
            return String("");

        return _hwSerial->readString();
    }

    String readStringUntil(char terminator)
    {
        if (!_hwSerial)
            return String("");

        return _hwSerial->readStringUntil(terminator);
    }

    /**
     * @brief Arduino Print functionality mappings.
     *
     */
    size_t print(const __FlashStringHelper *ifsh)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(ifsh);
    }

    size_t print(const String &s)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(s);
    }

    size_t print(const char str[])
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(str);
    }

    size_t print(char c)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(c);
    }

    size_t print(unsigned char b, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(b, base);
    }

    size_t print(int n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(unsigned int n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(unsigned long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(long long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(unsigned long long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, base);
    }

    size_t print(double n, int digits = 2)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(n, digits);
    }

    size_t print(const Printable &x)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->print(x);
    }

    size_t println(const __FlashStringHelper *ifsh)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(ifsh);
    }

    size_t println(const String &s)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(s);
    }

    size_t println(const char c[])
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(c);
    }

    size_t println(char c)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(c);
    }

    size_t println(unsigned char b, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(b, base);
    }

    size_t println(int n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(unsigned int n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(unsigned long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(long long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(unsigned long long n, int base = DEC)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, base);
    }

    size_t println(double n, int digits = 2)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(n, digits);
    }

    size_t println(const Printable &x)
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println(x);
    }

    size_t println()
    {
        if (!_hwSerial)
            return 0;

        return _hwSerial->println();
    }
#endif
};

/**
 * @brief The sfTkArdUARTBus class implements an sfTkIUARTBus interface, connecting Arduino UART to the sfTkIBus
 * interface.
 *
 */
template <class SerialType = HardwareSerial> class sfTkArdUARTBus : public sfTkISerialBus
{
  public:
    /**
     * @brief Constructor for the UART bus
     *
     */
    sfTkArdUARTBus(void) : sfTkISerialBus(), _uartPort{nullptr}
    {
    }

    /**
     * @brief Construct a new sfTkArdUARTBus object
     *
     * @param uartPort UART port to use
     */
    sfTkArdUARTBus(sfTkArdUART<SerialType> &uartPort) : sfTkISerialBus(), _uartPort{&uartPort}
    {
    }

    /**
     * @brief Construct a new sfTkArdUARTBus object
     *
     * @param hwStream Pass in an underlying hardware serial port
     */
    sfTkArdUARTBus(Stream &hwStream) : sfTkISerialBus()
    {
        _uartPort = new sfTkArdUART<SerialType>(hwStream);
    }

    /**
     * @brief Copy constructor
     *
     * @param rhs Bus object to be copied
     */
    sfTkArdUARTBus(sfTkArdUARTBus const &rhs) : sfTkISerialBus()
    {
        _uartPort = rhs._uartPort ? new sfTkArdUART<SerialType>(*rhs._uartPort) : nullptr;
    }

    /**
     * @brief Destructor
     */
    ~sfTkArdUARTBus()
    {
        delete _uartPort;
    }

    /**
     * @brief Copy assignment
     *
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfTkArdUARTBus &operator=(const sfTkArdUARTBus &rhs)
    {
        if (this != &rhs)
        {
            delete _uartPort;
            _uartPort = rhs._uartPort ? new sfTkArdUART<SerialType>(*rhs._uartPort) : nullptr;
        }
        return *this;
    }

    /**
     * @brief Method sets up the required UART settings.
     * @note This function provides a default UART Port.
     *
     * @retval ksfTkErrOk on successful execution.
     */
    sfTkError_t init(void)
    {
        if (!_uartPort)
            _uartPort = new sfTkArdUART<SerialType>();
        return _uartPort->init();
    }

    /**
     * @brief - address version of the init method
     *
     * @param baudRate The baud rate to set
     */
    sfTkError_t init(uint32_t baudRate, bool bInit = false)
    {
        if (!_uartPort)
            _uartPort = new sfTkArdUART<SerialType>();
        return _uartPort->init(baudRate, bInit);
    }

    /**
     * @brief - config version of the init method
     *
     * @param config The configuration to set
     */
    sfTkError_t init(sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        if (!_uartPort)
            _uartPort = new sfTkArdUART<SerialType>();
        return _uartPort->init(config, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided UART port.
     *
     * @param uartPort Port for communication
     * @param baudRate The baud rate to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(sfTkArdUART<SerialType> &uartPort, uint32_t baudRate, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(uartPort);
        return _uartPort->init(baudRate, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided UART port.
     *
     * @param uartPort Port for communication
     * @param config The configuration to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(sfTkArdUART<SerialType> &uartPort, sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(uartPort);
        return _uartPort->init(config, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided UART port.
     *
     * @param uartPort Port for communication
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(sfTkArdUART<SerialType> &uartPort, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(uartPort);
        return _uartPort->init(sfTkIUART::kDefaultBaudRate, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     *
     * @param hwStream The hardware serial port to use
     * @param baudRate The baud rate to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(Stream &hwStream, uint32_t baudRate, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(hwStream);
        return _uartPort->init(baudRate, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     *
     * @param hwStream The hardware serial port to use
     * @param config The configuration to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(Stream &hwStream, sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(hwStream);
        return _uartPort->init(config, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     *
     * @param hwStream The hardware serial port to use
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(Stream &hwStream, bool bInit = false)
    {
        delete _uartPort; // Delete existing reference to port.
        _uartPort = new sfTkArdUART<SerialType>(hwStream);
        return _uartPort->init(sfTkIUART::kDefaultBaudRate, bInit);
    }

    /**
     * @brief Write `len` bytes to the UART TX buffer.
     *
     * @param data Data buffer to write.
     * @param len Number of bytes to write.
     * @return sfTkError_t - Returns ksfTkErrOk on success, or ksfTkErrFail code.
     */
    sfTkError_t write(const uint8_t *data, size_t length) override
    {
        if (!_uartPort)
            return ksfTkErrBusNotInit;

        sfTkError_t retVal = _uartPort->write(data, length);

        return retVal;
    }

    /**
     * @brief Reads an array of bytes from the serial interface
     *
     * @param data The data buffer to read into
     * @param length The length of the data buffer
     * @param bytesRead[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t read(uint8_t *data, size_t length, size_t &readBytes) override
    {
        if (!_uartPort)
            return ksfTkErrBusNotInit;
        return _uartPort->read(data, length, readBytes);
    }

  protected:
    /** The actual UART port */
    sfTkArdUART<SerialType> *_uartPort;
};
