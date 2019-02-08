// Custom config for fc3.  Basically a modified food computer V2 config.

// Put includes for all the system's modules here.
//
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <openag_mhz16.h>
#include <openag_atlas_ph.h>
#include <openag_atlas_ec.h>

#include <openag_binary_sensor.h>
#include <openag_binary_actuator.h>
#include <openag_pwm_actuator.h>
#include <openag_doser_pump.h>
#include <openag_pulse_actuator.h>
#include <openag_tone_actuator.h>

// Put instantiations of all the system's modules here.  
//
// Sensors
//
Am2315 am2315_1;                                   // Air Temp and Humidity, I2C, Addr = 92
Ds18b20 ds18b20_1(5);                              // Water Temp, One Wire
MHZ16 mhz16_1(77);                                 // CO2, I2C, Addr = 77
AtlasPh atlas_ph_1(99);                            // PH, I2C, Addr = 99
AtlasEc atlas_ec_1(100);                           // EC, I2C, Addr = 100
BinarySensor water_level_sensor_high_1(3, false);
BinarySensor water_level_sensor_low_1(4, false);
//
// Actuator Instances. Sorted by pin number.
//
ToneActuator chiller_compressor_1(9, false, 140, -1);
DoserPump pump_1_nutrient_a_1(28, true);
DoserPump pump_2_nutrient_b_1(29, true);
PulseActuator pump_3_ph_up_1(30, true);
PulseActuator pump_4_ph_down_1(31, true);
BinaryActuator pump_5_water_1(32, true, 10000);
BinaryActuator chiller_fan_1(33, true, 10000);
BinaryActuator chiller_pump_1(34, true, 10000);
BinaryActuator heater_core_2_1(35, true, 10000);
BinaryActuator air_flush_1(36, true, 10000);
BinaryActuator water_circulation_pump_1(37, true, 10000);
BinaryActuator water_aeration_pump_1(38, true, 10000);
BinaryActuator chamber_fan_1(39, true, 10000);
//PwmActuator led_blue_1(40, true, 0);
//PwmActuator led_white_1(41, true, 0);
//PwmActuator led_red_1(42, true, 0);
BinaryActuator heater_core_1_1(43, true, 10000);
BinaryActuator grow_light(10, false, 10000);

// Put pointers to instantiated modules into the mod_ptr_array.
// Remember to update NMODS and ACTUATOR_OFFSET to be correct.
const uint8_t NMODS = 22;
const uint8_t ACTUATOR_OFFSET = 7;

Module *mod_ptr_array[] = {
   &am2315_1,                   //0 - Put sensors at the head of the list.
   &ds18b20_1,                  //1
   &mhz16_1,                    //2
   &atlas_ph_1,                 //3
   &atlas_ec_1,                 //4
   &water_level_sensor_high_1,  //5
   &water_level_sensor_low_1,   //6
   &air_flush_1,                //7 - First Actuator in the list.
   &chamber_fan_1,              //8
   &chiller_fan_1,              //9
   &chiller_compressor_1,       //10
   &chiller_pump_1,             //11
   &heater_core_2_1,            //12
   &water_aeration_pump_1,      //13
   &water_circulation_pump_1,   //14
   &heater_core_1_1,            //15
   &pump_1_nutrient_a_1,        //16
   &pump_2_nutrient_b_1,        //17
   &pump_5_water_1,             //18
   &pump_3_ph_up_1,             //19
   &pump_4_ph_down_1,           //20
   &grow_light                  //21
};

// Put the name of the modules in the mname_array data structure.
//
//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "air_temp_hum",
   "water_temp",
   "co2",
   "ph",
   "ec",
   "water_high",
   "water_low",
   "air_flush",
   "chamber_fan",
   "chiller_fan",
   "chiller_comp",
   "chiller_pump",
   "heater_core_2",
   "water_aeration",
   "water_circ_pump",
   "heater_core_1",
   "nut_pump_1",
   "nut_pump_2",
   "water_pump",
   "ph_up",
   "ph_down",
   "grow_light"
};
