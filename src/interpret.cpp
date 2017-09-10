#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <apply.h>

int interpret(int input) {

   //Random test stuff - pull out later.
   //Adafruit_AM2315 am2315;
   //Serial.println(am2315.readHumidity());
   //Serial.println(am2315.readTemperature());

   return apply(1, 9, 3);
}

// TODO: put an experiment in her to call a function with arguments that is
// stored in an array of "canned" funceitons.
int test_canned_function_call(int function) {
   return 0;
}
