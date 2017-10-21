#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

#include <openag_module.h>
#include <openag_am2315.h>
#include <openag_ds18b20.h>

// Create an instance of each Module that will run on this food computer.
// Note: Add/Subtract from this list to customize your food computer
//       to contain different sensors and actuators.
//
Am2315 am2315_1;
Ds18b20 ds18b20_1(5);

//local function signatures.
bool beginModule(Module &module, String name);
void sendModuleStatus(Module &module, String name);

// Setup wrapper functions and function name table so that the serial
// monitor can call functions that access Module functions.
//
//Install module wrapper functions

// All the built in functions take one argument (an integer) which is assumed to point
// to a pair based list of function parameters.  The built in function is responsible
// for parsing the paramter list.

typedef int (*function_ptr)(int i);

const int NMODS = 1;

int am2315_1_w(int args);

//This array holds the pointers to all the Module functions.
const function_ptr mod_array[] = {
   &am2315_1_w
};

//This array holds the name of all the module functions.
const char *mname_array[1] = {
   "am2315_1"
};

// Look for a module function that matches the name given.  If one is found then 
// return cons('M', the functions index);
//
int find_module_function(int function_name) {

   char *fn = get_str(function_name); 

   if (!fn){
      return 0;
   }
  
   // Look for an invocation of a module function
   for (int i = 0; i < NMODS; i++) {
      if (strcmp(fn, mname_array[i]) == 0) {
         return cons(make_char('M'), get_int(i));
      }
   }

   return 0;
}

int apply_module_function(int func, int args) { 
   return (*(mod_array[get_int(cdr(func))])) (args);
}

// This section contains the module functions called from the serial monitor
//

int am2315_1_w(int args) {
Serial.println("got to am2315");
Serial.println(get_char(caar(args)));
return -1;

}


// This section contains the Module functions called from the Arduino setup() 
// and loop() functions.
//
boolean beginModules() {
  
  boolean success = true;

  success = success && beginModule(am2315_1, "AM2315 #1");
  success = success && beginModule(ds18b20_1, "DS18B20 #1");

  return success;
}

bool beginModule(Module &module, String name){
  bool status = module.begin() == OK;
  if(!status){
    sendModuleStatus(module, name);
  }
  return status;
}

void sendModuleStatus(Module &module, String name){
  Serial.print(module.status_level); Serial.print(',');
  Serial.print(name);  Serial.print(',');
  Serial.print(module.status_code);  Serial.print(',');
  Serial.print(module.status_msg);   Serial.print('\n');
  Serial.flush();
}

/*
  // Begin sensors
  beginModule(am2315_1, "AM2315 #1");
  beginModule(mhz16_1, "MHZ16 #1");
  beginModule(ds18b20_1, "DS18B20 #1");
  beginModule(water_level_sensor_low_1, "Water Level Low sensor");
  beginModule(water_level_sensor_high_1, "Water Level High sensor");
  beginModule(atlas_ph_1, "Atlas pH #1");
  beginModule(atlas_ec_1, "Atlas EC #1");

  // Begin Actuators
  beginModule(pump_1_nutrient_a_1, "Pump 1, Nutrient A");
  beginModule(pump_2_nutrient_b_1, "Pump 2, Nutrient B");
  beginModule(pump_3_ph_up_1, "Pump 3, pH Up");
  beginModule(pump_4_ph_down_1, "Pump 4, pH Down");
  beginModule(pump_5_water_1, "Pump 5, Water");
  beginModule(chiller_fan_1, "Chiller Fan");
  beginModule(chiller_pump_1, "Chiller Pump");
  beginModule(heater_core_2_1, "Heater core #2");
  beginModule(air_flush_1, "Air Flush");
  beginModule(water_aeration_pump_1, "Water Aeration Pump");
  beginModule(water_circulation_pump_1, "Water Circulation Pump");
  beginModule(chamber_fan_1, "Chamber Circulation Fan");
  beginModule(led_blue_1, "LED Blue");
  beginModule(led_white_1, "LED White");
  beginModule(led_red_1, "LED Red");
  beginModule(heater_core_1_1, "Heater Core #1");
  beginModule(chiller_compressor_1, "Chiller Compressor #1");
*/
