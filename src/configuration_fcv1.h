// Put includes for all the system's modules here.
//
#include <dht22.h>
#include <tsl2561.h>
#include <gc0011.h>
#include <ph.h>
#include <openag_ds18b20.h>
#include <ec.h>
#include <openag_binary_sensor.h>

#include <openag_binary_actuator.h>

// Put instantiations of all the system's modules here.  
// Food Computer V1 -> see https://github.com/OpenAgInitiative/gro-hardware/blob/master/ElectronicsSchematicLarge.pdf
//
// Sensors
//
SensorDht22 dht22(A0);                               // air temperature and humidity
SensorTsl2561 tsl2561(0x29);                         // tsl2561 has 3 possible i2c address (0x39, 0x29, 0x49)
                                                     // selectable with jumpers.
SensorGc0011 co2(12, 11);                            // GC0011 CO2 sensor rx pin=12, tx pin=11
SensorPh ph(A1);                                     // Analog sensor on pin A1
Ds18b20 water_temp(5);                               // Water temperature 1-Wire probe on Pin 5.
SensorEc ec(A2, 2);                                  // Analog ec sensor on A2 with sensor power controlled by pin 2.
BinarySensor shell_open(4, 1);                       // Food Computer shell off contact switch - active high
BinarySensor window_open(3, 1);                      // Grow chamber window off contact switch - active high

//
// Actuator Instances.
//
BinaryActuator humidifier(9, true, 10000);           //AC port 1 - Humidifier
BinaryActuator grow_light(8, true, 10000);           //AC port 2 - Grow Light
BinaryActuator ac_3(7, true, 10000);                 //AC port 3 - Unused switched 120 VAC line 
BinaryActuator air_heat(6, true, 10000);             //AC port 4 - Air Heater
BinaryActuator vent_fan(14, true, 10000);            //5VDC - Ventilation Fan
BinaryActuator circ_fan(15, true, 10000);            //5VDC - Circulation Fan
BinaryActuator chamber_light(53, true, 10000);       //5VDC - Chamber Lights
BinaryActuator mb_light(52, true, 10000);            //5VDC - Motherboard Lights

// Put pointers to instantiated modules into the mod_ptr_array.
// Remember to update NMODS and ACTUATOR_OFFSET to be correct.
const uint8_t NMODS = 16;
const uint8_t ACTUATOR_OFFSET = 8;

Module *mod_ptr_array[] = {
   &dht22,               //0 - put Sensors at the head of the list
   &tsl2561,             //1
   &co2,                 //2
   &ph,                  //3
   &water_temp,          //4
   &ec,                  //5
   &shell_open,          //6
   &window_open,         //7
   &humidifier,          //8 - first Actuator in the list
   &grow_light,          //9
   &ac_3,                //10
   &air_heat,            //11
   &vent_fan,            //12
   &circ_fan,            //13
   &chamber_light,       //14
   &mb_light             //15
};

// Put the name of the modules in the mname_array data structure.
//
//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "air_temp_hum",
   "light_meter",
   "co2",
   "ph",
   "water_temp",
   "ec",
   "shell",
   "window",
   "humidifier",
   "grow_light",
   "ac_3",
   "air_heat",
   "vent_fan",
   "circ_fan",
   "chamber_light",
   "mb_light"
};
