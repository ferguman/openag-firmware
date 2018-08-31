#ifndef OPENAG_MODULE
#define OPENAG_MODULE

#include <pair.h>
#include <types.h>

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

static const uint8_t OK = 0;
static const uint8_t WARN = 1;
static const uint8_t ERROR = 2;

static const uint8_t CODE_OK = 0;

//- class Module {
class SensorPh : public Module {

  public:
    SensorPh();
    uint8_t begin();
    uint8_t update();
    void print_readings_as_csv();
    int cmd(int args);
    float get_ph();

//  private:

};

#endif
