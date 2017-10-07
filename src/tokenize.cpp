#include "Arduino.h"
#include <tokenize.h>
#include <types.h>
#include <pair.h>
#include <test.h>
#include <extract_symbol.h>
#include <extract_num.h>

// Internal routines
boolean next_char_is(String *str, int char_ptr, char compare_char);

// Convert the input message string into a list of tokens. 
// Tokens are constructed as pairs. The car of the token contains the
// token type (as a single character) and the cdr contains a pointer to a data type (e.g. integer, 
// string, char) that is appropriate for the given token type.
// The following token types are generated: 
// ' -> The ' character serves as the quote character and is passed through as is.
// ( or ) -> parantheses are passed through as is.
// N -> Integers or Floats (e.g. 1001 or 1001.001 Numbers are generated when a number is 
//         succesfully parsed from the input stream.
// X -> Strings are generated when strings (deliminated by ") are encountered.
// S -> A symbol is generated when none of the above token types apply and the input is legal.
//

int tokenize(String *str) {

   if (str->length() == 0) {
      return 0;
   }

   if (str->charAt(0) == '(') {
       //Start a token list with a parenthesis.
       int tok_list = cons(cons(make_char('('), nil),nil);
       if (token_iter(str, 1, tok_list) != 0) {
          return tok_list;
       } else {
          return 0;  //Error
       }
   } else {
      Serial.println("All lines must be fully parenthesized.");
      return 0;
   }
}

int token_iter(String *str, unsigned int cur_pos, int token_list) {

   //Check for end of the list or a null token_list.
   if (cur_pos >= str->length() || token_list == 0 ) {
      return token_list;
   }
   
   //Stop if a carriage return or line feed is encountered in the input string.
   if (((int)str->charAt(cur_pos) == 13) || ((int)str->charAt(cur_pos) == 10)) {
      return token_list;
   }


   //Throw away space characters.
   if (str->charAt(cur_pos) == ' ' || str->charAt(cur_pos) == '\t') {
       return token_iter(str, cur_pos + 1, token_list);
   }

   //Check for parenthesis or ' (i.e sybmol) characters.
   if (str->charAt(cur_pos) == '(' || str->charAt(cur_pos) == ')' || str->charAt(cur_pos) == '\'') {
       return token_iter(str, cur_pos + 1, add_list_item(token_list, cons(make_char(str->charAt(cur_pos)),nil)));
   }

   //Check for symbol. 
   int end_of_sym = extract_sym(str, cur_pos);
   if (end_of_sym != 0) { 

      if(end_of_sym - cur_pos + 1 <= 30) {

         char symbol[31]; 

         for(int i=cur_pos; i<=end_of_sym; i++) {
            symbol[i-cur_pos] = str->charAt(i);
         }

         //null terminate the string.
         symbol[end_of_sym - cur_pos + 1] = 0;
      
         return token_iter(str, end_of_sym + 1, add_list_item(token_list, cons(make_char('S'), make_str(symbol))));
      } else {
         Serial.println("Error in tokenizer. Symbols may not be longer than 30 characters.");
      }
   } 
   
   //Check for number. 
   int num = extract_num(str, cur_pos);
   if (is_pair(num)) {
      return token_iter(str, get_int(car(num)), add_list_item(token_list, cdr(num)));
   }

   //Check for string.
   //TODO: Write string extractor.

   Serial.print("Error in tokenizer. Illegal character encountered. ASCII: ");
   Serial.println((int) str->charAt(cur_pos));
   return 0;
}

void test_tokenize() {

   String test1 = F("(foobar)");
   int result = tokenize(&test1);

   assert_int_not_equals(F("tokenize.cpp"), 0, result);
   assert_char_equals(F("tokenize.cpp"), '(', get_char(caar(result)));
   assert_char_equals(F("tokenize.cpp"), get_char(caadr(result)), 'S');
   char test2[] = "foobar";
   assert_c_str_equals(F("tokenize.cpp"), test2, get_str(cdr(cadr(result))));
   assert_char_equals(F("tokenize.cpp"), ')', get_char(caaddr(result)));

}
