#include "Arduino.h"
#include <tokenize.h>
#include <types.h>
#include <pair.h>
#include <test.h>
#include <extract_symbol.h>

int tokenize(String *str) {

   if (str->length() == 0) {
      return 0;
   }

   if (str->charAt(0) == '(') {
       //Start a token list with a parenthesis.
       int tok_list = cons(make_char('('), nil);
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

// Break the input message up into a list of tokens. 
// Tokens are constructed as pairs. The car of the token contains the
// token type and the cdr contains a pointer to a data type (e.g. integer, 
// string, char) that is appropriate for the given token type.
// The following tokens are generated: 
// ' -> The ' character serves as the quote character and is passed through as is.
// ( or ) -> parantheses are passed through as is.
// I or F  -> Integers or Floats (e.g. 1001 or 1001.001 Numbers are generated when a number is 
//         succesfully parsed from the input stream.
// X -> Strings are generated when strings (deliminated by ") are encountered.
// S -> A symbol is generated when none of the above token types apply and the input is legal.
// [value as entered by the user] -> Everything else that is not too long and doesn't contain illegal characters.
// Error -> Stuff that can't be tokenized.
//
int token_iter(String *str, unsigned int cur_pos, int token_list) {

   int next_pos = 0;

   //Check for end of the list or a null token_list.
   if (cur_pos >= str->length() || token_list == 0) {
      return token_list;
   }

   //Throw away space characters.
   if (str->charAt(cur_pos) == ' ' || str->charAt(cur_pos) == '\t') {
       next_pos = cur_pos++;
       return token_iter(str, next_pos, token_list);
   }

   //Check for parenthesis or ' (i.e sybmol) characters.
   if (str->charAt(cur_pos) == '(' || str->charAt(cur_pos) == ')' || str->charAt(cur_pos) == '\'') {
       return token_iter(str, cur_pos + 1, add_to_list(token_list, cons(make_char(str->charAt(cur_pos)), nil)));
   }

   //Check for string.
   //TODO: Write string extractor.

   //Check for float.
   //TODO: Write the float extractor.

   //Check for integer. 
   //int value = parse_int(str, next_pos);
   //TODO: replace the predicate below with an is_pair(value) function.
   //if (value > 0 and value <= 100) {
   //   next_pos = cur_pos + get_int(car(value)); 
   //   return token_iter(str, next_pos, cons(token_list, cdr(value)));
   //}

   //Check for symbol. 
   int end_of_sym = extract_sym(str, cur_pos);
   if (end_of_sym != 0) { 
      return token_iter(str, end_of_sym + 1, add_to_list(token_list, cons(make_char('S'), make_str((*str).substring(cur_pos, end_of_sym)))));
   } 

   Serial.println("Error in tokenizer. Illegal character encountered.");
   return 0;
}


// Return a pair -> (length_of_integer, integer_type_pointer)
int parse_int(String *str, int cur_pos) {

   return cons(make_int(5), make_int(12345));

}
