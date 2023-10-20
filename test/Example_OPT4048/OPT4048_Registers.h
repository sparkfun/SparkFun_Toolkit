/* 
OPT4048_Registers.h

SparkFun Tristimulus Color Sensor - OPT4048

Qwiic 1x1
* https://www.sparkfun.com/products/
Qwiic Mini
* https://www.sparkfun.com/products/

Repository:
* https://github.com/sparkfun/SparkFun_OPT4048_Arduino_Library

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

License(http://opensource.org/licenses/MIT).

The following file defines the registers for the opt4048 IC.
*/

#pragma once
#include "Arduino.h"
#include <stdint.h>

#define OPT4048_ADDR_HIGH 0x45 
#define OPT4048_ADDR_LOW 0x44 // DEFAULT
#define OPT4048_ADDR_DEF 0x44 // DEFAULT
#define OPT4048_ADDR_SDA 0x46
#define OPT4048_ADDR_SCL 0x45

#define OPT4048_DEVICE_ID 0x2084

// Setttings - use find in your editor for to search for "settings"
// to scroll through all predefined setting types.
/// @brief Range Settings found in Register 0x0A
typedef enum
{
    RANGE_2KLUX2 = 0x00,
    RANGE_4KLUX5,
    RANGE_9LUX,
    RANGE_18LUX,
    RANGE_36LUX,
    RANGE_72LUX,
    RANGE_144LUX,
    RANGE_AUTO = 0x0C
} opt4048_range_t;

/// @brief Converstion Settings for Register 0x0A
typedef enum
{
    CONVERSION_TIME_600US = 0x00,
    CONVERSION_TIME_1MS,
    CONVERSION_TIME_1MS8,
    CONVERSION_TIME_3MS4,
    CONVERSION_TIME_6MS5,
    CONVERSION_TIME_12MS7,
    CONVERSION_TIME_25MS,
    CONVERSION_TIME_50MS,
    CONVERSION_TIME_100MS,
    CONVERSION_TIME_200MS,
    CONVERSION_TIME_400MS,
    CONVERSION_TIME_800MS

} opt4048_conversion_time_t;

/// @brief Operation mode settings found in Register 0x0A
typedef enum
{
    OPERATION_MODE_POWER_DOWN = 0x00,
    OPERATION_MODE_AUTO_ONE_SHOT,
    OPERATION_MODE_ONE_SHOT,
    OPERATION_MODE_CONTINUOUS

} opt4048_operation_mode_t;

/// @brief Fault count settings found in Register 0x0A
typedef enum
{
    FAULT_COUNT_1 = 0x00,
    FAULT_COUNT_2,
    FAULT_COUNT_3,
    FAULT_COUNT_8
} opt4048_fault_count_t;

/// @brief Threshold channel settings found in Register 0x0B
typedef enum
{
    THRESH_CHANNEL_CH0 = 0x00,
    THRESH_CHANNEL_CH1,
    THRESH_CHANNEL_CH2,
    THRESH_CHANNEL_CH3
} opt4048_threshold_channel_t;

/// @brief Interrupt settings found in Register 0x0B
typedef enum
{
    INT_SMBUS_ALERT = 0x00,
    INT_DR_NEXT_CHANNEL,
    INT_DR_ALL_CHANNELS = 0x03
} opt4048_int_cfg_t;

/// @brief OPT4048 Register for Exponent and Result (MSB) for Channel 0
#define SFE_OPT4048_REGISTER_EXP_RES_CH0 0x00
typedef union {
    struct
    {
        uint16_t result_msb_ch0 : 12;
        uint8_t exponent_ch0 : 4;
    };
    uint16_t word;

} opt4048_reg_exp_res_ch0_t;

/// @brief OPT4048 Register for Result (LSB), Counter, and CRC for Channel 0
#define SFE_OPT4048_REGISTER_RES_CNT_CRC_CH0 0x01
typedef union {
    struct
    {
        uint8_t crc_ch0 : 4;
        uint8_t counter_ch0 : 4;
        uint8_t result_lsb_ch0 : 8;
    };
    uint16_t word;

} opt4048_reg_res_cnt_crc_ch0_t;

/// @brief OPT4048 Register for Exponent and Result (MSB) for Channel 1
#define SFE_OPT4048_REGISTER_EXP_RES_CH1 0x02
typedef union {
    struct
    {
        uint16_t result_msb_ch1 : 12;
        uint8_t exponent_ch1 : 4;
    };
    uint16_t word;

} opt4048_reg_exp_res_ch1_t;

