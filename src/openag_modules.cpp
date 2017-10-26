#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

#include <openag_module.h>
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <src.h>

// Create an instance of each Module that will run on this food computer.
// Note: Add/Subtract from this list to customize your food computer
//       to contain different sensors and actuators.
//
#include <openag_modules_instances.h>

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

const int NMODS = 2;

int am2315_1_w(int args);
int ds18b20_w(int args);

//This array holds the pointers to all the Module functions.
const function_ptr mod_array[] = {
   &am2315_1_w,
   &ds18b20_w
};

//This array holds the name of all the module functions.
const char *mname_array[NMODS] = {
   "am2315_1",
   "ds18b20"
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
         return cons(make_char('M'), make_int(i));
      }
   }

   return 0;
}

int apply_module_function(int func, int args) { 
   return (*(mod_array[get_int(cdr(func))])) (args);
}

// This section contains the module functions called from the serial monitor
//

char *show_open_ag_module_status(uint8 status_code) {

   if (status_code == OK) {
      Serial.

}

int am2315_1_w(int args) {

   char begin[] = "begin";
   char update[] = "update";
   char temp_and_hum[] = "temp_and_hum";

   if (get_char(caar(args)) == 'S' && strcmp(begin, get_str(cdar(args))) == 0) {
      Serial.print("am2315 Begin command status:");
      Serial.println(show_open_ag_module_status(am2315_1.begin()));
      return -1;
   }

   if (get_char(caar(args)) == 'S' && strcmp(update, get_str(cdar(args))) == 0) {
      am2315_1.update();
      return -1;
   }

   if (get_char(caar(args)) == 'S' && strcmp(temp_and_hum, get_str(cdar(args))) == 0) {
      Serial.print("am2315_1: Temp=");
      Serial.print(am2315_1.get_air_temperature());      
      Serial.print(", Humidity=");
      Serial.println(am2315_1.get_air_humidity());
      return -1;
   }

   Serial.println(F("Error: Unkown am2315 command."));
   return 0;
}

int ds18b20_w(int args) {

   char begin[] = "begin";
   char update[] = "update";
   char temp[] = "temp";

   if (get_char(caar(args)) == 'S' && strcmp(begin, get_str(cdar(args))) == 0) {
      ds18b20.begin();
      return -1;
   }

   if (get_char(caar(args)) == 'S' && strcmp(update, get_str(cdar(args))) == 0) {
      ds18b20.update();
      return -1;
   }

   if (get_char(caar(args)) == 'S' && strcmp(temp, get_str(cdar(args))) == 0) {
      Serial.print("DS18b20: ");
      Serial.println(ds18b20.get_temperature());
      return -1;
   }

   Serial.println(F("Error: Unkown ds18b20 command."));
   return 0;
}

