#ifndef DFROBOT_PH 
#define DFROBOT_PH

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <openag_module.h>

class SensorPh : public Module {

  public:
    SensorPh(int analog_pin);
    uint8_t begin();
    uint8_t update();
    void print_readings_as_csv();
    int cmd(int args);
    float get_ph();

  private:
    int pin_;
    float ph_;
    float ph_calibration_offset_ = -3.036; 
    float ph_calibration_coefficient_ = 4.406;
};

#endif
