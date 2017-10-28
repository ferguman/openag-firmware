#include "Arduino.h"
#include <Wire.h>
#include <types.h>
#include <pair.h>

int i2c_send(int args);
int i2c_begin(int args);
int i2c_begin_trans(int args);
int i2c_write(int args);
int i2c_read(int args);
int i2c_end_trans(int args);
boolean read_chars(char *read_buffer, int buffer_index);

// This is a set of monitor function that allows access to the I2C Wire library 
// running on the Arduino.
//
// Monitor Command Examples:
// 
// (i2c_begin  1234))
// (atlas_show_ph (i2c_cmd 99 'R 2000 20 1))
//

//

int i2c_help(int args) {

   Serial.println(F("(i2c_help)          Prints this message."));
   Serial.println(F("(i2c_cmd args)      Send a command to an I2C device. Accepts 5 arguments:"));
   Serial.println(F("                    1) i2c_addr -> 7 bit address of device to send data to."));
   Serial.println(F("                    2) data -> Data that should be written. Specify strings as"));
   Serial.println(F("                               as symbols (e.g. 'R)")); 
   Serial.println(F("                    3) read delay -> The number of milliseconds to pause between writing the command to the ")); 
   Serial.println(F("                                     device and reading data from the device.")); 
   Serial.println(F("                    4) # of bytes to read")); 
   Serial.println(F("                    5) send stop bit -> Set to 1 to send a stop message after the i2c read. Refer to the Arudino ")); 
   Serial.println(F("                                        Wire libary (Wire.RequestFrom() command) for information on how to use this")); 
   Serial.println(F("                                        bit with values of 0.")); 

   return -1;

}
// Arguments: i2c address (7 bits), payload (an integer or symbol), delay between
// send and read in milliseconds, Number of bytes as an Integer, Send Stop as Integer (0 or 1)
//
int i2c_cmd(int args) {

   if (i2c_send(args)) {

      delay(get_int(caddr(args)));

      return i2c_read(cons(car(args), cons(cadddr(args), cons(cadr(cdddr(args)), nil))));

   } else {
      return nil;
   } 
}

//Arguments: Address as an Integer, Number of bytes as an Integer, Send Stop as Integer (0 or 1)
//
int i2c_read(int args) {

   //char read_char;
   char read_data[30];
   int buffer_index = 0;

   Wire.requestFrom(get_int(car(args)), 20, 1);

   //Read the Wire buffer
   if (read_chars(read_data, buffer_index)) {
      Wire.endTransmission();
      return make_str(read_data);
   } else {
      Wire.endTransmission();
      return 0;
   }
}

boolean read_chars(char *read_buffer, int buffer_index) {

   if (!Wire.available() || buffer_index >= 29) {

      read_buffer[buffer_index] = 0;              //null terminate the string

      if (buffer_index > 0) {
         return true;
      } else {
         return false;
      }

   } else {

      read_buffer[buffer_index] = (char) Wire.read();

      if (read_buffer[buffer_index] == 0) {
         return true;
      } else {
         return read_chars(read_buffer, buffer_index+1);
      }
  }
}

// Arguments: i2c address (7 bits), payload (an integer or symbol)
//
int i2c_send(int args) {

   if (i2c_begin(args)) {
      if (i2c_begin_trans(args)) {
         if (i2c_write(cdr(args))) {
            if (i2c_end_trans(args)) {
               //Serial.println("i2c_send successuf.");
               return 1;
            }
         }
      }
   }

   Serial.println("i2c_send failure.");

   return 0;

}

// Accept an I2c address (7 bits) and call the Wire library begin command. 
//
int i2c_begin(int args) {

   //Serial.println("Calling Wire.begin()");
   
   Wire.begin();

   return 1;

}

// Accept an I2C address (7 bits) and invoke Wire.beginTransmission(address)
//
int i2c_begin_trans(int args) {

   //Serial.print("Calling Wire.beginTransmission with address value ");
   //Serial.println(get_int(car(args)));

   Wire.beginTransmission(get_int(car(args)));

   return 1;
}

// Accept an integer or a symbol and write the integer or the characters of the symbol to the I2C
int i2c_write(int args) {

   if (is_int(car(args))) {
      //Serial.print("Calling Wire.write with value "); Serial.println(get_int(car(args)));
      Wire.write(get_int(car(args)));
   } else {
      //Serial.print("Calling Wire.write with value: "); Serial.println(get_str(cdar(args)));
      Wire.print(get_str(cdar(args)));
   }
 
   return 1;
}

// Invoke Wire.endTransmission()
//
int i2c_end_trans(int args) {

   //Serial.println("Invoking Wire.endTransmission()");

   Wire.endTransmission();
   
   return 1;
}


// Expects the following args: uint8_t addr, uint8_t count
//
int i2c_request_from(int args) {

   uint8_t addr = (uint8_t) get_int(car(args));
   uint8_t count = (uint8_t) get_int(cadr(args));

   //Serial.println("Calling Wire.requestFrom()");
   //Serial.print("addr: "); Serial.println(addr);
   //Serial.print("count: "); Serial.println(count);

   return Wire.requestFrom(addr, count);

}
