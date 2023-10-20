#pragma once

// #include <stdint.h> // REMOVE ME LATER, just for squiggle removal.
// #include "../../src/SparkFun_Toolkit.h"
#include <SparkFun_Toolkit.h>
#include "OPT4048_Registers.h"
#include <math.h>

/// @brief Struct used to store the color data from the OPT4048.
typedef struct
{
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t white;
    uint8_t counterR; // Sample counter
    uint8_t counterG;
    uint8_t counterB;
    uint8_t counterW;
    uint8_t CRCR;
    uint8_t CRCG;
    uint8_t CRCB;
    uint8_t CRCW;

} sfe_color_t;

/// @brief  Union used to re-calculate the CRC for optional double check.
typedef union {
    struct
    {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
    };
    uint8_t byte;
} crcBits;

/// @brief  Union used to re-calculate the CRC for optional double check.
typedef union {
    struct
    {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
    };
    uint8_t byte;
} exponBits;

/// @brief  Union used to re-calculate the CRC for optional double check.
typedef union {
    struct
    {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
        uint8_t bit8 : 1;
        uint8_t bit9 : 1;
        uint8_t bit10 : 1;
        uint8_t bit11 : 1;
        uint8_t bit12 : 1;
        uint8_t bit13 : 1;
        uint8_t bit14 : 1;
        uint8_t bit15 : 1;
        uint8_t bit16 : 1;
        uint8_t bit17 : 1;
        uint8_t bit18 : 1;
        uint8_t bit19 : 1;
    };
    uint32_t word;
} mantissaBits;

#define OPT_MATRIX_ROWS 4
#define OPT_MATRIX_COLS 4

template<typename sfeBusDevice, typename sfeBusDeviceSettings>
class OPT4048SFEBusBase
{
    public:
        OPT4048SFEBusBase() : _sfeBus{nullptr}, _devSettings{nullptr} {};

        bool begin(sfeBusDevice *theBus, sfeBusDeviceSettings *devSettings, uint8_t deviceAddress = OPT4048_ADDR_DEF)
        {
            setCommunicationBus(theBus, devSettings);
            return begin(deviceAddress);
        }
        
        bool begin(sfeBusDevice *theBus, uint8_t deviceAddress = OPT4048_ADDR_DEF)
        {
            setCommunicationBus(theBus);
            return begin(deviceAddress);
        }

        bool begin(uint8_t deviceAddress = OPT4048_ADDR_DEF)
        {
            if(!_sfeBus)
                setCommunicationBus();

            setDeviceAddress(deviceAddress);

            _sfeBus->begin();
            
            return init();
        }

        /// @brief Pings the device to see if it's there.
        /// @return true on successful execution.
        bool init(void)
        {
            if(SFE_BUS_OK != _sfeBus->ping(_devSettings))
                return false;
            return isConnected();
        }

        /// @brief Checks that the bus is connected with the OPT4048 by checking
        /// it's unique ID.
        /// @return True on successful execution.
        bool isConnected(void)
        {
            return (OPT4048_DEVICE_ID == getDeviceID());
        }

        /// @brief Retrieves the the device's ID: 0x24 for the OPT4048.
        /// @return Returns the unique ID.
        uint16_t getDeviceID(void)
        {
            uint8_t buff[2];
            uint16_t uniqueId;
            opt4048_reg_device_id_t idReg;

            int8_t result = readRegisterRegion(SFE_OPT4048_REGISTER_DEVICE_ID, buff);
            if(SFE_BUS_OK != result)
                return 0;

            idReg.word = (buff[0] << 8) | buff[1];

            uniqueId = (idReg.DIDH << 2) | idReg.DIDL;

            return uniqueId;
        }

        /// @brief Sets the pointer to the data bus for read and writes.
        /// @param theBus This parameter sets the bus.
        /// @param deviceSettings The bus device settings.
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setCommunicationBus(sfeBusDevice *theBus, sfeBusDeviceSettings *deviceSettings)
        {
            int8_t result = setCommunicationBus(theBus);
            if(SFE_BUS_OK != result)
                return result;

            result = setCommunicationDevSettings(deviceSettings);
            if(SFE_BUS_OK != result)
                return result;

            return SFE_BUS_OK;
        }

