#include "Arduino.h"
#include <openag_module.h>
#include <openag_ds18b20.h>
#include <test.h>
#include <types.h>
#include <pair.h>
#include <apply.h>

int apply_error(int i) {
   Serial.print(F("Apply Error.  Unknown built-in function call."));
   return 0;
}

int test_func_args (int i) {
   return i;
}
   
//  Take an air temperature reading.
int air_temp(int args) {

   extern Ds18b20 ds18b20_1;
   ds18b20_1.update();

   Serial.print("DS18b20: ");
   Serial.println(ds18b20_1.get_temperature());

   return 0;
}

// Creae native functions for:
// 1) Display the 64 bit addresses of all connected One Wire devices.
// 

// Declare the type to be a pointer to a function that returns a void with an
// unspecified number of arguments. Then create an array to hold the function
// pointers.
//
typedef int (*function_ptr)(int i);

// This does not work -> const function_ptr fp_array[] = PROGMEM {
// If the above declaration is used then i can't use
// return (*(fp_array[fi])) (args); to call the function. I can use
// return (*(fp_array[2])) (args); to call functions.
// 
const function_ptr fp_array[] = {
   &apply_error, 
   &air_temp,
   &run_tests};

const char *fname_array[3] = {
   "apply_error",
   "air_temp",
   "unit_tests"};

const int NBIF = 3;  //Set to size of fname_array.

int find_built_in_function(int function_name) {

   int function_index = 0;

   char *fn = get_str(function_name); 
   Serial.print("function: "); Serial.println(*fn);
   if (!fn){
      return 0;
   }
   
   for (int i = 1; i < NBIF; i++) {
      //if (fn  == fname_array[i]) {
      if (strcmp(fn, fname_array[i]) == 0) {
         function_index = i;
      }
   }

   return function_index;
}

int apply(int op, int args, int env) { 

Serial.println("in apply");

   int fi = find_built_in_function(op) + 0;

   if (fi) {
       return (*(fp_array[fi])) (args);
   } 

   return 0;

   //return (*(fp_array[0]))((*(fp_array[1]))(args));
}

void test_apply() {

   //int test1 = cons(make_str("foobar"), nil);

   //Pass (foobar) to parse.
   //int test2 = parse(cons(cons(make_char('('),nil), cons(cons(make_char('S'), make_str("foobar")), cons(cons(make_char(')'), nil), nil))));
   //assert_str_equals(F("parse.cpp"), F("foobar"), get_str(car(test2)));

}
