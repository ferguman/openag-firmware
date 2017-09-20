#include "Arduino.h"
#include <pair.h>
#include <extract_symbol.h>
#include <tokenize.h>
#include <parse.h>

void print_test_data(String test_name, int a, int b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.print(b);
}

void assert_int_equals(String test_name, int a, int b) {
   print_test_data(test_name, a, b);
   if (a == b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}

void assert_int_not_equals(String test_name, int a, int b) {
   print_test_data(test_name, a, b);
   if (a != b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}

void print_char_test_data(String test_name, char a, char b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.print(b);
}

void assert_char_equals(String test_name, char a, char b) {
   print_char_test_data(test_name, a, b);
   if (a == b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}

void print_str_test_data(String test_name, String a, String b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.print(b);
}

void assert_str_equals(String test_name, String a, String b) {
   print_str_test_data(test_name, a, b);
   if (a == b) {
      Serial.println("-> Pass");
   } else {
      Serial.println("-> Fail");
   }
}
void run_tests() {
   test_pair();
   test_extract_symbol();
   test_tokenize();
   test_parse();
}
