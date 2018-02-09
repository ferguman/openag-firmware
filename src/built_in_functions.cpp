#include "Arduino.h"
#include <test.h>
#include <types.h>
#include <pair.h>
#include <interpret.h>

#include <wire_w.h>
#include <atlas_w.h>
#include <onewire_w.h>
#include <openag_module.h>
#include <openag_modules.h>

// Signatures of built-ins that are in this file.
int apply_error(int i); 
int config_open_ag_loop(int args);
int help(int args);
int iter(int args);
int invoke_delay(int args);

// All the built in functions take one argument (an integer) which is assumed to point
// to a pair based list of function parameters.  The built in function is responsible
// for parsing the paramter list.

typedef int (*function_ptr)(int i);

// BTW: This does not work -> const function_ptr fp_array[] = PROGMEM {
// If the above declaration is used then i can't use
// return (*(fp_array[fi])) (args); to call the function. I can use
// return (*(fp_array[2])) (args); to call functions.
// 

const int NBIF = 17;  //Set to size of fname_array.

// This array holds points to all the built-in functions.
const function_ptr fp_array[] = {
   &apply_error,           //0 
   &run_tests,             //1
   &i2c_begin,             //2
   &i2c_request_from,      //3
   &i2c_begin_trans,       //4
   &i2c_end_trans,         //5
   &i2c_write,             //6
   &i2c_send,              //7
   &i2c_read,              //8
   &i2c_cmd,               //9
   &atlas_show_ph,         //10
   &config_open_ag_loop,   //11
   &help,                  //12
   &i2c_help,              //13
   &one_wire_addr,         //14
   &iter,                  //15
   &invoke_delay           //16
};

// This array holds the names of all the built in functions.
const char *fname_array[NBIF] = {
   "apply_error",
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
   "one_wire_addr",
   "iter",
   "delay"
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

// Args -> Number of times to iterate, thing to iterate
//
int iter(int args) {

   if (get_int(car(args)) > 30) {
      Serial.println(F("Max. number of iterations (30) exceeded."));
      return 0;
   }

   if (get_int(car(args)) <= 1) {
      interpret(cadr(args));
      return -1;
   }

   // Iterate the operation argument but don't save it's return;
   interpret(cadr(args));

   // Decrement the interation counter and recurse.
   int i = get_int(car(args)) - 1;
   set_car(args, make_int(i));

   return iter(args);
}

int help(int args) {

   if (args == nil) {  

      Serial.println(F("(delay)             Delay for the specified number of milliseconds. Accepts 1 argument."));
      Serial.println(F("                    1) Number of milli-seconds to delay."));
      Serial.println(F("(help)              Prints this message."));
      Serial.println(F("(iter)              Iterate an argument.  Accetps 2 arguments."));
      Serial.println(F("                    1) Number of times to iterate.  Specify a # from 1 to 30."));
      Serial.println(F("                    2) Expresssion to be interated."));
      Serial.println(F("                    Example command: (iter 2 '(help))"));
      Serial.println(F("(unit_tests)        Runs unit tests.  Helpful for testing that "));
      Serial.println(F("                    the Serial Monitor that is installed is a")); 
      Serial.println(F("                    stable version."));
      Serial.println(F("(i2c_help)          Print help for I2C commands."));
      Serial.println(F("(openag_help)       Print help for Open Ag Module commands."));
   
      return -1;

   } else {

      Serial.println(F("Unknown help command."));
      return 0;
   }
}

// args -> number of milliseconds to delay
//
int invoke_delay(int args) {

   int i = get_int(car(args));

   if (i > 0 && i <= 15000) {
      delay(i);
      return -1;
   }

   return 0;
}

void test_built_in_functions() {

   String tn = F("test_built_in_functions:");

   char test[] = "unit_tests";
   int test2 = find_built_in_function(make_str(test));

   assert_true(tn, is_pair(test2));
   assert_char_equals(tn, 'B', get_char(car(test2)));
   assert_int_equals(tn, 1, get_int(cdr(test2))); 

}
