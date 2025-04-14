/**
 * @file sfTkArdSerial.cpp
 * @brief Implementation file for the SparkFun Toolkit Arduino Serial driver.
 *
 * This file contains the Arduino Serial driver for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */
#include <Arduino.h>

// clang-format off
#include "sfTkArdSerial.h"
#include "sfTk/sfTkError.h"
#include "sfTk/sfTkISerial.h"
// clang-format on

sfTkError_t sfTkArdSerial::init(Stream &hwStream)
{
    _hwStream = &hwStream;

    return init();
}

sfTkError_t sfTkArdSerial::init()
{
    if (!_hwStream)
        return ksfTkErrSerialNotInit;

    if (!_hwStream->availableForWrite())
        return ksfTkErrSerialNoResponse;

    return ksfTkErrOk;
}

sfTkError_t sfTkArdSerial::write(const uint8_t *data, size_t length)
{
    if (!_hwStream)
        return ksfTkErrSerialNotInit;

    return (_hwStream->write(data, length) == length ? ksfTkErrOk : ksfTkErrSerialUnderRead);
}

sfTkError_t sfTkArdSerial::read(uint8_t *data, size_t length, size_t &bytesRead)
{
    if (!_hwStream)
        return ksfTkErrSerialNotInit;

    if (!data)
        return ksfTkErrSerialNullBuffer;

    if (length == 0)
        return ksfTkErrSerialDataTooLong; // nothing to read

    bytesRead = 0; // zero out value

    bytesRead = _hwStream->readBytes(data, length);

    if (bytesRead == 0)
        return ksfTkErrFail;

    // return success if all bytes read
    return (bytesRead == length) ? ksfTkErrOk : ksfTkErrSerialUnderRead;
}

int sfTkArdSerial::available()
{
    if (!_hwStream)
        return 0;
    return _hwStream->available();
}

int sfTkArdSerial::availableForWrite()
{
    if (!_hwStream)
        return 0;
    return _hwStream->availableForWrite();
}

int sfTkArdSerial::peek()
{
    if (!_hwStream)
        return -1;
    return _hwStream->peek();
}

void sfTkArdSerial::flush()
{
    if (!_hwStream)
        return;
    _hwStream->flush();
}
