#include "Arduino.h"
#include <pair.h>

void assert_int_equals(String test_name, int a, int b) {
   Serial.print(test_name); Serial.print(":a="); Serial.print(a);
   Serial.print(",b="); Serial.print(b);
   if (a == b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}

void assert_char_equals(String test_name, char a, char b) {
   Serial.print(test_name); Serial.print(":a="); Serial.print(a);
   Serial.print(",b="); Serial.print(b);
   if (a == b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}

void run_tests() {
   test_pair();
}
