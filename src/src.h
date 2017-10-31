#ifndef SRC
#define SRC

#include <openag_module.h>
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <openag_binary_actuator.h>
#include <openag_pwm_actuator.h>

   extern Am2315 am2315_1;
   extern Ds18b20 ds18b20_1;

   extern BinaryActuator air_flush_1;
   extern BinaryActuator chamber_fan_1;
   extern BinaryActuator chiller_fan_1;
   extern PwmActuator led_blue_1;
   extern PwmActuator led_white_1;
   extern PwmActuator led_red_1;
   extern MHZ16 mhz16_1;

#endif
