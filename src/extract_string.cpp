#include "Arduino.h"
#include <types.h>
#include <pair.h>
#include <test.h>

//Internal functions
boolean legal_string_char(char input_char);
int extract_string_iter(String *str, unsigned int cur_pos, char *sb, uint8_t cur_str_index);

const int MAX_STR_SIZE = 32;

//Scan the input string and test that it contains a string starting at the position indicated
//by cur_pos.
//If a string is not found then return 0.
//If a string is found then return a pair.
//The first item of the pair contains the index of the next character in the input
//buffer (i.e. str) which is after the end of the detected string.
//The second item of the pair contains the string (as a Type).
//
int extract_string(String *str, unsigned int cur_pos) {

   char sb[MAX_STR_SIZE + 1];

   //All strings must start with ".
   if (str->charAt(cur_pos) == 34) {
      return extract_string_iter(str, cur_pos + 1, sb, 0);
   } else {
      return 0;
   }
}

int extract_string_iter(String *str, unsigned int cur_pos, char *sb, uint8_t cur_str_index) {

   //Check to see if you have come to then end of the input buffer.
   if (cur_pos >= str->length()) {
      Serial.println(F("Error: Unterminated string detected."));
      return 0;
   }   

   //Check for overflow.
   if (cur_str_index >= MAX_STR_SIZE) {
      Serial.println("Error: String is too long. Strings are limited to 32 characters.");
      return 0;
   }

   //Check for escaped double quotes
   if ((cur_pos < str->length() - 1) && ((str->charAt(cur_pos) == 92) && (str->charAt(cur_pos + 1) == 34))) {
      sb[cur_str_index] = 34;
      return extract_string_iter(str, ++cur_pos, sb, ++cur_str_index);
   } 

   //Check for a legal string character
   if (legal_string_char(str->charAt(cur_pos))) {
      sb[cur_str_index] = str->charAt(cur_pos);
      return extract_string_iter(str, ++cur_pos, sb, ++cur_str_index);
   }

   //Check for end of string
   if (str->charAt(cur_pos) == 34) {
      sb[cur_str_index] = 0;
      return cons(make_int(cur_pos + 1), make_str(sb));
   }     

   //Must be an illegal character - declare and error.
   Serial.println(F("Error: String parsing error."));
   return 0;
}

boolean legal_string_char(char input_char) {
  
   //TBD need to add " escaping. 
   //Accept any printable character except for ". 
   if (input_char != 34) {
       if (input_char >= 32 && input_char <= 126) {
       return true;
       }
   }

   return false;
}

void test_extract_string() {

   String tn = F("extract_string.cpp");
   
   String test1 = "foo\"bar\"foo";

   int result = extract_string(&test1, 3);

   assert_true(tn, is_pair(result));
   assert_int_equals(tn, 8, get_int(car(result)));
   char test2[4] = "bar";
   assert_c_str_equals(tn, test2, get_str(cdr(result)));

}

