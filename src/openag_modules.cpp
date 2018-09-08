#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

// You must define an environment variable to tell the pre-compiler which module configuration file to use.
// One way to do this is to put a build_flags section in your platformio.ini file as per the following lines:
// build_flags =
//    -D OA_FC_V1
// Currently the configuration files are placed in the src/ directory.
//
#ifdef OA_FC_V1
#include <configuration_fcv1.h> 
#else
#error "You must define a pre-compiler flag. See openag_modules.cpp for details."
#endif

// Internal function declarations
bool checkModule(Module &module, String name);
bool beginModule(Module &module, String name);
void sendModuleStatus(Module &module, String name);

uint8_t get_command_length() {
   // commands are started by the string 0, hence the 1 in the equation below.
   return (1 + NMODS - ACTUATOR_OFFSET);
}

// The OpenAg FC V2 control loop calls this method in order to send actuator
// commands.  
//
// TBD - add help text for the set_actuator command.
// 
void set_actuators(String splitMessages[]) {

    for (int i=0; i<(NMODS - ACTUATOR_OFFSET); i++) {
        (*(mod_ptr_array[i + ACTUATOR_OFFSET])).set_cmd(splitMessages[i+1].c_str());
    }
}

// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Actuators.
//
bool checkActuatorLoop() {

  bool allActuatorSuccess = true;

   for (int i=ACTUATOR_OFFSET; i < NMODS; i++) {
      allActuatorSuccess = checkModule(*(mod_ptr_array[i]), mname_array[i]) && allActuatorSuccess;
   }

  return allActuatorSuccess;
}

//
// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Sensors.
//
bool checkSensorLoop() {

  bool allSensorSuccess = true;

  for (int i=0; i < ACTUATOR_OFFSET; i++) {
      allSensorSuccess = checkModule(*(mod_ptr_array[i]), mname_array[i]) && allSensorSuccess;
   }

  return allSensorSuccess;
}

// This routine is invoked (via the Arduino serial port and src.cpp) by the 
// OpenAG FC V2 Controller (.e.g. Raspberry PI Model 3B) ROS Node and thus changes here will make the code 
// incompatible with the current Controller code.
// 
// Prints the data in CSV format via serial.
//
void sensorLoop(){

  //TBD: Need to put the actual dynamic status in, right?
  Serial.print(OK);

  for (int i=0; i < ACTUATOR_OFFSET; i++) {

      Serial.print(',');
      (*(mod_ptr_array[i])).print_readings_as_csv();
  }

  Serial.println();

  // https://www.arduino.cc/en/serial/flush
  // Wait until done writing.
  Serial.flush();
}

void beginModules() {

  for (int i=0; i < NMODS; i++) {
      beginModule(*(mod_ptr_array[i]), mname_array[i]);
  }
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
  Serial.println(module.status_msg);   //-   Serial.print('\n');
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
