// Put includes for all the system's modules here.
//
#include <openag_atlas_ph.h>
#include <openag_atlas_ec.h>
#include <openag_ds18b20.h>
#include <openag_binary_actuator.h>

// Put instantiations of all the system's modules here.  
//
// Sensors
//
AtlasPh atlas_ph_1(99);                            // PH, I2C, Addr = 99
AtlasEc atlas_ec_1(100);                           // EC, I2C, Addr = 100
Ds18b20 water_temp(5);                             // Water temperature 1-Wire probe on Pin 5.
//
// Actuator Instances. Sorted by pin number.
//
BinaryActuator ac_120v(10, true, 10000);
BinaryActuator ph_down_pump(7, true, 10000);
BinaryActuator ph_up_pump(8, true, 10000);
BinaryActuator nutrient_pump(9, true, 10000);
BinaryActuator water_pump(6, true, 10000);
BinaryActuator comm_led(4, true, 10000);

// Put pointers to instantiated modules into the mod_ptr_array.
// Remember to update NMODS and ACTUATOR_OFFSET to be correct.
const uint8_t NMODS = 9;
const uint8_t ACTUATOR_OFFSET = 3;

Module *mod_ptr_array[] = {
   &atlas_ph_1,                 //0 - Put sensors at the head of the list.
   &atlas_ec_1,                 //1
   &water_temp,                 //2
   &ac_120v,                    //3 - First actuator
   &ph_down_pump,               //4
   &ph_up_pump,                 //5
   &nutrient_pump,              //6
   &water_pump,                 //7
   &comm_led                    //8
};

// Put the name of the modules in the mname_array data structure.
//
//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "ph",
   "ec",
   "water_temp",
   "ac_120v",
   "ph_down_pump",
   "ph_up_pump",
   "nutrient_pump",
   "water_pump",
   "comm_led"
};
