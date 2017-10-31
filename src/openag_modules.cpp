#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <openag_binary_actuator.h>
#include <openag_pwm_actuator.h>
#include <openag_mhz16.h>
#include <src.h>

//local function signatures.
void sendModuleStatus(Module &module, String name);
int send_begin_cmd(int args);
int send_update_cmd(int args);
int send_set_cmd(int args);

// Specify the installed module classes and names here.
//
const uint8_t NMODS = 9;

Module *mod_ptr_array[] = {
   &am2315_1,          //0
   &ds18b20_1,         //1
   &air_flush_1,       //2
   &chamber_fan_1,     //3
   &chiller_fan_1,     //4
   &led_blue_1,        //5
   &led_white_1,       //6
   &led_red_1,         //7
   &mhz16_1            //8
};

//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "am2315",
   "ds18b20",
   "air_flush",
   "chamber_fan",
   "chiller_fan",
   "blue_led",
   "white_led",
   "red_led",
   "co2"
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

const uint8_t NMOD_FUNCS = 1;

// Module function signatures
//
int oa_mod_cmd(int args);

//This array holds the pointers to all the Module functions.
const function_ptr mod_array[NMOD_FUNCS] = {
   &oa_mod_cmd,
};

//This array holds the name of all the module functions.
const char *mfname_array[NMODS] = {
   "oa_mod_cmd"
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

   Serial.println("in oa_mod_cmd_2"); 

   Module *mod = find_module(cdar(args));

   if (mod) {return show_open_ag_module_status(mod->cmd(args));}

   Serial.println(F("Error in openag_modules:oa_mod_cmd_2: Unknown module name."));
   return 0;
}

int openag_help(int args) {

   return 0;

}
