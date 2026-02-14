/******************************************************************************
 * @file BlackIoT_BMV080_Arduino_Library.cpp
 * 
 * This file implements the BMV080SPI classes,
 * for use with the BMV080 sensor.
 *
 * This library is derived from the SparkFun_BMV080_Arduino_Library
 *
 ******************************************************************************/

#if __has_include("bmv080.h")
#include "bmv080.h"
#include "bmv080_defs.h"
#else
#error                                                                                                                 \
    "BOSCH BMV080 SDK Must be installed. ERROR: bmv080.h not found"

#endif

#include "BlackIoT_BMV080_Arduino_Library.h"


#include <cstring>

// Some communication functions used with the system. These are from the original code from
// Bosch  - so keeping them the same. It is unclear if the library they provide depends on these
// specific values - it probably does - so leaving as is.

#define E_COMBRIDGE_OK ((int8_t)0)
/*! -1: Status codes returned when memory allocation fails */
#define E_COMBRIDGE_ERROR_MEMORY_ALLOCATION ((int8_t)-1)
/*! -2: Status codes returned when the read operation fails */
#define E_COMBRIDGE_ERROR_READ ((int8_t)-2)
/*! -3: Status codes returned when the write operation fails */
#define E_COMBRIDGE_ERROR_WRITE ((int8_t)-3)
/*! -4: Status codes returned when writing the header fails */
#define E_COMBRIDGE_ERROR_WRITE_HEADER ((int8_t)-4)
/*! -5:  Status codes returned when a reference is null */
#define E_COMBRIDGE_ERROR_NULLPTR ((int8_t)-5)

// C function used in this library only - so static

/* Our bus read and write functions */

