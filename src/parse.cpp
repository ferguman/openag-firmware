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
// Every time a close parantheesis or token is added to the current list the parser
// checks to see if the current list's parent list is a quote. If it is then the parser 
// sets the cur list to be the list that contains the quote character.
//
// Returns a list of data items.
// 
// Parse creates an empty high level pair that is used to connect to the parsed list. This
// allows the lower level routines to check that they don't attempt to pop a list off an empty
// list stack. And yes a stack of lists is maintained to store higher level lists while lower
// level list are being scanned. This empty high level pair is not returned.
//
int parse(int token_list) {

    // Check for empty or missing token list.
    if (token_list == nil || !is_pair(token_list)) {
       Serial.println(F("Error in parse.cpp: Parse expects a list as input."));
       return 0;
    }

    // All list must start with a '('.
    if (get_char(car(token_list)) == '(') {

       int cur_list = cons(nil, nil);  // Return this to the user after it is filled with content.
       //int parsed_list = cons(cur_list, nil);

       // Put an empty high level pair on the stack so that the parser can check for stack underflow.
       // Put the head of the parsed list as the top of the stack for easy retreival at the end of the 
       // parsing iterations.
       //if (cur_list  == parse_iter(cons(parsed_list, cons(cur_list, nil)), cur_list, cdr(token_list))) {
       if (parse_iter(nil, cur_list, cdr(token_list))) {
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
      Serial.println("Error in parse_iter: Unexpected end of input tokens.");
      return false;
      //return car(list_stack);
   }

   // Start of list
   if (get_char(car(token_list)) == '(') {
      int nl = cons(nil, nil);
      int parent_list = add_list_item(cur_list, nl);
      return parse_iter(cons(parent_list, list_stack), nl, cdr(token_list));
   }

   // End of list
   if (get_char(car(token_list)) == ')') {

      if (list_stack == nil)  {
         if (cdr(token_list) == nil) {
            //We are at the end of the list
            return true;
         } else {
            Serial.println("Error in parse_iter: Unexpected last paranthesis found.");
            return false;
         }
      } 

      if (get_char(caar(list_stack)) == '\'') {
         Serial.println("Error in parser_iter: Unbalanced last paranthesis found inside a symbol.");
         return false;
      } else {
         if (get_char(caadr(list_stack)) == '\'') {
            return parse_iter(cadddr(list_stack), caddr(list_stack), cdr(token_list));
         } else {
            return parse_iter(cadr(list_stack), cadr(list_stack), cdr(token_list));         
         }
      }
   }

   // Beginning of quote
   if (get_char(car(token_list)) == '\'') {

      int quoted_list = cons(car(token_list), nil);
      int parent_list  = add_list_item(cur_list, quoted_list);

      return parse_iter(cons(quoted_list, cons(parent_list, list_stack)), quoted_list, cdr(token_list));
   }
   
   // A symbol
   if (get_char(caar(token_list)) == 'S') {

      // If the symbol is quoted then pop to the quotes parent list.
      if (get_char(caar(list_stack)) == '\'')  {
         // Store token in cdr of cur_list.
         set_cdr(cur_list, car(token_list));
         return parse_iter(cddr(list_stack), cadr(list_stack), cdr(token_list));
      } else {
         // Add token to the end of the current list.
         int nl = add_list_item(cur_list, car(token_list));
         return parse_iter(list_stack, nl, cdr(token_list));
      }
   }

   // Add strings, floats and integers to the current list
   if (is_type(car(token_list))) {
      int nl = add_list_item(cur_list, car(token_list));
      return parse_iter(list_stack, nl, cdr(token_list));
   }
       
   // Something is wrong.
   Serial.println(F("Error parsing. Unrecognized token."));
   return 0;

}

void test_parse() {

   String tn = F("parse.cpp");

   //Pass (foobar) to parse.
   char symbol[7] = "foobar"; 
   int test2 = parse(cons(make_char('('), 
                          cons(cons(make_char('S'), make_str(symbol)), 
                          cons(make_char(')'),nil))));

   assert_int_equals(tn, 0, cdr(test2));
 
   char test3[] = "foobar"; 
   assert_char_equals(tn, 'S', get_char(caar(test2)));
   assert_c_str_equals(tn, test3, get_str(cdar(test2)));

   char symbol2[6] = "hello";
   int test4 = parse(cons(make_char('('), 
                     cons(cons(make_char('S'), make_str(symbol)), 
                     cons(make_char('\''), 
                     cons(cons(make_char('S'), make_str(symbol2)),
                     cons(make_char(')'), nil))))));

   assert_char_equals(tn, 'S', get_char(caar(test4)));
   assert_c_str_equals(tn, symbol, get_str(cdar(test4)));
   assert_char_equals(tn, '\'', get_char(caadr(test4)));
   assert_char_equals(tn, 'S', get_char(cadr(cadr(test4))));
   assert_c_str_equals(tn, symbol2, get_str(cddr(cadr(test4))));
   //assert_char_equals(tn, 'S', get_char(car((cadr(cadr(test4))))));
   //assert_c_str_equals(tn, symbol2, get_str(cdr(cadr(cadr(test4)))));

}
