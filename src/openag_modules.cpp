#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

// Put includes for all the system's modules here.
//
/* -
#include <openag_ds18b20.h>
#include <openag_mhz16.h>
#include <openag_atlas_ph.h>
#include <openag_atlas_ec.h>
*/

#include <dht22.h>

#include <openag_binary_sensor.h>
#include <openag_binary_actuator.h>
#include <openag_pwm_actuator.h>
#include <openag_doser_pump.h>
#include <openag_pulse_actuator.h>
#include <openag_tone_actuator.h>

// Internal function declarations
bool str2bool(String str);
bool checkModule(Module &module, String name);
bool beginModule(Module &module, String name);
void sendModuleStatus(Module &module, String name);

// Put instantiations of all the system's modules here.  
// Food Computer V1 -> see https://github.com/OpenAgInitiative/gro-hardware/blob/master/ElectronicsSchematicLarge.pdf
//
// Sensors
//
SensorDht22 dht22(A0);                               // air temperature and humidity
//
// Actuator Instances. Sorted by pin number.
//
BinaryActuator humidifier(9, true, 10000);    //AC port 1
BinaryActuator grow_light(8, true, 10000);    //AC port 2
BinaryActuator ac_3(7, true, 10000);          //AC port 3
BinaryActuator air_heat(6, true, 10000);      //AC port 4

// Put pointers to instantiated modules into the mod_ptr_array.
// Remember to update NMODS and ACTUATOR_OFFSET to be correct.
const uint8_t NMODS = 5;
const uint8_t ACTUATOR_OFFSET = 1;

Module *mod_ptr_array[] = {
   &dht22,               //0 - put Sensors at the head of the list
   &humidifier,          //1 - first Actuator in the list
   &grow_light,          //2
   &ac_3,                //3
   &air_heat             //4
};

// Put the name of the modules in the mname_array data structure.
//
//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "air_temp_hum",
   "humidifier",
   "grow_light",
   "ac_3",
   "air_heat"
};

// Put lines for all the FC V2 actuators below. If you modify this function
// then you must make sure the calling code in src.cpp is updated to send 
// the correct number of actuator messages in the correct order, furthermore you can
// no longer use the OpenAg ROS code that communicates with the firmware without
// modifying it to account for the new sensor and actuator set.
//
// The OpenAg FC V2 control loop calls this method in order to send actuator
// commands.  
//
// TBD - add help text for the set_actuator command.
// 
void set_actuators(String splitMessages[]) {

  humidifier.set_cmd(str2bool(splitMessages[1]));                 // BinaryActuator bool
  grow_light.set_cmd(str2bool(splitMessages[2]));                 // BinaryActuator bool
  ac_3.set_cmd(str2bool(splitMessages[3]));                       // BinaryActuator bool
  air_heat.set_cmd(str2bool(splitMessages[4]));                   // BinaryActuator bool

}

// Put checkModule(...) calls in this function for each Actuator.
//
// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Actuators.
//
bool checkActuatorLoop() {

  bool allActuatorSuccess = true;

  allActuatorSuccess = checkModule(humidifier, "Humidifier") && allActuatorSuccess;
  allActuatorSuccess = checkModule(grow_light, "Grow Light") && allActuatorSuccess;
  allActuatorSuccess = checkModule(ac_3,  "AC #3") && allActuatorSuccess;
  allActuatorSuccess = checkModule(air_heat, "Air Heater") && allActuatorSuccess;

  return allActuatorSuccess;
}

// Put checkModule(...) calls in this function for each Sensor.
//
// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Sensors.
//
bool checkSensorLoop() {

  bool allSensorSuccess = true;

  // Run Update on all sensors
  allSensorSuccess = checkModule(dht22, "dht22") && allSensorSuccess;

  return allSensorSuccess;
}

// Put code here for each Sensor.  This routine is invoked (via the Arduino serial port and src.cpp) by the 
// OpenAG FC V2 Controller (.e.g. Raspberry PI Model 3B) ROS Node and thus changes here will make the code 
// incompatible with the current Controller code.
// 
// Prints the data in CSV format via serial.
// Columns: status,hum,temp,co2,water_temperature,water_low,water_high,ph,ec
//
void sensorLoop(){

  Serial.print(OK);                                             Serial.print(',');
  Serial.print(dht22.get_air_humidity());                       Serial.print(',');
  Serial.print(dht22.get_air_temperature());
  Serial.print('\n');

  // https://www.arduino.cc/en/serial/flush
  // Wait until done writing.
  Serial.flush();
}
// Put a call to beginModule here for each Module.
//
void beginModules() {

  // Begin Sensors
  beginModule(dht22, "dht22");

  // Begin Actuators
  beginModule(humidifier, "Humididier");
  beginModule(grow_light, "Grow Light");
  beginModule(ac_3, "AC #3");
  beginModule(air_heat, "Air Heater");
}

// Runs the update loop - The update method is what causes Sensors to take new readings.  The update
// method causes actuators to invoke their built-in control loop and change the actuator outputs as 
// per the control loop's logic.
//
void updateLoop(){

   for (int i=0; i < NMODS; i++) {
      (*(mod_ptr_array[i])).update();
   }
}

// You don't need to change anything beneath this line in order to add/remove/change sensors or actuators.
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

//If a the module does not have OK status then print it's status.
//
bool checkModule(Module &module, String name){

  bool status = module.status_code == OK;

  if(!status) {
    sendModuleStatus(module, name);
  }
  return status;
}

// Look for a module class that matches the name given.  If one is found then 
// return the class.
//
Module *find_module(int module_name) {

   char *mn = get_str(module_name); 

   if (!mn){
      return 0;
   }
  
   // Look for the module 
   for (int i = 0; i < NMODS; i++) {
      if (strcmp(mn, mname_array[i]) == 0) {
         return mod_ptr_array[i];
      }
   }

   return 0;
}

void openag_modules_show_mods() {

   Serial.println(F("The following modules are installed on this system:"));

   for (int i=0; i < NMODS; i++) {

      if (i == 0) {
         Serial.println(F("SENSORS:"));
      }

      if (i == ACTUATOR_OFFSET) {
         Serial.println(F(""));
         Serial.println(F("ACTUATORS:"));
      }

      Serial.println(mname_array[i]);
   }

}

int openag_modules_show_mod_status(int status_code) {

   if (status_code == OK) {
      Serial.println(F("Module response: OK"));
      return -1;      
   }
   if (status_code == WARN) {
      Serial.println(F("Module response: WARN"));
      return -1;      
   }
   if (status_code == ERROR) {
      Serial.println(F("Module response: ERROR"));
      return -1;      
   }

   Serial.println(F("Error: Unknown module status code."));
   return 0;
}

bool str2bool(String str){
  str.toLowerCase();
  return str.startsWith("true");
}