// --------------------------------------------------------------------------------------------
// BOSCH API Callbacks
// --------------------------------------------------------------------------------------------
// Callback for reading data-- called from the Bosch supplied library
//
// static int8_t device_read_16bit_CB(bmv080_sercom_handle_t handle, uint16_t header, uint16_t *payload,
int8_t BMV080SPI::device_read_16bit_CB(bmv080_sercom_handle_t handle, uint16_t header, uint16_t *payload,
                                         uint16_t payload_length)
{
    if (handle == nullptr)
        return E_COMBRIDGE_ERROR_NULLPTR;

    // Get our sparkfun toolkit bus object/interface
    SPIClass *_spi_bmv080 = (SPIClass *)handle;

    // Apply settings
    _spi_bmv080->beginTransaction(spi_bmv080_Settings);

    // Signal communication start
    digitalWrite(_spi_bmv080->pinSS(), LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spi_bmv080->transfer16(header);

    for (size_t i = 0; i < payload_length; i++)
        *payload++ = _spi_bmv080->transfer16(0x00);

    // End transaction
    digitalWrite(_spi_bmv080->pinSS(), HIGH);
    _spi_bmv080->endTransaction();

    return E_COMBRIDGE_OK;
}

// --------------------------------------------------------------------------------------------
// Callback for reading data-- called from the Bosch supplied library
//
int8_t BMV080SPI::device_write_16bit_CB(bmv080_sercom_handle_t handle, uint16_t header, const uint16_t *payload,
                                          uint16_t payload_length)
{
    if (handle == nullptr)
        return E_COMBRIDGE_ERROR_NULLPTR;

    // Get our sparkfun toolkit bus object/interface
    SPIClass *_spi_bmv080 = (SPIClass *)handle;
    
    // Apply settings
    _spi_bmv080->beginTransaction(spi_bmv080_Settings);

    // Signal communication start
    digitalWrite(_spi_bmv080->pinSS(), LOW);

    // A leading "1" must be added to transfer with devRegister to indicate a "read"
    _spi_bmv080->transfer16(header);

    for (size_t i = 0; i < payload_length; i++)
        _spi_bmv080->transfer16(*payload++);

    // End transaction
    digitalWrite(_spi_bmv080->pinSS(), HIGH);
    _spi_bmv080->endTransaction();

    return E_COMBRIDGE_OK;
}

// --------------------------------------------------------------------------------------------
// Delay callback function for the Bosch library
//
int8_t BMV080SPI::device_delay_CB(uint32_t period)
{
    delay(period);

    return E_COMBRIDGE_OK;
}

//---------------------------------------------------------------------
// helpful class method/callback

/* Custom function for consuming sensor readings */
void BMV080SPI::set_sensor_value(bmv080_output_t bmv080_output, void *callback_parameters)
{
    ((BMV080SPI *)callback_parameters)->setSensorValue(bmv080_output);
}

//---------------------------------------------------------------------
// End CB functions
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// Core object implementation
//---------------------------------------------------------------------

bool BMV080SPI::begin(SPIClass &spiPort)
{
    spi_bmv080 = &spiPort;
    return spi_bmv080->begin(BMV080_SCK, BMV080_MISO, BMV080_MOSI, BMV080_CS);
}

SPISettings BMV080SPI::spi_bmv080_Settings = SPISettings(100000, MSBFIRST, SPI_MODE0);
//---------------------------------------------------------------------
float BMV080SPI::PM10()
{
    return _sensorValue.pm10_mass_concentration;
}

//---------------------------------------------------------------------
float BMV080SPI::PM25()
{
    return _sensorValue.pm2_5_mass_concentration;
}

//---------------------------------------------------------------------
float BMV080SPI::PM1()
{
    return _sensorValue.pm1_mass_concentration;
}

//---------------------------------------------------------------------
bool BMV080SPI::isObstructed()
{
    return _sensorValue.is_obstructed;
}

//---------------------------------------------------------------------
void BMV080SPI::setSensorValue(bmv080_output_t bmv080_output)
{
    // TODO: should here be a mode where the library user can set register a callback function to handle the data?
    //       This way the end user can get all the sensor data at once - possible issue is stack/re-entrancy
    _dataAvailable = true;

    // cache the latest sensor values - copy output to our class variable
    _sensorValue = bmv080_output;
}

//---------------------------------------------------------------------
// Read the latest values from the sensor.
//
// Return the value if a struct is passed in.
bool BMV080SPI::readSensor(bmv080_output_t *bmv080_output /* default is nullptr*/)
{
    _dataAvailable = false;
    if (!sensorServiceRoutine())
        return false;

    if (_dataAvailable && bmv080_output != nullptr)
        *bmv080_output = _sensorValue;

    return _dataAvailable;
}

//---------------------------------------------------------------------
bool BMV080SPI::setMode(uint8_t mode)
{
    bmv080_status_code_t bmv080_current_status =
        E_BMV080_ERROR_PARAM_INVALID_VALUE; // return status from the Bosch API function

    if (mode == BMV080_MODE_CONTINUOUS)
    {
        bmv080_current_status = bmv080_start_continuous_measurement(_bmv080_handle_class);
    }
    else if (mode == BMV080_MODE_DUTY_CYCLE)
    {
        bmv080_duty_cycling_mode_t duty_cycling_mode = E_BMV080_DUTY_CYCLING_MODE_0;
        bmv080_current_status = bmv080_start_duty_cycling_measurement(
            _bmv080_handle_class, (bmv080_callback_tick_t)millis, duty_cycling_mode);
    }

    // check if the mode was set correctly
    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
// Called to pump the service routine of the BMV080 sensor driver
//

bool BMV080SPI::sensorServiceRoutine(void)
{
    if (_bmv080_handle_class == NULL)
        return false;
    /* The interrupt is served by the BMV080 sensor driver */
    bmv080_status_code_t bmv080_current_status =
        bmv080_serve_interrupt(_bmv080_handle_class, (bmv080_callback_data_ready_t)set_sensor_value, (void *)this);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
// Our init method
bool BMV080SPI::init(void)
{
    // Do we have a bus?
    if (spi_bmv080 == nullptr)
        return false;
    
    // Call various bosch API functions to initialize the sensor
    uint16_t major, minor, patch;
    char id[kBMV800IDLength];
    if (!driverVersion(major, minor, patch) || !open() || !reset() || !ID(id))
        return false;

    return true;
}

//---------------------------------------------------------------------
bool BMV080SPI::open()
{
    if (spi_bmv080 == nullptr)
        return false;

    // Open the device - pass in the data read, data write and delay functions callbacks. Note - the "sercom_handle_t"
    // is just a pointer to our Toolkit communication bus objects

    bmv080_status_code_t status = bmv080_open(
        &_bmv080_handle_class, (bmv080_sercom_handle_t) spi_bmv080, (bmv080_callback_read_t) device_read_16bit_CB,
        (bmv080_callback_write_t)device_write_16bit_CB, (bmv080_callback_delay_t)device_delay_CB);

    return (status == E_BMV080_OK);
}

//---------------------------------------------------------------------
bool BMV080SPI::close()
{
    if (spi_bmv080 == nullptr)
        return false;

    // Close the device

    bmv080_status_code_t status = bmv080_close(&_bmv080_handle_class);

    return (status == E_BMV080_OK);
}

//---------------------------------------------------------------------
bool BMV080SPI::reset()
{
    bmv080_status_code_t bmv080_current_status = bmv080_reset(_bmv080_handle_class);

    return (bmv080_current_status == E_BMV080_OK);
}
//---------------------------------------------------------------------
bool BMV080SPI::driverVersion(uint16_t &major, uint16_t &minor, uint16_t &patch)
{
    char git_hash[12];
    int32_t commits_ahead = 0;

    bmv080_status_code_t bmv080_current_status =
        bmv080_get_driver_version(&major, &minor, &patch, git_hash, &commits_ahead);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
// Method to get the ID
bool BMV080SPI::ID(char idOut[kBMV800IDLength])
{
    memset(idOut, 0x00, kBMV800IDLength);
    bmv080_status_code_t bmv080_current_status = bmv080_get_sensor_id(_bmv080_handle_class, idOut);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
uint16_t BMV080SPI::dutyCyclingPeriod()
{
    uint16_t duty_cycling_period = 0;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "duty_cycling_period", (void *)&duty_cycling_period);

    return (bmv080_current_status == E_BMV080_OK ? duty_cycling_period : 0);
}

//---------------------------------------------------------------------
bool BMV080SPI::setDutyCyclingPeriod(uint16_t duty_cycling_period)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "duty_cycling_period", (void *)&duty_cycling_period);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
float BMV080SPI::volumetricMassDensity()
{
    float volumetric_mass_density = 0.0;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "volumetric_mass_density", (void *)&volumetric_mass_density);

    return (bmv080_current_status == E_BMV080_OK ? volumetric_mass_density : 0.0);
}

//---------------------------------------------------------------------
bool BMV080SPI::setVolumetricMassDensity(float volumetric_mass_density)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "volumetric_mass_density", (void *)&volumetric_mass_density);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
float BMV080SPI::integrationTime()
{
    float integration_time = 0.0;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "integration_time", (void *)&integration_time);

    return (bmv080_current_status == E_BMV080_OK ? integration_time : 0.0);
}

