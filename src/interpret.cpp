#include "Arduino.h"
#include <Adafruit_AM2315.h>

//TODO: Test that we can store the function array in flash.
//      Test calling functions with argument
//      Move the pre-stored function to its own code file.

// Declare the type to be a function that returns a void with an 
// unspecified number of arguments.
typedef void (*function_ptr)();
function_ptr fp_array[1];

void test_func() {
   Serial.println("test_func");
}


int interpret(int input) {

    //Note - the line below must be in a function or the compiler displays an unknow
    //       type error.
    fp_array[0] = &test_func;
    (*(fp_array[0]))();


   Adafruit_AM2315 am2315;
   Serial.println(am2315.readHumidity());
   Serial.println(am2315.readTemperature());

//Also make one of the canned functions a manual mode that tells the
//system to ignore stop their usual functions and await manual commands

    return 1;
}

// TODO: put an experiment in her to call a function with arguments that is
// stored in an array of "canned" funceitons.
int test_canned_function_call(int function) {
   return 0;
}
