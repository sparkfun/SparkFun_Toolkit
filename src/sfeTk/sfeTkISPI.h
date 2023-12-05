
// sfeTkISPI.h
//
// Defines the SPI communication bus interface for the SparkFun Electronics Toolkit
/*

The MIT License (MIT)

Copyright (c) 2022 SparkFun Electronics
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions: The
above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
"AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "sfeTkIBus.h"

class sfeTkSPI : public sfeTkIBus
{
  public:
    sfeTkI2C() : _csPin{kNoCSPin}
    {
    }

    /// @brief setter for the CS Pin
    ///
    /// @param devCS The device's CS Pin
    ///
    virtual void setCS(uint8_t devCS)
    {
        _cs = devCS;
    }

    /// @brief getter for the cs pin
    ///
    /// @retval uint8_t returns the CS pin for the device
    ///
    virtual uint8_t cs(void)
    {
        return _cs;
    }

    static constexpr uint8_t kNoCSPin = 0;

  private:
    uint8_t _cs;
};

//};