        /// @brief Sets the pointer to the data bus for read and writes.
        /// @param theBus This parameter sets the hardware bus.
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setCommunicationBus(sfeBusDevice *theBus = nullptr)
        {
            if(theBus == nullptr)
                theBus = new sfeBusDevice();

            if(!theBus)
                return SFE_BUS_E_NULL_PTR;

            _sfeBus = theBus;

            return SFE_BUS_OK;
        }

        /// @brief Sets the pointer to the data bus for read and writes.
        /// @param deviceSettings This parameter sets the bus' device settings.
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setCommunicationDevSettings(sfeBusDeviceSettings *deviceSettings = nullptr)
        {
            if(deviceSettings == nullptr)
                deviceSettings = new sfeBusDeviceSettings();

            if(!deviceSettings)
                return SFE_BUS_E_NULL_DEV_SETTINGS;

            _devSettings = deviceSettings;

            return SFE_BUS_OK;
        }

        /// @brief Sets the pointer to the data bus for read and writes.
        /// @param deviceAddress This parameter sets the device address.
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setDeviceAddress(const uint8_t deviceAddress)
        {
            int8_t result = SFE_BUS_OK;

            if(!_devSettings)
                result = setCommunicationDevSettings();

            if(SFE_BUS_OK != result)
                return result;

            _devSettings->devAddr = deviceAddress;

            return SFE_BUS_OK;
        }

        /// @brief Writes to the data to the given register using the hardware data bus.
        /// @param  offset The register to write to.
        /// @param  data The data to write to the register.
        /// @param  length The number of writes
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t writeRegisterRegion(const uint8_t offset, const uint8_t *data, const uint16_t length = 2)
        {
            return _sfeBus->writeRegisterBytes((SFEBusDevSettings*)_devSettings, offset, data, length);
        }

        /// @brief Reads data from the specified register using the set data bas.
        /// @param  offset The register to read from.
        /// @param  data The pointer to the value to store the value.
        /// @param  length The number of reads
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t readRegisterRegion(const uint8_t offset, uint8_t *data, const uint16_t numBytes = 2)
        {
            return _sfeBus->readRegisterBytes((SFEBusDevSettings*)_devSettings, offset, data, numBytes);
        }

        ///////////////////////////////////////////////////////////////////Device Settings

        /// @brief Sets the minimum of settings to get the board running.
        /// @return True on successful execution.
        bool setBasicSetup(void)
        {
            int8_t result = SFE_BUS_OK;

            Serial.println("Entered setup.");
            result = setRange(RANGE_36LUX);
            if(SFE_BUS_OK != result)
                return false;
            Serial.println("Set range.");

            result = setConversionTime(CONVERSION_TIME_200MS);
            if(SFE_BUS_OK != result)
                return false;
            Serial.println("Set conversion time.");

            result = setOperationMode(OPERATION_MODE_CONTINUOUS);
            if(SFE_BUS_OK != result)
                return false;

            Serial.println("Set operation mode.");

            return true;
        }

