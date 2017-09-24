#include "Arduino.h"
#include <pair.h>
#include <extract_symbol.h>
#include <tokenize.h>
#include <parse.h>
#include <apply.h>

void print_test_data(String test_name, int a, int b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.println(b);
}

void print_pass_fail(boolean pass) {
   if (pass) {
      Serial.print("Pass ->");
   } else {
      Serial.print("Fail ->");
   }
}
void assert_int_equals(String test_name, int a, int b) {
   print_pass_fail(a==b);
   print_test_data(test_name, a, b);
}

void assert_int_not_equals(String test_name, int a, int b) {
   print_pass_fail(a != b);
   print_test_data(test_name, a, b);
}

void print_char_test_data(String test_name, char a, char b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.println(b);
}

void assert_char_equals(String test_name, char a, char b) {
   print_pass_fail(a == b);
   print_char_test_data(test_name, a, b);
}

void print_str_test_data(String test_name, String a, String b) {
   Serial.print(test_name); Serial.print(F(":a=")); Serial.print(a);
   Serial.print(F(",b=")); Serial.println(b);
}

void assert_str_equals(String test_name, String a, String b) {
   print_pass_fail(a == b);
   print_str_test_data(test_name, a, b);
}

int run_tests(int arg_list) {
   test_pair();
   test_extract_symbol();
   test_tokenize();
   test_parse();
   return 0;
}
