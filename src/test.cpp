#include "Arduino.h"
#include <pair.h>
#include <extract_symbol.h>
#include <extract_num.h>
#include <tokenize.h>
#include <parse.h>
#include <apply.h>
#include <types.h>
#include <process_message.h>

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

void assert_true(String test_name, boolean a) {
   print_pass_fail(a);
   print_test_data(test_name, true, a);
}

void assert_int_equals(String test_name, int a, int b) {
   print_pass_fail(a==b);
   print_test_data(test_name, a, b);
}

void assert_int_not_equals(String test_name, int a, int b) {
   print_pass_fail(a != b);
   print_test_data(test_name, a, b);
}

void assert_float_equals(String test_name, float a, float b) {
   print_pass_fail(a==b);
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

void assert_c_str_equals(String test_name, char *a, char *b) {

   char *ac;
   char *bc;
   ac = a;
   bc = b;

   boolean pass = true;
   while (*ac && pass) {
      if (*ac != *bc) {
          pass = false;
      }
      ac++;
      bc++;
   }

   if (*ac != *bc) {
      pass = false;
   }

   print_pass_fail(pass);

   Serial.print(test_name); Serial.print(F(":a="));
   Serial.print(a);
   Serial.print(F(",b="));
   Serial.print(b);
   Serial.println("");
}


int run_tests(int arg_list) { 

   // Test yourself first.
   char test[7] = "foobar";
   assert_c_str_equals(F("test.cpp"), test, test);

   // Now test all the other files.
   test_pair();
   test_types();
   test_extract_symbol();
   test_extract_num();
   test_tokenize();
   test_parse();
   test_process_message(); 

   char result[] = "End of Tests";
   return cons(make_str(result), nil);

}
