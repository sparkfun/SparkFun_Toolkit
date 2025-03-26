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

#include <cstddef>
#include <memory>

#include <Arduino.h>
#include <api/HardwareSerial.h>
#include <api/Print.h>
#include <api/Stream.h>
#include <api/String.h>

// Include our platform UART interface definitions.
#include "sfTk/sfTkError.h"
#include "sfTk/sfTkISerial.h"
#include "sfTkArduino.h"
#include <sfTk/sfTkIUART.h>
#include <sfTk/sfTkISerialBus.h>

#define NO_IGNORE_CHAR  '\x01' // a char not found in a valid ASCII numeric field

/**
 * @brief The sfTkArdUART implements an sfTkIUART interface, defining the Arduino implementation for UART in the Toolkit
 */
class sfTkArdUART : public sfTkIUART
{
public:
    /**
     * @brief Constructor
     */
    sfTkArdUART(void) : sfTkIUART(), _hwSerial{nullptr}
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
    sfTkArdUART(arduino::HardwareSerial &hwSerial) : sfTkIUART(), _hwSerial{&hwSerial}
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
        if(this != &rhs)
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
    sfTkError_t init(void);

    /**
     * @brief - address version of the init method
     * 
     * @param baudRate The baud rate to set
     */
    sfTkError_t init(uint32_t baudRate, bool bInit = false);

    /**
     * @brief config version of the init method
     * 
     * @param config The UART configuration settings.
     */
    sfTkError_t init(UARTConfig_t config, bool bInit = false);

    /**
     * @brief Method sets up the required UART settings.
     * 
     * @param uartPort Port for UART communication.
     * @param baudRate The baud rate to set.
     * @param bInit This flag tracks whether the bus has been initialized.
     * 
     * @retval ksfTkErrOk on successful execution.
     */
    sfTkError_t init(arduino::HardwareSerial &hwSerial, uint32_t baudRate, bool bInit = false);

    /**
     * @brief Method sets up the required UART settings.
     *
     * @param uartPort Port for UART communication.
     * @param config The UART configuration settings.
     * @param bInit This flag tracks whether the bus has been initialized.
     * 
     * @retval ksftkErrOk on successful execution.
     */
    sfTkError_t init(arduino::HardwareSerial &hwSerial, UARTConfig_t config, bool bInit = false);
    
    /**
     * @brief Write `len` bytes to the UART TX buffer.
     * 
     * @param data Data buffer to write.
     * @param len Number of bytes to write.
     * @return sfTkError_t - Returns ksfTkErrOk on success, or ksfTkErrFail code.
     */
    sfTkError_t write(const uint8_t *data, size_t len) override;

    /**
     * @brief Write one byte to the UART TX buffer.
     * 
     * @param data Byte to write.
     * @return sfTkError_t - Returns ksfTkErrOk on success, or ksfTkErrFail code.
     */
    sfTkError_t write(const uint8_t data) override;

    /**
     * @brief Reads an array of bytes from the serial interface
     * 
     * @param data The data buffer to read into
     * @param length The length of the data buffer
     * @param bytesRead[out] The number of bytes read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t read(uint8_t *data, size_t length, size_t &bytesRead) override;

    /**
     * @brief Reads a single byte from the serial interface
     * 
     * @param data[out] Byte to be read
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t read(uint8_t &data) override;

    /**
     * @brief Checks if UART is running.
     * 
     * @return true - UART is running.
     * @return false - UART is not running.
     */
    operator bool();

    /**
     * @brief setter for UART baud rate
     * 
     * @param baudRate The baud rate to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setBaudRate(const uint32_t baudRate) override;

    /**
     * @brief setter for the stop bits
     * 
     * @param stopBits The stop bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setStopBits(const sfTkUARTStopBits_t stopBits) override;

    /**
     * @brief setter for the parity
     * 
     * @param parity The parity to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setParity(const sfTkUARTParity_t parity) override;

    /**
     * @brief setter for the data bits
     * 
     * @param dataBits The data bits to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setDataBits(const sfTkUARTDataBits_t dataBits) override;

    /**
     * @brief setter for the internal config object
     * 
     * @param config The config struct to set
     * @return sfTkError_t Returns ksfTkErrOk on success, or ksfTkErrFail code
     */
    sfTkError_t setConfig(const uint32_t baudRate = kDefaultBaudRate, 
                          const sfTkUARTDataBits_t dataBits = kDefaultDataBits, 
                          const sfTkUARTParity_t parity = kDefaultParity, 
                          const sfTkUARTStopBits_t stopBits = kDefaultStopBits) override;

    /**
     * @brief Arduino HardwareSerial functionality mappings.
     * 
     */
    void end(void);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    void flush(void);

    /**
     * @brief Arduino Stream functionality mappings.
     * 
     */
    void setTimeout(unsigned long timeout);
    unsigned long getTimeout();
    
    bool find(const char *target);   
    bool find(const uint8_t *target);

    bool find(const char *target, size_t length);
    bool find(const uint8_t *target, size_t length);

    bool find(char target);

    bool findUntil(const char *target, const char *terminator);
    bool findUntil(const uint8_t *target, const char *terminator);

    bool findUntil(const char *target, size_t targetLen, const char *terminate, size_t termLen);
    bool findUntil(const uint8_t *target, size_t targetLen, const char *terminate, size_t termLen);

