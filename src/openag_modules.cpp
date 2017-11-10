#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

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

#include <src.h>

//local function signatures.
void sendModuleStatus(Module &module, String name);
int send_begin_cmd(int args);
int send_update_cmd(int args);
int send_set_cmd(int args);
int show_mods(int args);
int begin_mods(int args);
int check_actuator_loop(int args);
int check_sensor_loop(int args);
int read_sensors(int args);
int fc(int args);

// Specify the installed module classes and names here.
//
const uint8_t NMODS = 24;
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
   &led_blue_1,                 //10
   &led_white_1,                //11
   &led_red_1,                  //12
   &chiller_compressor_1,       //13
   &chiller_pump_1,             //14
   &heater_core_2_1,            //15
   &water_aeration_pump_1,      //16
   &water_circulation_pump_1,   //17
   &heater_core_1_1,            //18
   &pump_1_nutrient_a_1,        //19
   &pump_2_nutrient_b_1,        //20
   &pump_5_water_1,             //21
   &pump_3_ph_up_1,             //22
   &pump_4_ph_down_1            //23
};

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
   "blue_led",
   "white_led",
   "red_led",
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
   "ph_down"
};

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

// All the built in functions take one argument (an integer) which is assumed to point
// to a pair based list of function parameters.  The built in function is responsible
// for parsing the paramter list.

typedef int (*function_ptr)(int i);

const uint8_t NMOD_FUNCS = 9;

// Module function signatures
//
int oa_mod_cmd(int args);

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
const char *mfname_array[NMODS] = {
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
   for (int i = 0; i < NMODS; i++) {
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
   }

   Serial.println(F("Error: Unknown module response code."));
   
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

   Module *mod = find_module(cdar(args));

   if (mod) {return show_open_ag_module_status(mod->cmd(args));}

   Serial.println(F("Error in openag_modules:oa_mod_cmd_2: Unknown module name."));
   return 0;
}

int show_mods(int args) {

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
      Serial.println(F("                          2) Command: One of 'begin, 'update, 'read, or 'set"));
      Serial.println(F("                          3) Set commands take an argument.  Example 0"));
      Serial.println(F("                          Example Command: (oa_mod_cmd 'blue_led 'set 0.8)")); 
      Serial.println(F(""));
      Serial.println(F("(openag_help)             Prints this message."));
      Serial.println(F(""));
      Serial.println(F("(read_sensors)            Show sensor data as a comma seperated list. The sensor data is returned"));
      Serial.println(F("                          as status,hum,temp,co2,water_temperature,water_low,water_high,ph,ec."));
      Serial.println(F(""));
      Serial.println(F("(show_mods)               Lists all the Sensors and Actuators installed on this system."));

      return -1;

   } else {

      Serial.println(F("Unknown openag help command."));
      return 0;
   }
}
