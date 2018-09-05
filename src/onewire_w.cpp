#include "Arduino.h"
#include <OneWire.h>
#include <pair.h>
#include <types.h>

int one_wire_help(int args) {

   Serial.println(F("(ow_help)       Prints this message."));
   Serial.println(F("(ow_addr arg)   Find the address of a One Wire device. Accepts 1 argument:"));
   Serial.println(F("                1) Pin Number -> Pin number of the pin to which the One Wire slave is connected."));
   Serial.println(F("                NOTE: This command assumes one and only one One Wire device is connected"));
   Serial.println(F("                to the given pin."));
   return -1;
}


// Arguments: OneWire device Arudino pin #
//
int one_wire_addr(int args) {

   OneWire ow(get_int(car(args)));
   byte addr[8];

   while (ow.search(addr)) {

      Serial.print(F("Found 1-Wire device with address: "));

      for(int i=0; i<8; i++) {
         Serial.print("0x");

         if (addr[i] < 16) {
            Serial.print('0');
         }

         if (i == 7) {
            Serial.println(addr[i], HEX);
         }
         if (i < 7) {
            Serial.print(addr[i], HEX);
            Serial.print(", ");
         }
       }
   }

   if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return -1;
   }

   ow.reset_search();
   return -1;
}

