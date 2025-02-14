
/**
 * @file sfToolkit.h
 * @brief Header file for the SparkFun Toolkit.
 *
 * This file contains the declarations of various utility functions
 * and types for the SparkFun Toolkit library.
 *
 * @author SparkFun Electronics
 * @date 2024-2025
 * @copyright Copyright (c) 2024-2025, SparkFun Electronics Inc. This project is released under the MIT License.
 * 
 * SPDX-License-Identifier: MIT
 */


#pragma once

#include <stdint.h>

/**
    @brief Common include file for the core of the SparkFun Electronics Toolkit
*/
#include "sfTkError.h"


// byte order types/enum
enum class sfTkByteOrder : uint8_t
{
    BigEndian = 0x01,
    LittleEndian = 0x02
};

// Note - toolkit *functions* start with sftk_ to avoid name collisions

// Function to determine the byte order of the system
sfTkByteOrder sftk_system_byteorder(void);

uint8_t sftk_byte_swap(uint8_t i);
uint16_t sftk_byte_swap(uint16_t i);
uint32_t sftk_byte_swap(uint32_t i);


// Area for platform specific implementations. The interface/functions are 
// defined here, with the expectation that the platform provides the implementation.

// delay in milliseconds
void sftk_delay_ms(uint32_t ms);

// ticks in milliseconds
uint32_t sftk_ticks_ms(void);