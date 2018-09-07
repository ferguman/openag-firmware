/** 
 *  \file sensor_gc0011.h
 *  \brief Sensor module for air co2, temperature, and humidity.
 */

// Library based off: Cozir Example Sketch from CO2Meter.com
// Library found at: http://www.co2meters.com/Documentation/AppNotes/AN128-%20Cozir_Arduino.pdf
// Component found at: http://www.co2meter.com/products/cozir-0-2-co2-sensor
// Modified by Jake Rye
// July 15, 2015
// Modified by Joe O'Brien
// August 28, 2018

#ifndef SENSOR_GC0011_H
#define SENSOR_GC0011_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <support_software_serial.h>
#include <openag_module.h>

/** 
 *  \brief Sensor module for air co2, temperature, and humidity.
 */
class SensorGc0011 : public Module {

  public:
    SensorGc0011(int rx_pin, int tx_pin);
    uint8_t begin();
    uint8_t update();
    uint8_t set_cmd(const char *cmd);
    int cmd(int args);
    float get_co2();
    float get_temperature();
    float get_humidity();
    void print_readings_as_csv();

  private:
    // Private Functions
    void getSensorData(void);
    void sendMessage(String message);
    String receiveMessage();

    // Private Variables
    int rx_pin_;
    int tx_pin_;

    float temperature_;
    float humidity_;
    float co2_;

    uint32_t _time_of_last_reading;
    const static uint32_t _min_update_interval = 10000;

    SoftwareSerial *ss_;
    uint32_t timeout_;

    static const bool TEMPERATURE_AND_HUMIDITY_ENABLED = false;
};

#endif // SENSOR_GC0011_H_
