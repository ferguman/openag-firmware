#include "Arduino.h"
#include <pair.h>
#include <types.h>
#include <openag_module.h>  // This .h file defines the base class for all modules. See 
                            // ./lib/openag_firmware_module/openag_module.h for the details.

#include <openag_modules.h> //This .h file contains the openag specific monitor commands such as beginModules(). 

/* - 
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
*/

#include <src.h>

// This file contains the Open Ag Module related command implementation code for commands sent via the Serial 
// Monitor to the Arudino.

//local function signatures.
// void sendModuleStatus(Module &module, String name);
// int send_begin_cmd(int args);
// int send_update_cmd(int args);
// int send_set_cmd(int args);
int oa_mod_cmd(int args);
int openag_help(int args);
int show_mods(int args);
int begin_mods(int args);
int check_actuator_loop(int args);
int check_sensor_loop(int args);
int read_sensors(int args);
int fc(int args);

// All the built in functions take one argument (an integer) which is assumed to point
// to a pair based list of function parameters.  The built in function is responsible
// for parsing the parameter list.

typedef int (*function_ptr)(int i);

const uint8_t NMOD_FUNCS = 9;

//This array holds the pointers to all the Module functions.
const function_ptr mod_array[NMOD_FUNCS] = {
   &oa_mod_cmd,            //0
   &oa_mod_cmd,            //1
   &openag_help,           //2
   &show_mods,             //3
   &begin_mods,            //4
   &check_actuator_loop,   //5
   &check_sensor_loop,     //6
   &read_sensors,          //7
   &fc                     //8
};

//This array holds the name of all the module functions.
const char *mfname_array[NMOD_FUNCS] = {
   "oa_mod_cmd",
   "c",
   "openag_help",
   "show_mods",
   "begin_mods",
   "check_actuators",
   "check_sensors",
   "read_sensors",
   "fc"
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
   for (int i = 0; i < NMOD_FUNCS; i++) {
      if (strcmp(fn, mfname_array[i]) == 0) {
         return cons(make_char('M'), make_int(i));
      }
   }

   return 0;
}

int apply_module_function(int func, int args) { 
   return (*(mod_array[get_int(cdr(func))])) (args);
}

// This section contains the module functions called from the serial monitor

int show_open_ag_module_status(int arg) {

   if(is_int(arg)) { 

      int status_code = get_int(arg);

      return openag_modules_show_mod_status(status_code);
      return 0;
   }
  
   return 0;
}

boolean is_cmd(int args, char *cmd) {

   if (get_char(caadr(args)) == 'S' && strcmp(cmd, get_str(cdadr(args))) == 0) {
      return true;
   } else {
      return false;
   }
}

int oa_mod_cmd(int args) {

   Module *mod;
   mod = find_module(cdar(args));

   if (mod) {return show_open_ag_module_status(mod->cmd(args));}

   Serial.println(F("Error in openag_modules:oa_mod_cmd: Unknown module name."));
   return 0;
}

int show_mods(int args) {

   openag_modules_show_mods();

   return -1;
}

int begin_mods(int args) {

   beginModules();
   return -1;

}

int check_actuator_loop(int args) {

   boolean all_good = checkActuatorLoop(); 

   if (all_good) {
      Serial.println(F("All actuator Modules are OK."));
   } else {
      Serial.println(F("One or more actuator Modules are in an error state."));
   }

   return -1;
}

int check_sensor_loop(int args) {

   boolean all_good = checkSensorLoop();

   if (all_good) {
      Serial.println(F("All sensor Modules are OK."));
   } else {
      Serial.println(F("One or more sensor Modules are in an error state."));
   }

   return -1;
}

int read_sensors(int args) {

   sensorLoop();

   return -1;
}

int fc(int args) {

   char on[] = "on";
   char off[] = "off";
   char read[] = "read";

   if (strcmp(on, get_str(cdar(args))) == 0) {
      fc_loop_on = true;
      return -1;
   }

   if (strcmp(off, get_str(cdar(args))) == 0) {
      fc_loop_on = false;
      return -1;
   }

   if (strcmp(read, get_str(cdar(args))) == 0) {

      Serial.print(F("This food computer microcontroller control loop is currently "));
      if (fc_loop_on) {
         Serial.println(F("ON."));
      } else {
         Serial.println(F("OFF."));
      }
      return -1;
   }

   Serial.println(F("Unknown parameter."));

   return 0;
}

int openag_help(int args) {

   if (args == nil) {  

      Serial.println(F("(begin_mods)              Initialize all installed Openag Modules."));
      Serial.println(F(""));
      Serial.println(F("(c)                       Short form of the (oa_mod_cmd) command"));
      Serial.println(F("                          Example Command: (c 'blue_led 'set 0.8)")); 
      Serial.println(F(""));
      Serial.println(F("(check_actuators)         Check all the installed Openag Modules. Print the status of"));
      Serial.println(F("                          all sensors that are not in the OK state."));
      Serial.println(F(""));
      Serial.println(F("(check_sensors)           Check all the installed Openag sensors.  Print the status of"));
      Serial.println(F("                          all sensors that are not in the OK state."));
      Serial.println(F(""));
      Serial.println(F("(fc)                      Turn the microcontroller's control loop on or off. Accepts 1 argument."));
      Serial.println(F("                          1) Cmd: One of 'on, 'off, or 'read."));
      Serial.println(F("                          Example Command: (fc 'on)")); 
      Serial.println(F(""));
      Serial.println(F("(oa_mod_cmd)              Runs an Open Ag Module command. Accepts up to 3 arguments.")); 
      Serial.println(F("                          1) Module name, Type (show_mods) to see the list. Example: 'ph"));
      Serial.println(F("                          2) Command: One of 'begin, 'update, 'read, 'state' or 'set"));
      Serial.println(F("                          3) Set commands take an argument.  Example 0"));
      Serial.println(F("                          Example Command: (oa_mod_cmd 'blue_led 'set 0.8)")); 
      Serial.println(F(""));
      Serial.println(F("                          Some modules support additional commands. Enter (c 'mod_name 'help)"));
      Serial.println(F("                          for more information."));
      Serial.println(F(""));
      Serial.println(F("(openag_help)             Prints this message."));
      Serial.println(F(""));
      Serial.println(F("(read_sensors)            Show sensor data as a comma seperated list. The sensor data is returned"));
      Serial.println(F("                          as status,hum,temp,co2,water_temperature,water_low,water_high,ph,ec."));
      Serial.println(F(""));
      Serial.println(F("(show_mods)               Lists the names of the Sensor and Actuator Modules installed on this system."));

      return -1;

   } else {

      Serial.println(F("Unknown openag help command."));
      return 0;
   }
}
