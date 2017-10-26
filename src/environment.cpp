#include "Arduino.h"
#include <pair.h>
#include <types.h>
#include <built_in_functions.h>
#include <openag_modules.h>

// Look for a built in function that matches the name given.  If one is found then 
// return cons('B' or 'M', the functions index);
//
int find_function(int function_name) {

   int func_ptr = 0;

   char *fn = get_str(function_name); 

   if (!fn){
      return 0;
   }

   func_ptr = find_built_in_function(function_name);
   if (is_pair(func_ptr)) { return func_ptr; }

   func_ptr = find_module_function(function_name);
   if (is_pair(func_ptr)) { return func_ptr; }

   return 0;
}

int apply_function(int func, int args) {

   // Check for a built-in function call.
   if (get_char(car(func)) == 'B') {
      return apply_built_in_function(func, args);
      //return (*(fp_array[get_int(cdr(func))])) (args);
   }

   // Check for a module function call.
   if (get_char(car(func)) == 'M') {
      return apply_module_function(func, args);
      //return (*(mod_array[get_int(cdr(func))])) (args);
   }

   Serial.println(F("built_in_functions.cpp: apply_built_in_function: Uknown function."));
   return 0;
  
}

