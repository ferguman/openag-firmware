/** 
 *  \file sensor_dht22.h
 *  \brief Sensor module for air temperature and humidity.
 */

// Library based off: DHT library from Seeed Studio
// Library found at: https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
// Component found at: http://www.seeedstudio.com/depot/grove-temperaturehumidity-sensor-pro-p-838.html?cPath=144_147
// Modified by: Jake Rye

#ifndef SensorDht22_H
#define SensorDht22_H

#include "Arduino.h"
#include <openag_module.h>

// 8 MHz(ish) AVR ---------------------------------------------------------
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
#define COUNT 3
// 16 MHz(ish) AVR --------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
#define COUNT 6
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

/** 
 *  \brief Sensor module for air temperature and humidity.
 */
class SensorDht22 : public Module {

  public:
    SensorDht22(int pin);

    uint8_t begin();
    uint8_t update();
    float get_air_temperature();
    float get_air_humidity();
    void print_readings_as_csv();
    int cmd(int args);
    
  private:
    boolean read();
    void getRawSensorData();
    
    int pin_;
    uint8_t data[6];
    uint8_t count_;
    uint32_t last_read_time_;
    boolean first_reading_;
    float humidity_raw_;
    float temperature_raw_;

    // status codes
    static const uint8_t CODE_CHECKSUM_FAILURE = 1;
	    
};

#endif // SensorDht22_H
