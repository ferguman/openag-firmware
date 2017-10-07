#include "Arduino.h"
#include <parse.h>
#include <types.h>
#include <pair.h>
#include <test.h>
#include <parse.h>

// The parser accepts a list of tokens (see tokenize.cpp). It scans the list from front to rear
// and generates a new list that is a tree that mirrors the parentheitcal structure of the input
// token list.
//
// Every time an open paranthesis is encountered a new list is started and added to the end of the
// current list.  Every time a close paranthesis is encountered the current list is changed
// to be the list that was being used before the current list was started.  See the paragraph below
// for special handling of the close paranthesis character when a quoted list is detected.
//
// Every time the quote character is encountered a new list is started with the quote character
// at its head. Then another new list is created and added to the tail of the quote character list.
// Everytime a close parantheesis or token is added to the current list the parser
// checks to see if the current list's parent list is a quote. If it is then the parser 
// sets the cur list to be the list that contains the quote character.
//
// Returns a list of data items.
// 
// Parse creates an empty high level pair that is used to connect to the parsed list. This
// allows the lower level routines to check that they don't attempt to pop a list off an empty
// list stack. And yes a stack of lists is maintained to store higher level lists while lower
// level list are being scanned.
//
int parse(int token_list) {

    // Check for empty or missing token list.
    if (token_list == nil || !is_pair(token_list)) {
       Serial.println(F("Error in parse.cpp: Parse expects a list as input."));
       return 0;
    }

    // All list must start with a '('.
    if (get_char(caar(token_list)) == '(') {
       int cur_list = cons(nil, nil);
       //TODO - I don't think the next line is needed.
       int parsed_list = cons(cur_list, nil);
       if (parsed_list  == parse_iter(cons(cur_list, parsed_list), cur_list, cdr(token_list))) {
          return cur_list;
       } else {
          Serial.println(F("Error in parse.cpp: Parsing failed."));
          return 0;
       }
    } else {
       Serial.println(F("Error in parse.cpp: Token list does not start with (."));
       return 0;
    }       
    
    return 0;
}

// list_stack is used to store each level of lists. As "("'s are encountered
// new list are started and the current list is pushed to the stack. As ")"'s 
// are encountered the higher level list is popped off the stack and 
// and parsing continues.
//
int parse_iter(int list_stack, int cur_list, int token_list) {

   // No more tokens to parse.
   if (token_list == nil) {
      return list_stack;
   }

   // Start of list
   if (get_char(caar(token_list)) == '(') {
      int nl = cons(nil, nil);
      return parse_iter(cons(nl, list_stack), add_list_item(cur_list, nl), cdr(token_list));
   }

   // End of list
   if (get_char(caar(token_list)) == ')') {
      if (cdr(list_stack) == nil) {
         Serial.println(F("Error in parse_iter: Unbalanced paranthesises."));
         return 0; 
      } else {
         if (get_char(cadr(list_stack)) == '\'') {
            return parse_iter(cddr(list_stack), cddr(list_stack), cdr(token_list));
         } else {
            return parse_iter(cdr(list_stack), cdr(list_stack), cdr(token_list));
         }
      }
   }

   // Beginning of quote
   if (get_char(caar(token_list)) == '\'') {
      int nl = add_list_item(cur_list, cons(caar(token_list), nil));
      return parse_iter(cons(nl, list_stack), nl, cdr(token_list));
   }

   // Add symbols, strings, floats and integers to the current list
   if ((get_char(caar(token_list)) == 'S') || is_type(caar(token_list))) {
      int nl = add_list_item(cur_list, cdar(token_list));
      // If the symbol is quoted then pop to the quotes parent list.
      if (get_char(car(list_stack)) == '\'')  {
         return parse_iter(cdr(list_stack), cdr(list_stack), cdr(token_list));
      } else {
         return parse_iter(list_stack, nl, cdr(token_list));
      }
   }
       
   // Something is wrong.
   Serial.println(F("Error parsing. Unrecognized token."));
   return 0;

}

void test_parse() {

   // Test parse()
   // assert_int_equals(F("parse.cpp"), 0, parse(0));

   // int test1 = cons(make_char('X'),nil);
   //assert_int_equals(F("parse.cpp"), 0, parse(test1));

   //Pass (foobar) to parse.
   char symbol[7] = "foobar"; 
   int test2 = parse(cons(cons(make_char('('),nil), cons(cons(make_char('S'), make_str(symbol)), cons(cons(make_char(')'), nil), nil))));
   assert_str_equals(F("parse.cpp"), F("foobar"), get_str(car(test2)));
}
