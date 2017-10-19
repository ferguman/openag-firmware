#include "Arduino.h"
#include <pair.h>
#include <types.h>

// Arguments -> String containing the bytes read from the probe.
//
int atlas_show_ph(int args) {

   if (!is_pair(args)) { return nil; }

   uint8_t return_code = (uint8_t)(get_str(car(args))[0]);

   Serial.print("Atlas PH Probe reading status: ");

   switch (return_code) {    //switch case based on what the response code is.
      case 1:                       //decimal 1.
        Serial.println("Success");  //means the command was successful.
        break;                        //exits the switch case.

      case 2:                        //decimal 2.
        Serial.println("Failed");    //means the command has failed.
        break;                         //exits the switch case.

      case 254:                      //decimal 254.
        Serial.println("Pending");   //means the command has not yet been finished calculating.
        break;                         //exits the switch case.

      case 255:                      //decimal 255.
        Serial.println("No Data");   //means there is no further data to send.
        break;                       //exits the switch case.
    }

    Serial.print("PH = "); Serial.println(get_str(car(args)) + 1);

    return -1;   // Return success code.
}

