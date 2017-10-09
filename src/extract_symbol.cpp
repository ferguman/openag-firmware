#include "Arduino.h"
#include <extract_symbol.h>
#include <extract_num.h>
#include <types.h>
#include <pair.h>
#include <test.h>

int extract_sym(String *str, int cur_pos) {
   return extract_sym_iter(str, cur_pos, cur_pos);
}

// Return 0 if no symbol is found.
// Return the end position of the symbol in the string if a symbol is found.
//
int extract_sym_iter(String *str, unsigned int start_pos, unsigned int cur_pos) {

   //Beyond the last chacter of the string. 
   if (cur_pos >= str->length()) {
      if (start_pos < cur_pos && start_pos < str->length()) {
         return cur_pos - 1;
      } else {
         return 0;
      }
   } else {
      // Don't accept numbers as the first character of symbols.
      if (is_sym_char(str, cur_pos) || ((cur_pos > start_pos) && is_digit(str->charAt(cur_pos)))) {
         return extract_sym_iter(str, start_pos, cur_pos + 1);
      } else {
         if (start_pos < cur_pos) {
            return cur_pos - 1;
         } else {
            return 0;
         } 
     }
   } 
}

boolean is_sym_char(String *str, int char_pos) {
   
   //Accept upper case ASCII
   if (str->charAt(char_pos) >= 65 && str->charAt(char_pos) <= 90) {
      return true;
   }

   //Accept lower case ASCII
   if (str->charAt(char_pos) >= 97 && str->charAt(char_pos) <= 122) {
      return true;
   }

   //Accept "-"
   if (str->charAt(char_pos) == 95) {
      return true;
   }

   return false;
}

void test_extract_symbol() {

   String test = F("how");
   String test2 = F("(test");
   String test3 = F("?");

   assert_int_equals(F("extract_symbol.cpp"), extract_sym(&test, 1), 2);
   assert_int_equals(F("extract_symbol.cpp"), extract_sym(&test2, 0), 0);
   assert_int_equals(F("extract_symbol.cpp"), extract_sym_iter(&test, 3, 4), 0);
   assert_int_equals(F("extract_symbol.cpp"), extract_sym(&test3, 0), 0);
}
