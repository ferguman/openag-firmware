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

// Accept an I2C address (7 bits) and invoke Wire.beginTransmission(address)
//
int i2c_begin_trans(int args) {

   Serial.print("Calling Wire.beginTransmission with address value ");
   Serial.println(get_int(car(args)));

   Wire.beginTransmission(get_int(car(args)));

   return 0;
}

// Accept an integer or a symbol and write the integer or the characters of the symbol to the I2C
int i2c_write(int args) {

   if (is_int(car(args))) {
      Serial.print("Calling Wire.write with value "); Serial.println(get_int(car(args)));
      Wire.write(get_int(car(args)));
   } else {
      Serial.print("Calling Wire.write with value: "); Serial.println(get_str(cdar(args)));
      Wire.print(get_str(cdar(args)));
   }
 
   return 0;
}

// Invoke Wire.endTransmission()
//
int i2c_end_trans(int args) {

   Serial.println("Invoking Wire.endTransmission()");

   Wire.endTransmission();
   
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
