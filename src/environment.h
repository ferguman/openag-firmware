#include "Arduino.h"
#include <openag_module.h>
#include <openag_ds18b20.h>
#include <openag_am2315.h>

#ifndef SENSOR_ACCUATOR_ENV
#define SENSOR_ACCUATOR_ENV 1

// Add lines to instantiate instances of the sensors and actuators that your grow environment
// supports.  
#ifndef am2315_1
Am2315 am2315_1;
Ds18b20 ds18b20_1(5);
#endif

#endif
