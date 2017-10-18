#include "Arduino.h"
#include <openag_module.h>
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <test.h>
#include <types.h>
#include <pair.h>
#include <wire_w.h>

// Signatures of built-ins that are in this file.
int apply_error(int i); 
int air_temp(int args);


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

const int NBIF = 10;  //Set to size of fname_array.

// This array holds points to all the built-in functions.
const function_ptr fp_array[] = {
   &apply_error,           //0 
   &air_temp,              //1
   &run_tests,             //2
   &i2c_begin,             //3
   &i2c_request_from,      //4
   &i2c_begin_trans,       //5
   &i2c_end_trans,         //6
   &i2c_write,             //7
   &i2c_send,              //8
   &i2c_read               //9
};

// This array holds the names of all the built in functions.
const char *fname_array[NBIF] = {
   "apply_error",
   "air_temp",
   "unit_tests",
   "i2c_begin",
   "i2c_request_from",
   "i2c_begin_trans",
   "i2c_end_trans",
   "i2c_write",
   "i2c_send",
   "i2c_read"
};

//Install Modules
int am2315_1_w(int args);

const int NMODS = 1;

//This array holds the pointers to all the Module functions.
const function_ptr mod_array[] = {
   &am2315_1_w
};

//This array holds the name of all the module functions.
const char *mname_array[1] = {
   "am2315_1"
};

// Look for a built in function that matches the name given.  If one is found then 
// return cons('B' or 'M', the functions index);
//
int find_built_in_function(int function_name) {

   //int function_index = 0;

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

   // Look for an invocation of a module function
   for (int i = 0; i < NMODS; i++) {
      if (strcmp(fn, mname_array[i]) == 0) {
         return cons(make_char('M'), get_int(i));
      }
   }

   return 0;
}

int apply_built_in_function(int func, int args) {

   // Check for a built-in function call.
   if (get_char(car(func)) == 'B') {
   //return (*(fp_array[fi])) (args);
      return (*(fp_array[get_int(cdr(func))])) (args);
   }

   // Check for a module function call.
   if (get_char(car(func)) == 'M') {
      return (*(mod_array[get_int(cdr(func))])) (args);
   }

   Serial.println(F("built_in_functions.cpp: apply_built_in_function: Uknown function."));
   return 0;
  
}

// Built-in functions

int apply_error(int i) {
   Serial.print(F("Apply Error.  Unknown built-in function call."));
   return 0;
}

/*
int test_func_args (int i) {
   return i;
}
*/

int am2315_1_w(int args) {

Serial.println(get_str(car(args)));
return 0;

}
   
//  Take an air temperature reading.
int air_temp(int args) {

   extern Ds18b20 ds18b20_1;
   ds18b20_1.update();

   Serial.print("DS18b20: ");
   Serial.println(ds18b20_1.get_temperature());

   return 0;
}

void test_built_in_functions() {

   String tn = F("test_built_in_functions:");

   char test[] = "unit_tests";
   int test2 = find_built_in_function(make_str(test));

   assert_true(tn, is_pair(test2));
   assert_char_equals(tn, 'B', get_char(car(test2)));
   assert_int_equals(tn, 2, get_int(cdr(test2))); 

}
