#include "Arduino.h"
#include <openag_module.h>
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <test.h>
#include <types.h>
#include <pair.h>
#include <wire_w.h>
#include <atlas_w.h>
#include <openag_modules.h>
#include <src.h>
#include <onewire_w.h>

// Signatures of built-ins that are in this file.
int apply_error(int i); 
int water_temp(int args);
int config_open_ag_loop(int args);
int help(int args);

// TODO: Create native functions for:
// 1) Display the 64 bit addresses of all connected One Wire devices.
// 

// All the built in functions take one argument (an integer) which is assumed to point
// to a pair based list of function parameters.  The built in function is responsible
// for parsing the paramter list.

typedef int (*function_ptr)(int i);

// BTW: This does not work -> const function_ptr fp_array[] = PROGMEM {
// If the above declaration is used then i can't use
// return (*(fp_array[fi])) (args); to call the function. I can use
// return (*(fp_array[2])) (args); to call functions.
// 

const int NBIF = 16;  //Set to size of fname_array.

// This array holds points to all the built-in functions.
const function_ptr fp_array[] = {
   &apply_error,           //0 
   &water_temp,            //1
   &run_tests,             //2
   &i2c_begin,             //3
   &i2c_request_from,      //4
   &i2c_begin_trans,       //5
   &i2c_end_trans,         //6
   &i2c_write,             //7
   &i2c_send,              //8
   &i2c_read,              //9
   &i2c_cmd,               //10
   &atlas_show_ph,         //11
   &config_open_ag_loop,   //12
   &help,                  //13
   &i2c_help,              //14
   &one_wire_addr          //15
};

// This array holds the names of all the built in functions.
const char *fname_array[NBIF] = {
   "apply_error",
   "water_temp",
   "unit_tests",
   "i2c_begin",
   "i2c_request_from",
   "i2c_begin_trans",
   "i2c_end_trans",
   "i2c_write",
   "i2c_send",
   "i2c_read",
   "i2c_cmd",
   "atlas_show_ph",
   "set_oa_lp",
   "help",
   "i2c_help",
   "one_wire_addr"
};

// Look for a built in function that matches the name given.  If one is found then 
// return cons('B', the functions index);
//
int find_built_in_function(int function_name) {

   char *fn = get_str(function_name); 

   if (!fn){
      return 0;
   }
  
   // Look for an invocation of a built in function 
   for (int i = 1; i < NBIF; i++) {
      if (strcmp(fn, fname_array[i]) == 0) {
         return cons(make_char('B'), make_int(i));
      }
   }

   return 0;
}

int apply_built_in_function(int func, int args) {
   return (*(fp_array[get_int(cdr(func))])) (args);
}

// Built-in functions

int config_open_ag_loop(int args) {
Serial.println("got to config_open_ag_loop()");
return -1;
}

int apply_error(int i) {
   Serial.print(F("Apply Error.  Unknown built-in function call."));
   return 0;
}

//  Take a water  temperature reading.
int water_temp(int args) {

   ds18b20.update();

   Serial.print("DS18b20: ");
   Serial.println(ds18b20.get_temperature());

   return -1;
}
   


int help(int args) {

   if (args == nil) {  

      Serial.println(F("(help)              Prints this message."));
      Serial.println(F("(unit_tests)        Runs unit tests.  Helpful for testing that "));
      Serial.println(F("                    the Serial Monitor that is installed is a")); 
      Serial.println(F("                    stable version."));
      Serial.println(F("(i2c_help)          Print help for the I2C commands."));
   
      return -1;

   } else {

      Serial.println(F("Unknown help command."));
      return 0;
   }
}

void test_built_in_functions() {

   String tn = F("test_built_in_functions:");

   char test[] = "unit_tests";
   int test2 = find_built_in_function(make_str(test));

   assert_true(tn, is_pair(test2));
   assert_char_equals(tn, 'B', get_char(car(test2)));
   assert_int_equals(tn, 2, get_int(cdr(test2))); 

}
