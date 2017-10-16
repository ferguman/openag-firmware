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
// If the input can be cast to an Integer, Float, or String type then 
// the case is performed and the resulting Type is added to the list.
// If the input is a quote character then it is passed through as is.
// If the input is a parenthesis, or a Symbol then
// it is added to the list as a pair.  The car of the pair contains the
// token type (as a single character) and the cdr contains a pointer to a data type (e.g. integer, 
// string, char) that is appropriate for the given token type.
//
// The following token types are generated: 
// ' -> The ' character serves as the quote character and is passed through as is.
// ( or ) -> parantheses are passed through as is.
// S -> A symbol is generated when none of the above token types apply and the input is legal.
// I, F, S -> Integer, Float and Strings are added to the token list as they are encountered in the 
//            in the input stream.
//

int tokenize(String *str) {

   if (str->length() == 0) {
      return 0;
   }

   if (str->charAt(0) == '(') {
       //Start a token list with a parenthesis.
       //int tok_list = cons(cons(make_char('('), nil),nil);
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
       //return token_iter(str, cur_pos + 1, add_list_item(token_list, cons(make_char(str->charAt(cur_pos)),nil)));
       return token_iter(str, cur_pos + 1, add_list_item(token_list, make_char(str->charAt(cur_pos))));
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

   String tn = F("tokenize.cpp");

   String test1 = F("(foobar)");
   int result = tokenize(&test1);

   assert_int_not_equals(tn, 0, result);
   assert_char_equals(tn, '(', get_char(car(result)));
   assert_char_equals(tn, get_char(caadr(result)), 'S');
   char test2[] = "foobar";
   assert_c_str_equals(tn, test2, get_str(cdr(cadr(result))));
   assert_char_equals(tn, ')', get_char(caddr(result)));

   String test3 = F("(foobar2 argument)");
   result = tokenize(&test3);
   char test4[] = "foobar2";
   assert_c_str_equals(tn, test4, get_str(cdr(cadr(result))));
   assert_char_equals(tn, 'S', get_char(caaddr(result)));
   char test5[] = "argument"; 
   assert_c_str_equals(tn, test5, get_str(cdr(caddr(result))));

   String test6 = F("(foo_bar 125 56.32)");
   result = tokenize(&test6);

   char test7[] = "foo_bar";
   assert_c_str_equals(tn, test7, get_str(cdr(cadr(result))));
   assert_int_equals(tn, 125, get_int(caddr(result)));
   assert_int_equals(tn, 5632, (int) (100 * get_float(car(cdddr(result)))));

   String test8 = F("(foo_bar 'Find)");
   result = tokenize(&test8);

   assert_char_equals(tn, '(', get_char(car(result)));
   char test10[] = "Find";
   assert_char_equals(tn, '\'', get_char(caddr(result)));
   assert_char_equals(tn, 'S', get_char(car(cadddr(result))));
   assert_c_str_equals(tn, test10, get_str(cdr(cadddr(result))));
}
