#include "Arduino.h"
#include <Wire.h>
#include <types.h>
#include <pair.h>

// This is a monitor function that allows access to the I2C Wire library 
// running on the Arduino.
//
// Monitor Command Examples
// 
// (i2c_begin  1234))

// Accept an I2c address (7 bits) and call the Wire library begin command. 
//
int i2c_begin(int args) {

   Serial.println("Calling Wire.begin()");
   
   Wire.begin();

   return 0;

}

// Expects the following args: uint8_t addr, uint8_t count
//
int i2c_request_from(int args) {

   uint8_t addr = (uint8_t) get_int(car(args));
   uint8_t count = (uint8_t) get_int(cadr(args));

   Serial.println("Calling Wire.requestFrom()");
   Serial.print("addr: "); Serial.println(addr);
   Serial.print("count: "); Serial.println(count);

   return Wire.requestFrom(addr, count);

}
