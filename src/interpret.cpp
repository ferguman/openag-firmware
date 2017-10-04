#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <apply.h>
#include <interpret.h>
#include <types.h>
#include <pair.h>

// Accept a parse tree as input.  
//
int interpret(int input) {

   //Random test stuff - pull out later.
   //Adafruit_AM2315 am2315;
   //Serial.println(am2315.readHumidity());
   //Serial.println(am2315.readTemperature());

   if (is_pair(input)) {

      return apply(car(input), 0, 0);
   }

   return 0;

}

// TODO: put an experiment in her to call a function with arguments that is
// stored in an array of "canned" funceitons.
int test_canned_function_call(int function) {
   return 0;
}