/// @brief OPT4048 Register for Result (LSB), Counter, and CRC for Channel 1
#define SFE_OPT4048_REGISTER_RES_CNT_CRC_CH1 0x03
typedef union {
    struct
    {
        uint8_t crc_ch1 : 4;
        uint8_t counter_ch1 : 4;
        uint8_t result_lsb_ch1 : 8;
    };
    uint16_t word;

} opt4048_reg_res_cnt_crc_ch1_t;

/// @brief  OPT4048 Register for Exponent and Result (MSB) for Channel 2
#define SFE_OPT4048_REGISTER_EXP_RES_CH2 0x04
typedef union {
    struct
    {
        uint16_t result_msb_ch2 : 12;
        uint8_t exponent_ch2 : 4;
    };
    uint16_t word;

} opt4048_reg_exp_res_ch2_t;

/// @brief OPT4048 Register for Result (LSB), Counter, and CRC for Channel 2
#define SFE_OPT4048_REGISTER_RES_CNT_CRC_CH2 0x05
typedef union {
    struct
    {
        uint8_t crc_ch2 : 4;
        uint8_t counter_ch2 : 4;
        uint8_t result_lsb_ch2 : 8;
    };
    uint16_t word;

} opt4048_reg_res_cnt_crc_ch2_t;

/// @brief OPT4048 Register for Exponent and Result (MSB) for Channel 3
#define SFE_OPT4048_REGISTER_EXP_RES_CH3 0x06
typedef union {
    struct
    {
        uint16_t result_msb_ch3 : 12;
        uint8_t exponent_ch3 : 4;
    };
    uint16_t word;

} opt4048_reg_exp_res_ch3_t;

/// @brief  OPT4048 Register for Result (LSB), Counter, and CRC for Channel 3
#define SFE_OPT4048_REGISTER_RES_CNT_CRC_CH3 0x07
typedef union {
    struct
    {
        uint8_t crc_ch3 : 4;
        uint8_t counter_ch3 : 4;
        uint8_t result_lsb_ch3 : 8;
    };
    uint16_t word;

} opt4048_reg_res_cnt_crc_ch3_t;

/// @brief OPT4048 Register for Threshold  Exponent and Result - Low
#define SFE_OPT4048_REGISTER_THRESH_L_EXP_RES 0x08
typedef union {
    struct
    {
        uint16_t thresh_result : 12;
        uint8_t thresh_exp : 4;
    };
    uint16_t word;

} opt4048_reg_thresh_exp_res_low_t;

/// @brief OPT4048 Register for Threshold Exponent and Threshold Result - High
#define SFE_OPT4048_REGISTER_THRESH_H_EXP_RES 0x09
typedef union {
    struct
    {
        uint16_t thresh_result : 12;
        uint8_t thresh_exp : 4;
    };
    uint16_t word;

} opt4048_reg_thresh_exp_res_high_t;

/// @brief OPT4048 Register that controls the main functions of the device.
#define SFE_OPT4048_REGISTER_CONTROL 0x0A


typedef union {
    struct
    {
        uint16_t fault_count : 2;
        uint16_t int_pol : 1;
        uint16_t latch : 1;
        uint16_t op_mode : 2;
        uint16_t conversion_time : 4;
        uint16_t range : 4;
        uint16_t reserved : 1;
        uint16_t qwake : 1;
    };
    uint16_t word;
} opt4048_reg_control_t;


/// @brief OPT4047 Register with settings for the interrupt pin.
#define SFE_OPT4048_REGISTER_INT_CONTROL 0x0B
typedef union {
    struct
    {
        uint16_t i2c_burst : 1;
        uint16_t reserved_two : 1;
        uint16_t int_cfg : 2;
        uint16_t int_dir : 1;
        uint16_t threshold_ch_sel : 2;
        uint16_t reserved_one : 9;
    };
    uint16_t word;

} opt4048_reg_int_control_t;


/// @brief OPT4048 register containing various status flags.
#define SFE_OPT4048_REGISTER_FLAGS 0x0C
typedef union {
    struct
    {
        uint8_t flag_low : 1;
        uint8_t flag_high : 1;
        uint8_t conv_ready_flag : 1;
        uint8_t overload_flag : 1;
        uint16_t reserved : 12;
    };
    uint16_t word;

} opt4048_reg_flags_t;

/// @brief  OPT4048 Register containing the device ID.
#define SFE_OPT4048_REGISTER_DEVICE_ID 0x11
typedef union {
    struct
    {
        uint16_t DIDH : 12;
        uint16_t DIDL : 2;
        uint16_t reserved : 2;
    };
    uint16_t word;

} opt4048_reg_device_id_t;

