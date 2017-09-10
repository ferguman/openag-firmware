#include "Arduino.h"

int test_func(int i) {
   Serial.print(F("test_func: ")); Serial.println(i*i);
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
typedef int (*function_ptr)(int i);
const function_ptr fp_array[] PROGMEM = {
   &test_func, 
   &test_func_args};

int apply(int op, int args, int env) { 

    return (*(fp_array[0]))((*(fp_array[1]))(args));

}