//---------------------------------------------------------------------
bool BMV080SPI::setIntegrationTime(float integration_time)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "integration_time", (void *)&integration_time);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
uint32_t BMV080SPI::distributionId()
{
    uint32_t distribution_id = 0;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "distribution_id", (void *)&distribution_id);

    return (bmv080_current_status == E_BMV080_OK ? distribution_id : 0);
}

//---------------------------------------------------------------------

bool BMV080SPI::setDistributionId(uint32_t distribution_id)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "distribution_id", (void *)&distribution_id);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
bool BMV080SPI::doObstructionDetection()
{
    bool do_obstruction_detection = false;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "do_obstruction_detection", (void *)&do_obstruction_detection);

    return (bmv080_current_status == E_BMV080_OK ? do_obstruction_detection : false);
}

//---------------------------------------------------------------------
bool BMV080SPI::setDoObstructionDetection(bool do_obstruction_detection)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "do_obstruction_detection", (void *)&do_obstruction_detection);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
bool BMV080SPI::doVibrationFiltering()
{
    bool do_vibration_filtering = false;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "do_vibration_filtering", (void *)&do_vibration_filtering);

    return (bmv080_current_status == E_BMV080_OK ? do_vibration_filtering : false);
}

//---------------------------------------------------------------------
bool BMV080SPI::setDoVibrationFiltering(bool do_vibration_filtering)
{
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "do_vibration_filtering", (void *)&do_vibration_filtering);

    return (bmv080_current_status == E_BMV080_OK);
}

//---------------------------------------------------------------------
uint8_t BMV080SPI::measurementAlgorithm()
{
    bmv080_measurement_algorithm_t measurement_algorithm;
    bmv080_status_code_t bmv080_current_status =
        bmv080_get_parameter(_bmv080_handle_class, "measurement_algorithm", (void *)&measurement_algorithm);

    return (bmv080_current_status == E_BMV080_OK ? (uint8_t)measurement_algorithm : 0);
}

//---------------------------------------------------------------------
bool BMV080SPI::setMeasurementAlgorithm(uint8_t measurement_algorithm)
{
    bmv080_measurement_algorithm_t bmv080_measurement_algorithm = (bmv080_measurement_algorithm_t)measurement_algorithm;
    bmv080_status_code_t bmv080_current_status =
        bmv080_set_parameter(_bmv080_handle_class, "measurement_algorithm", (void *)&bmv080_measurement_algorithm);

    return (bmv080_current_status == E_BMV080_OK);
}