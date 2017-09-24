#include "Arduino.h"
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

// Creae native functions for:
// 1) Display the 64 bit addresses of all connected One Wire devices.
// 

// Declare the type to be a pointer to a function that returns a void with an
// unspecified number of arguments. Then create an array to hold the function
// pointers.
//
typedef int (*function_ptr)(int i);

const function_ptr fp_array[] PROGMEM = {
   &apply_error, 
   &test_func_args,
   &run_tests};

const String fname_array[] PROGMEM = {
   "apply_error",
   "function2",
   "tests"};

const int NBIF = 3;  //Set to size of fname_array.

int find_built_in_function(int function_name) {

   int function_index = 0;

   String fn = get_str(function_name); 
   if (fn == "") {
      return 0;
   }
   
   for (int i = 1; i < NBIF; i++) {
      if (fn  == fname_array[i]) {
         function_index = i;
      }
   }

   return function_index;
}

int call_built_in_function(int func_index, int func_args) {

   if (func_index == 2) { return run_tests(func_args); };

   Serial.println("Error: Unknown built-in function.");
   return 0;
}

int apply(int op, int args, int env) { 

   int fi = find_built_in_function(op) + 0;

   if (fi) {
       Serial.print("fi = "); Serial.println(fi);

       return call_built_in_function(fi, args);

       // could not get this stuff to work with fi as the array pointer. it works
       // if the array pointer is hard coded as in the example below.
       //return (*(fp_array[2])) (args);
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