        /// @brief Sets the OPT4048's quick wake setting.
        /// @param enable The range to set the device to.
        /// @return True on successful execution.
        bool setQwake(const bool enable = true)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);
            if(SFE_BUS_OK != result)
                return false;

            controlReg.word = (buff[0] << 8) | buff[1];
            controlReg.qwake = (uint8_t)enable;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);
            if(SFE_BUS_OK != result)
                return false;

            return true;
        }
        
        /// @brief Retrieves the quick wake status of the OPT4048.
        /// @return status of qwake setting.
        bool getQwake(void)
        {
            uint8_t buff[2];
            int8_t result = SFE_BUS_OK;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);
            if(SFE_BUS_OK != result)
                return false;

            controlReg.word = (buff[0] << 8) | buff[1];

           return (controlReg.qwake ? true : false);
        }

        /// @brief Sets the OPT4048's effective sensing range which will effect its resolution.
        /// @param range The range to set the device to.
        ///   RANGE_2KLUX2,
        ///   RANGE_4KLUX5,
        ///   RANGE_9LUX,
        ///   RANGE_18LUX,
        ///   RANGE_36LUX,
        ///   RANGE_72LUX,
        ///   RANGE_144LUX,
        ///   RANGE_AUTO
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setRange(const opt4048_range_t range)
        {
            uint8_t buff[2];
            int8_t result = SFE_BUS_OK;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);
            Serial.print("Read result was: ");
            Serial.println(result);
            if(SFE_BUS_OK != result)
                return result;
            controlReg.word = (buff[0] << 8) | buff[1];
            controlReg.range = range;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);
            Serial.print("Write result was: ");
            Serial.println(result);

            return result;
        }
        
        /// @brief Retrieves the light range in lux of the OPT4048.
        /// @return Range of lux able to be measured.
        opt4048_range_t getRange(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t controlReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            controlReg.word = (buff[0] << 8) | buff[1];

            return (opt4048_range_t)controlReg.range;
        }

        /// @brief Sets the OPT4048's conversion time which will effect its resolution. Longer conversion time
        /// will result in higher resolution.
        /// @param time The conversion time to set the device to. Possible values:
        ///   CONVERSION_TIME_600US,
        ///   CONVERSION_TIME_1MS,
        ///   CONVERSION_TIME_1MS8,
        ///   CONVERSION_TIME_3MS4,
        ///   CONVERSION_TIME_6MS5,
        ///   CONVERSION_TIME_12MS7,
        ///   CONVERSION_TIME_25MS,
        ///   CONVERSION_TIME_50MS,
        ///   CONVERSION_TIME_100MS,
        ///   CONVERSION_TIME_200MS,
        ///   CONVERSION_TIME_400MS,
        ///   CONVERSION_TIME_800MS
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setConversionTime(const opt4048_conversion_time_t time)
        {
            uint8_t buff[2];
            int8_t result = SFE_BUS_OK;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return result;

            controlReg.word = buff[0] << 8 | buff[1];

            controlReg.conversion_time = time;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            return result;
        }

        /// @brief Retrieves the conversion time used for the ADC.
        /// @return The OPT4048 conversion time.
        opt4048_conversion_time_t getConversionTime(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t controlReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            controlReg.word = buff[0] << 8 | buff[1];

            return (opt4048_conversion_time_t)controlReg.conversion_time;
            
        }


        /// @brief Sets the OPT4048's operation mode.
        /// @param mode The mode to set the device to. Possible Values:
        ///   OPERATION_MODE_POWER_DOWN,
        ///   OPERATION_MODE_AUTO_ONE_SHOT,
        ///   OPERATION_MODE_ONE_SHOT,
        ///   OPERATION_MODE_CONTINUOUS
        /// @return 0 for success, negative for failure, positive for warning.
        int8_t setOperationMode(const opt4048_operation_mode_t mode)
        {
            uint8_t buff[2];
            int8_t result = SFE_BUS_OK;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return result;

            controlReg.word = buff[0] << 8 | buff[1];

            controlReg.op_mode = mode;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            return result;
        }

        /// @brief Retrieves the set operation mode.
        /// @return The OPT4048 conversion time.
        opt4048_operation_mode_t getOperationMode(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t controlReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            controlReg.word = buff[0] << 8 | buff[1];

            return (opt4048_operation_mode_t)controlReg.op_mode;
        }

        /// @brief Changes the behavior of the interrupt from pin to latch.
        /// @param enable True to enable, false to disable.
        /// @return True on successful execution.
        bool setIntLatch(const bool enable = true)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            controlReg.word = buff[0] << 8 | buff[1];

            controlReg.latch = enable;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;
        }

        /// @brief Checks if the interrupt is set to pulse or latch.
        /// @return True if set to latch, false if not.
        bool getIntLatch(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t controlReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            controlReg.word = buff[0] << 8 | buff[1];

            return (bool)controlReg.latch;
        }

        /// @brief Sets the OPT4048's interrupt polarity.
        /// @param setHigh True to enable, false to disable.
        /// @return True on successful execution.
        bool setIntActiveHigh(const bool setHigh = true)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_control_t intReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            intReg.word = buff[0] << 8 | buff[1];

            intReg.int_pol = (uint8_t)setHigh;

            buff[0] = intReg.word >> 8;
            buff[1] = intReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;
        }

        /// @brief Gets the OPT4048's interrupt polarity.
        /// @return True if active high.
        bool getIntActiveHigh(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t intReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            intReg.word = buff[0] << 8 | buff[1];

            return (bool)intReg.int_pol;
        }

        /// @brief Sets the number of faults (light values over or under) before an interrupt is triggered.
        /// @param count The number of faults to trigger an interrupt
        ///   FAULT_COUNT_1,
        ///   FAULT_COUNT_2,
        ///   FAULT_COUNT_4,
        ///   FAULT_COUNT_8
        /// @return True on successful execution.
        bool setFaultCount(const opt4048_fault_count_t *count)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_control_t controlReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            controlReg.word = buff[0] << 8 | buff[1];

            controlReg.fault_count = *count;

            buff[0] = controlReg.word >> 8;
            buff[1] = controlReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;
        }

        /// @brief Retrieves the number of faults (light values over or under) before 
        /// an interrupt is triggered.
        /// @return The fault count.
        opt4048_fault_count_t getFaultCount(void)
        {
            uint8_t buff[2];
            opt4048_reg_control_t controlReg;
            controlReg.fault_count = 0;

            readRegisterRegion(SFE_OPT4048_REGISTER_CONTROL, buff);

            controlReg.word = buff[0] << 8 | buff[1];

            return ((opt4048_fault_count_t)controlReg.fault_count);
        }

        /// @brief Sets the channel for threshold logic
        /// @param channel The channel to set the threshold logic to.
        ///   THRESH_CHANNEL_CH0,
        ///   THRESH_CHANNEL_CH1,
        ///   THRESH_CHANNEL_CH2,
        ///   THRESH_CHANNEL_CH3
        /// @return True on successful execution.
        bool setThresholdChannel(const opt4048_threshold_channel_t *channel)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_int_control_t intReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            intReg.word = buff[0] << 8 | buff[1];

            intReg.threshold_ch_sel = *channel;

            buff[0] = intReg.word >> 8;
            buff[1] = intReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;
        }

        /// @brief Retrives the threshold channel.
        /// @return THe selected channel. 
        opt4048_threshold_channel_t getThresholdChannel(void)
        {
            uint8_t buff[2];
            opt4048_reg_int_control_t intReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            intReg.word = buff[0] << 8 | buff[1];

            return ((opt4048_threshold_channel_t)intReg.threshold_ch_sel);
        }

        /// @brief Sets the Lux High Value threshold.
        /// @param thresh The value in float
        /// @return Returns the high threshold.
        bool setThresholdHigh(const float *thresh)
        {
            if (*thresh < 2.15 || *thresh > 144000)
                return false;

            uint8_t buff[2];
            int8_t result;
            // opt4048_reg_thresh_exp_res_high_t threshReg;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_THRESH_H_EXP_RES, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;            
        }
        
        /// @brief Retrieves the Lux High Value threshold.
        /// @return Returns the high threshold.
        uint16_t getThresholdHigh(void)
        {
            uint8_t buff[2];
            opt4048_reg_thresh_exp_res_high_t threshReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_THRESH_H_EXP_RES, buff);

            threshReg.word = buff[0] << 8 | buff[1];

            return threshReg.thresh_result << threshReg.thresh_exp;
        }

        /// @brief Sets the Lux Low Value threshold.
        /// @param thresh The value in float
        /// @return Returns the high threshold.
        bool setThresholdLow(const float *thresh)
        {
            if (*thresh < 2.15 || *thresh > 144000)
                return false;

            uint8_t buff[2];
            int8_t result;
            // opt4048_reg_thresh_exp_res_high_t threshReg;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_THRESH_L_EXP_RES, buff);

            if (SFE_BUS_OK != result)
                return false;

            return true;
        }

        /// @brief Retrieves the Lux Low Value threshold.
        /// @return Returns the low threshold.
        uint16_t getThresholdLow(void)
        {
            uint8_t buff[2];
            opt4048_reg_thresh_exp_res_low_t threshReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_THRESH_L_EXP_RES, buff);

            threshReg.word = buff[0] << 8 | buff[1];

            return threshReg.thresh_result << threshReg.thresh_exp;
        }

        /// @brief Enable CRC for ADC calues
        /// @param set True to enable, false to disable.
        /// @return True on successful execution.
        bool setCRC(const bool enable = true)
        {
            if (enable)
                crcEnabled = true;
            else
                crcEnabled = false;
        }

        ///////////////////////////////////////////////////////////////////Interrupt Settings
        /// @brief Changes the behavior of the interrupt pin to be an INPUT to trigger single shot.
        /// @param set True to enable, false to disable.
        /// @return True on successful execution.
        bool setIntInput(const bool enable = true)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_int_control_t intReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            intReg.word = buff[0] << 8 | buff[1];

            intReg.int_dir = (uint8_t)enable;

            buff[0] = intReg.word >> 8;
            buff[1] = intReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            return true;
        }

        /// @brief Gets the interrupt input bit
        /// @return True if the interrupt is set to INPUT.
        bool getIntInputEnable(void)
        {
            uint8_t buff[2];
            opt4048_reg_int_control_t intReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            intReg.word = buff[0] << 8 | buff[1];

            return (bool)intReg.int_dir;
        }

        /// @brief Changes the behavior interrupt mechanism after the end of conversion
        /// @param mechanism The mechanism to set
        /// @return True on successful execution.
        bool setIntMechanism(const opt4048_int_cfg_t *mechanism)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_int_control_t intReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            intReg.word = buff[0] << 8 | buff[1];

            intReg.int_cfg = *mechanism;

            buff[0] = intReg.word >> 8;
            buff[1] = intReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            return true;
        }

        /// @brief Gets the interrupt mechanism for the OPT4048
        /// @return Returns the setd mechanism.
        opt4048_int_cfg_t getIntMechanism(void)
        {
            uint8_t buff[2];
            opt4048_reg_int_control_t intReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            intReg.word = buff[0] << 8 | buff[1];

            return ((opt4048_int_cfg_t)intReg.int_cfg);
        }

        /// @brief Enable register auto increment .
        /// @param enable True to enable, false to disable.
        /// @return True on successful execution.
        bool setI2CBurst(const bool enable = true)
        {
            uint8_t buff[2];
            int8_t result;
            opt4048_reg_int_control_t intReg;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            intReg.word = buff[0] << 8 | buff[1];

            intReg.i2c_burst = (uint8_t)enable;

            buff[0] = intReg.word >> 8;
            buff[1] = intReg.word;

            result = writeRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            if (SFE_BUS_OK !=result)
                return false;

            return true;
        }

        /// @brief Retrieves the I2C burst bit.
        /// @return True if I2C burst is set, false otherwise.
        bool getI2CBurst(void)
        {
            uint8_t buff[2];
            opt4048_reg_int_control_t intReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_INT_CONTROL, buff);

            intReg.word = buff[0] << 8 | buff[1];

            return (bool)intReg.i2c_burst;
        }

        /// @brief Retrieves the flag register
        /// @return The contents of the flag register
        opt4048_reg_flags_t getAllFlags(void)
        {
            uint8_t buff[2];
            opt4048_reg_flags_t flagReg;

            readRegisterRegion(SFE_OPT4048_REGISTER_FLAGS, buff);

            flagReg.word = buff[0] << 8 | buff[1];

            return flagReg;
        }

        /// @brief Checks the overload flag bit.
        /// @return True if the overload flag bit is set, false otherwise
        bool getOverloadFlag(void)
        {
            opt4048_reg_flags_t flagReg;
            flagReg = getAllFlags();

            return (bool)flagReg.overload_flag;
        }

        /// @brief Checks the conversion ready flag bit.
        /// @return True if that flag bit is set, false otherwise
        bool getConvReadyFlag(void)
        {
            opt4048_reg_flags_t flagReg;
            flagReg = getAllFlags();

            return (bool)flagReg.conv_ready_flag;
        }

        /// @brief Checks the too bright flag bit.
        /// @return True if that flag bit is set, false otherwise
        bool getTooBrightFlag(void)
        {
            opt4048_reg_flags_t flagReg;
            flagReg = getAllFlags();

            return (bool)flagReg.flag_high;
        }

        /// @brief Checks the too dim flag bit.
        /// @return True if that flag bit is set, false otherwise
        bool getTooDimFlag(void)
        {
            opt4048_reg_flags_t flagReg;
            flagReg = getAllFlags();

            return (bool)flagReg.flag_low;
        }
        
        ///////////////////////////////////////////////////////////////////Color Information
        /// @brief Reads Channel Zero (Red)
        /// @return Returns the ADC value of Channel Zero
        uint32_t getADCCh0(void)
        {
            uint8_t buff[4];
            uint32_t mantissa;
            opt4048_reg_exp_res_ch0_t adcReg;
            opt4048_reg_res_cnt_crc_ch0_t adc1Reg;

            readRegisterRegion(SFE_OPT4048_REGISTER_EXP_RES_CH0, buff, 4);

            adcReg.word = buff[0] << 8 | buff[1];
            adc1Reg.word = buff[2] << 8 | buff[3];

            mantissa = adcReg.result_msb_ch0 << 8 | adc1Reg.result_lsb_ch0;

            return mantissa << adcReg.exponent_ch0;
        }

        /// @brief Reads Channel One (Green)
        /// @return Returns the ADC value of Channel One
        uint32_t getADCCh1(void)
        {
            uint8_t buff[4];
            uint32_t mantissa;
            opt4048_reg_exp_res_ch0_t adcReg;
            opt4048_reg_res_cnt_crc_ch0_t adc1Reg;

            readRegisterRegion(SFE_OPT4048_REGISTER_EXP_RES_CH1, buff, 4);

            adcReg.word = buff[0] << 8 | buff[1];
            adc1Reg.word = buff[2] << 8 | buff[3];

            mantissa = adcReg.result_msb_ch0 << 8 | adc1Reg.result_lsb_ch0;

            return mantissa << adcReg.exponent_ch0;
        }

        /// @brief Reads Channel Two (Blue)
        /// @return Returns the ADC value of Channel Two
        uint32_t getADCCh2(void)
        {
            uint8_t buff[4];
            uint32_t mantissa;
            opt4048_reg_exp_res_ch0_t adcReg;
            opt4048_reg_res_cnt_crc_ch0_t adc1Reg;

            readRegisterRegion(SFE_OPT4048_REGISTER_EXP_RES_CH2, buff, 4);

            adcReg.word = buff[0] << 8 | buff[1];
            adc1Reg.word = buff[2] << 8 | buff[3];

            mantissa = adcReg.result_msb_ch0 << 8 | adc1Reg.result_lsb_ch0;

            return mantissa << adcReg.exponent_ch0;
        }

        /// @brief Reads Channel Three (White)
        /// @return Returns the ADC value of Channel Three
        uint32_t getADCCh3(void)
        {
            uint8_t buff[4];
            uint32_t mantissa;
            opt4048_reg_exp_res_ch0_t adcReg;
            opt4048_reg_res_cnt_crc_ch0_t adc1Reg;

            readRegisterRegion(SFE_OPT4048_REGISTER_EXP_RES_CH3, buff, 4);

            adcReg.word = buff[0] << 8 | buff[1];
            adc1Reg.word = buff[2] << 8 | buff[3];

            mantissa = adcReg.result_msb_ch0 << 8 | adc1Reg.result_lsb_ch0;

            return mantissa << adcReg.exponent_ch0;
        }

        /// @brief Retrieves all ADC values for all channels: Red, Green, Blue, and White.
        /// @return Returns the ADC values of the channels.
        sfe_color_t getAllADC(void)
        {
            sfe_color_t color;

            color.red = getADCCh0();
            color.green = getADCCh1();
            color.blue = getADCCh2();
            color.white = getADCCh3();

            return color;
        }

        /// @brief Retrieves all ADC values for all channels: Red, Green, Blue, and White, as well as the sample counter, and
        ///        the CRC value.
        /// @param color Pointer to the color struct to be populated with the channels values.
        /// @return Returns true on successful execution, false otherwise.
        bool getAllChannelData(sfe_color_t *color)
        {
            int32_t result;
            uint8_t buff[16];
            uint32_t mantissaCh0;
            uint32_t mantissaCh1;
            uint32_t mantissaCh2;
            uint32_t mantissaCh3;
            uint32_t adcCodeCh0;
            uint32_t adcCodeCh1;
            uint32_t adcCodeCh2;
            uint32_t adcCodeCh3;

            opt4048_reg_exp_res_ch0_t adc0MSB;
            opt4048_reg_res_cnt_crc_ch0_t adc0LSB;
            opt4048_reg_exp_res_ch1_t adc1MSB;
            opt4048_reg_res_cnt_crc_ch1_t adc1LSB;
            opt4048_reg_exp_res_ch2_t adc2MSB;
            opt4048_reg_res_cnt_crc_ch2_t adc2LSB;
            opt4048_reg_exp_res_ch3_t adc3MSB;
            opt4048_reg_res_cnt_crc_ch3_t adc3LSB;

            result = readRegisterRegion(SFE_OPT4048_REGISTER_EXP_RES_CH0, buff, 16);

            if (SFE_BUS_OK != result)
                return false;

            adc0MSB.word = buff[0] << 8 | buff[1];
            adc0LSB.word = buff[2] << 8 | buff[3];

            adc1MSB.word = buff[4] << 8 | buff[5];
            adc1LSB.word = buff[6] << 8 | buff[7];

            adc2MSB.word = buff[8] << 8 | buff[9];
            adc2LSB.word = buff[10] << 8 | buff[11];

            adc3MSB.word = buff[12] << 8 | buff[13];
            adc3LSB.word = buff[14] << 8 | buff[15];

            mantissaCh0 = adc0MSB.result_msb_ch0 << 8 | adc0LSB.result_lsb_ch0;
            adcCodeCh0 = mantissaCh0 << adc0MSB.exponent_ch0;

            mantissaCh1 = adc1MSB.result_msb_ch1 << 8 | adc1LSB.result_lsb_ch1;
            adcCodeCh1 = mantissaCh1 << adc1MSB.exponent_ch1;

            mantissaCh2 = adc2MSB.result_msb_ch2 << 8 | adc2LSB.result_lsb_ch2;
            adcCodeCh2 = mantissaCh2 << adc2MSB.exponent_ch2;

            mantissaCh3 = adc3MSB.result_msb_ch3 << 8 | adc3LSB.result_lsb_ch3;
            adcCodeCh3 = mantissaCh3 << adc3MSB.exponent_ch3;

            color->red = adcCodeCh0;
            color->green = adcCodeCh1;
            color->blue = adcCodeCh2;
            color->white = adcCodeCh3;

            color->counterR = adc0LSB.counter_ch0;
            color->counterG = adc1LSB.counter_ch1;
            color->counterR = adc2LSB.counter_ch2;
            color->counterR = adc3LSB.counter_ch3;

            color->CRCR = adc0LSB.crc_ch0;
            color->CRCG = adc1LSB.crc_ch1;
            color->CRCB = adc2LSB.crc_ch2;
            color->CRCW = adc3LSB.crc_ch3;

            return true;
        }

        /// @brief  Calculates the CRC for the OPT4048. Note that the OPT4048 does this already
        ///         but this is a way to double check the value.
        /// @param mantissa The mantissa value of the ADC
        /// @param exponent The exponent value of the ADC
        /// @param crc The CRC value of the ADC
        /// @return Returns the calculated CRC value.
        bool validateCRC(uint32_t mantissa, uint8_t expon, uint8_t crc)
        {
            if(!crcEnabled)
                return false; 

            mantissaBits mBits;
            exponBits exBits;
            crcBits cBits;
            crcBits compareAgainst;

            mBits.word = mantissa;
            exBits.byte = expon;
            cBits.byte = crc;

            compareAgainst.bit0 = exBits.byte xor mantissa xor cBits.byte;

            compareAgainst.bit1 = cBits.bit1 xor cBits.bit3 xor mBits.bit1 xor mBits.bit3 xor mBits.bit5 xor mBits.bit7 xor
                                mBits.bit9 xor mBits.bit11 xor mBits.bit13 xor mBits.bit15 xor mBits.bit17 xor mBits.bit19 xor
                                exBits.bit1 xor exBits.bit3;

            compareAgainst.bit2 =
                cBits.bit3 xor mBits.bit3 xor mBits.bit7 xor mBits.bit11 xor mBits.bit16 xor mBits.bit18 xor exBits.bit3;

            compareAgainst.bit3 = mBits.bit3 xor mBits.bit11 xor mBits.bit19;

            if (compareAgainst.byte == crc)
                return true;

            return false;
        }

        /// @brief Retrieves the Lux value.
        /// @return Returns the Lux value of the sensor
        uint32_t getLux(void)
        {
            uint32_t adcCh1;
            uint32_t lux;

            adcCh1 = getADCCh1();
            lux = adcCh1 * cieMatrix[1][3];

            return lux;
        }

        /// @brief  Retrieves the CIE X value of the sensor.
        /// @return Returns the CIE X value of the sensor
        double getCIEx(void)
        {
            double x = 0;
            double y = 0;
            double z = 0;
            double CIEx;
            static sfe_color_t color;

            getAllChannelData(&color);

            for (int row = 0; row < OPT_MATRIX_ROWS; row++)
            {
                x += color.red * cieMatrix[row][0];
                y += color.green * cieMatrix[row][1];
                z += color.blue * cieMatrix[row][2];
            }

            CIEx = x / (x + y + z);

            return CIEx;
        }

        /// @brief Retrieves the CIE Y value of the sensor.
        /// @return Returns the CIE Y value of the sensor
        double getCIEy(void)
        {
            double x = 0;
            double y = 0;
            double z = 0;
            double CIEy;
            sfe_color_t color;

            getAllChannelData(&color);

            for (int row = 0; row < OPT_MATRIX_ROWS; row++)
            {
                x += color.red * cieMatrix[row][0];
                y += color.green * cieMatrix[row][1];
                z += color.blue * cieMatrix[row][2];
            }

            CIEy = y / (x + y + z);

            return CIEy;
        }

        /// @brief Retrieves the Correlated Color Temperature (CCT) of the sensor.
        /// @return Returns the CCT of the sensor in Kelvin
        double getCCT(void)
        {
                double CIEx;
            double CIEy;
            double CCT;

            CIEx = getCIEx();
            CIEy = getCIEy();

            double n = (CIEx - 0.3320) / (0.1858 - CIEy);

            // Formula can be found under the CCT section in the datasheet.
            CCT = 432 * pow(n, 3) + 3601 * pow(n, 2) + 6861 * n + 5517; 

            return CCT;
        }

    protected:
        sfeBusDevice *_sfeBus;
        sfeBusDeviceSettings *_devSettings;
        bool crcEnabled = false;

        const double cieMatrix[OPT_MATRIX_ROWS][OPT_MATRIX_COLS] = {
            {.000234892992, -.0000189652390, .0000120811684, 0},
            {.0000407467441, .000198958202, -.0000158848115, .00215},
            {.0000928619404, -.0000169739553, .000674021520, 0},
            {0, 0, 0, 0}};
};

class sfeOPT4048ArduinoI2C : public OPT4048SFEBusBase<SFEBusArduinoI2C, SFEBusDevSettingsI2C> 
{
    /* Nothing to see here, see above. */
};