#include "Arduino.h"
#include <extract_symbol.h>
#include <types.h>
#include <pair.h>
#include <test.h>

// Internal functions
int add_digit_to_num(int digit, int num);
boolean is_digit(char ascii_char);
int convert_ascii_to_digit(int ascii_char);
int make_num_extract(int pos, int numer, int denom);
int extract_num_iter(String *str, int cur_int, unsigned int start_pos, unsigned int cur_pos);

// Return 0 if no number is found.
// Return cons(the end position of the number in the string, cons("N", cons(numerator as integer type, 
// cons(denominator as integer type)))) if a number is found.
//
int extract_num(String *str, unsigned int cur_pos) {

   // Look for a negative sign at the start of the number.
   // Is the character a negative sign = ASCII 45
   if (str->charAt(cur_pos) == 45) {
      if ((cur_pos+1 < str->length()) && is_digit(str->charAt(cur_pos+1))) {
         return extract_num_iter(str, -1 * convert_ascii_to_digit(str->charAt(cur_pos+1)), cur_pos, cur_pos+2);
      } else {
         return 0;
      }
   } 

   // Is the character a plus sign = ASCII 43
   if (str->charAt(cur_pos) == 43) {
      return extract_num_iter(str, 0, cur_pos, cur_pos+1);
   }

   // Is the first character a digit
   if (is_digit(str->charAt(cur_pos))) {
      return extract_num_iter(str, 0, cur_pos, cur_pos);
   }

   return 0;
}

// TODO: Add a base parameter so that the system can process binary, octal, and hex numbers in
//       addition to the current decimal.
//
int extract_num_iter(String *str, int cur_int, unsigned int start_pos, unsigned int cur_pos) {

   //Beyond the last character of the input string. 
   if (cur_pos >= str->length()) {
      if (start_pos < cur_pos && start_pos < str->length()) {
         return make_num_extract(cur_pos, cur_int, 0);
      } else {
         return 0;
      }
   }

   // Check for decimal point
   if (str->charAt(cur_pos) == 46) {

      //Fork the processing to get the number to the right of the decimal point.
      int frac_part = extract_num_iter(str, 0, start_pos, cur_pos+1);
      if (is_pair(frac_part)) {
         return make_num_extract(get_int(car(frac_part)), cur_int, get_int(caddr(frac_part)));
      } else {
         // Failure in fractional part.
         Serial.println("Error in extract_num_iter: Couldn't extract fractional part of a number.");
         return 0;
      }
   }

   // Check for digit
   if (is_digit(str->charAt(cur_pos))) {
      if ((cur_int >= 0 && cur_int <= 32767) || (cur_int < 0 && cur_int >= -32766)) {
         //Continue to scan as long as you are encountering digits
         return extract_num_iter(str, add_digit_to_num(convert_ascii_to_digit(str->charAt(cur_pos)), cur_int), start_pos, cur_pos + 1);
      } else {
         Serial.println("Error in extract_num_iter: Number contains too many digits.");
      }
   } 

   //No more digit characters
   return make_num_extract(cur_pos, cur_int, 0);
}

int add_digit_to_num(int digit, int num) {
  
   if (num < 0) {
      return (num * 10) + (-1 * digit);
   } else {
      return (num * 10) + digit;
  }
}

int make_num_extract(int pos, int numer, int denom) {

   return cons(make_int(pos), cons(make_char('N'), cons(make_int(numer), cons(make_int(denom),nil))));
}

int convert_ascii_to_digit(int ascii_char) {

   if (ascii_char >= 48 && ascii_char <= 57) {
      return ascii_char - 48;
   } else {
      Serial.println("convert_ascii_to_digit: Illegal character code.");
      return 0;
   }
}

boolean is_digit(char ascii_char) {
   
   //Accept ASCII characters for digits.
   if (ascii_char >= 48 && ascii_char <= 57) {
      return true;
   }

   return false;
}

void test_extract_num() {

   char zero = '0';
   char nine = '9';
   assert_true(F("extract_num.cpp"), is_digit(zero)); 
   assert_true(F("extract_num.cpp"), is_digit(nine)); 
 
   assert_int_equals(F("extract_num.cpp"), 3, convert_ascii_to_digit('3'));

   int num = make_num_extract(99, 3, 4);
   assert_int_equals(F("extract_num.cpp"), 99, get_int(car(num)));
   assert_char_equals(F("extract_num.cpp"), 'N', get_char(cadr(num))); 
   assert_int_equals(F("extract_num.cpp"), 3, get_int(caddr(num)));
   assert_int_equals(F("extract_num.cpp"), 4, get_int(cadddr(num)));

   assert_int_equals(F("extract_num.cpp"), 75, add_digit_to_num(5, 7));
   assert_int_equals(F("extract_num.cpp"), -8385, add_digit_to_num(5, -838));
   
   String str = "1234";
   num = extract_num(&str, 0);
   assert_char_equals(F("extract_num.cpp"), 'N', get_char(cadr(num))); 
   assert_int_equals(F("extract_num.cpp"), 1234, get_int(caddr(num)));
   assert_int_equals(F("extract_num.cpp"), 0, get_int(cadddr(num)));
   
   str = "-1234.7869";
   num = extract_num(&str, 0);
   assert_int_equals(F("extract_num.cpp"), 10, get_int(car(num))); 
   assert_char_equals(F("extract_num.cpp"), 'N', get_char(cadr(num))); 
   assert_int_equals(F("extract_num.cpp"), -1234, get_int(caddr(num)));
   assert_int_equals(F("extract_num.cpp"), 7869, get_int(cadddr(num)));

   str = "?";
   assert_int_equals(F("extract_num.cpp"), extract_num(&str, 0), 0); 
   
}
