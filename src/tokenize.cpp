#include "Arduino.h"
#include <types.h>
#include <pair.h>
#include <test.h>

int parse_int(String *str, int cur_pos);

// Break the input message up into a stream of tokens.
// The following tokens are generated: 
// ' -> A tick is generated when a symbol character (') is detected.
// ( or ) -> parantheses are passed through as is.
// I or F  -> Integers or Floats (e.g. 1001 or 1001.001 Numbers are generated when a number is 
//         succesfully parsed from the input stream.
// " -> String.
// Value -> Everything else that is not too long and doesn't contain illegal characters.
// Error -> Stuff that can't be tokenized.
//
// The tokenizer builds a list using pairs. 
int token_iter(String *str, unsigned int cur_pos, int token_list) {

   int next_pos = 0;

   //Check for end of the list or a null token_list.
   if (cur_pos > str->length() || token_list == 0) {
      return token_list;
   }

   //Throw away space characters.
   if (str->charAt(cur_pos) == ' ' || str->charAt(cur_pos) == '\t') {
       next_pos = cur_pos++;
       return token_iter(str, next_pos, token_list);
   }

   //Check for parenthesis or ' (i.e sybmol) characters.
   if (str->charAt(cur_pos) == '(' || str->charAt(cur_pos) == ')' || str->charAt(cur_pos) == '\'') {
       next_pos = cur_pos++;
       return token_iter(str, next_pos, cons(token_list, make_char(str->charAt(cur_pos))));
   }

   //Check for string.
   //TODO: Write string extractor.

   //Check for float.
   //TODO: Write the float extractor.

   //Check for integer. 
   int value = parse_int(str, next_pos);
   //TODO: replace the predicate below with an is_pair(value) function.
   if (value > 0 and value <= 100) {
      next_pos = cur_pos + get_int(car(value)); 
      return token_iter(str, next_pos, cons(token_list, cdr(value)));
   }

   //Check for token.
   //TODO: Write the token extractor.

   Serial.println("Error in tokenizer. Illegal character encountered.");
   return 0;
}

int tokenize(String *str) {

   if (str->length() == 0) {
      return 0;
   }

   if (str->charAt(0) == '(') {
       //Start a token list with a parenthesis.
       return token_iter(str, 1, cons(make_char('('), nil));
   } else {
      Serial.println("All lines must be fully parenthisized.");
      return 0;
   }
}

// Return a pair -> (length_of_integer, integer_type_pointer)
int parse_int(String *str, int cur_pos) {

   return cons(make_int(5), make_int(12345));

}