    long parseInt(arduino::LookaheadMode lookahead = arduino::LookaheadMode::SKIP_ALL, char ignore = NO_IGNORE_CHAR);
    float parseFloat(arduino::LookaheadMode lookahead = arduino::LookaheadMode::SKIP_ALL, char ignore = NO_IGNORE_CHAR);

    size_t readBytes( char *buffer, size_t length);
    size_t readBytes( uint8_t *buffer, size_t length);

    size_t readBytesUntil( char terminator, char *buffer, size_t length);
    size_t readBytesUntil( char terminator, uint8_t *buffer, size_t length);

    arduino::String readString();
    arduino::String readStringUntil(char terminator);

    /**
     * @brief Arduino Print functionality mappings.
     * 
     */
    size_t print(const arduino::__FlashStringHelper *);
    size_t print(const arduino::String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(long long, int = DEC);
    size_t print(unsigned long long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const arduino::Printable&);

    size_t println(const arduino::__FlashStringHelper *);
    size_t println(const arduino::String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(long long, int = DEC);
    size_t println(unsigned long long, int = DEC);
    size_t println(double, int = 2);
    size_t println(const arduino::Printable&);
    size_t println(void);

    size_t printf(const char *format, ...);
    size_t printf_P(const char *format, ...);

protected:
    /** The actual Arduino hardware port */
    arduino::HardwareSerial *_hwSerial;

private:
    bool _running = false; // Flag to track if the bus is running
    sfTkError_t _start(void); // Start the connection to the UART port
    
};

/**
 * @brief The sfTkArdUARTBus class implements an sfTkIUARTBus interface, connecting Arduino UART to the sfTkIBus interface.
 * 
 */
class sfTkArdUARTBus : public sfTkISerialBus
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
    sfTkArdUARTBus(sfTkArdUART &uartPort) : sfTkISerialBus(), _uartPort{std::make_unique<sfTkArdUART>(uartPort)}
    {
    }

    /**
     * @brief Construct a new sfTkArdUARTBus object
     * 
     * @param hwSerial Pass in an underlying hardware serial port
     */
    sfTkArdUARTBus(arduino::HardwareSerial &hwSerial) : sfTkISerialBus(), _uartPort{std::make_unique<sfTkArdUART>(hwSerial)}
    {
    }

    /**
     * @brief Copy constructer 
     * 
     * @param rhs Bus object to be copied
     */
    sfTkArdUARTBus(sfTkArdUARTBus const &rhs) : sfTkISerialBus(), _uartPort{std::make_unique<sfTkArdUART>(*rhs._uartPort)}
    {
    }

    /**
     * @brief Copy assignment
     * 
     * @param rhs right hand side of the assignment
     * @return value of the left hand side of the assignment
     */
    sfTkArdUARTBus &operator=(const sfTkArdUARTBus &rhs)
    {
        if(this != &rhs)
        {
            if(rhs._uartPort)
            {
                _uartPort = std::make_unique<sfTkArdUART>(*rhs._uartPort);
            }
            else
            {
                _uartPort.reset();
            }
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
        if(!_uartPort)
            _uartPort = std::make_unique<sfTkArdUART>();
        return _uartPort->init();
    }

    /**
     * @brief - address version of the init method
     * 
     * @param baudRate The baud rate to set
     */
    sfTkError_t init(uint32_t baudRate, bool bInit = false)
    {
        if(!_uartPort)
            _uartPort = std::make_unique<sfTkArdUART>();
        return _uartPort->init(baudRate, bInit);
    }

    /**
     * @brief - config version of the init method
     * 
     * @param config The configuration to set
     */
    sfTkError_t init(sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        if(!_uartPort)
            _uartPort = std::make_unique<sfTkArdUART>();
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
    sfTkError_t init(sfTkArdUART &uartPort, uint32_t baudRate, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(uartPort);
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
    sfTkError_t init(sfTkArdUART &uartPort, sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(uartPort);
        return _uartPort->init(config, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided UART port.
     * 
     * @param uartPort Port for communication
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(sfTkArdUART &uartPort, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(uartPort);
        return _uartPort->init(sfTkIUART::kDefaultBaudRate, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     * 
     * @param hwSerial The hardware serial port to use
     * @param baudRate The baud rate to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(arduino::HardwareSerial &hwSerial, uint32_t baudRate, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(hwSerial);
        return _uartPort->init(baudRate, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     * 
     * @param hwSerial The hardware serial port to use
     * @param config The configuration to set
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(arduino::HardwareSerial &hwSerial, sfTkIUART::UARTConfig_t config, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(hwSerial);
        return _uartPort->init(config, bInit);
    }

    /**
     * @brief Method sets up the required UART settings using the provided HardwareSerial port.
     * 
     * @param hwSerial The hardware serial port to use
     * @param bInit This flag tracks whether the bus has been initialized.
     * @return sfTkError_t ksfTkErrOk on successful execution.
     */
    sfTkError_t init(arduino::HardwareSerial &hwSerial, bool bInit = false)
    {
        _uartPort = std::make_unique<sfTkArdUART>(hwSerial);
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
        if(!_uartPort)
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
        if(!_uartPort)
            return ksfTkErrBusNotInit;
        return _uartPort->read(data, length, readBytes);
    }

protected:
    /** The actual UART port */
    std::unique_ptr<sfTkArdUART> _uartPort;
};
