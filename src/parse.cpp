#include "Arduino.h"
#include <parse.h>
#include <types.h>
#include <pair.h>
#include <test.h>

// The parser accepts a list of tokens (see tokenize.cpp). It scans the list from front to rear.
// Every time an open paranthesis is encountered a new list is started and every time a close
// paranthesis is encountered the curent list is closed and the resulting list is added to the 
// list that is a level above the current list.
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
    if (get_char(car(token_list)) == '(') {
       int cur_list = cons(nil, nil);
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

// Add an item to the last pair of a list.
int add_list_item(pair, item) {

   if (car(pair) == nil) {
      set_car(pair, item);
      return pair;
   } else {
      set_cdr(list, cons(item, nil));
      return cdr(list);
   }

}

// list_stack is used to store each level of lists. As "("'s are encountered
// new list are started and the current list is pushed to the stack. As ")"'s 
// are encountered the higher level list is popped off the stack and 
// and parsing continues.
//
int parse_iter(int list_stack, int cur_list, int token_list) {

   // No more tokens to parse.
   if (cdr(token_list) == nil) {
      return list_stack;
   }

   // Start of list
   if (get_char(car(token_list)) == '(') {
      int nl = cons(nil, nil);
      return parse_iter(cons(nl, list_stack), add_list_item(cur_list, nl), cdr(token_list));
   }

   // End of list
   if (get_char(car(token_list)) == ')') {
      if (cdr(list_stack) == nil) {
         Serial.println(F("Error in parse_iter: Unbalanced paranthesises."));
         return 0; 
      } else {
         if (get_char(caddr(list_stack)) == '\'') {
            return parse_iter(cddr(list_stack), caddr(list_stack), cdr(token_list));
         } else {
            return parse_iter(cdr(list_stack), cadr(list_stack), cdr(token_list));
         }
      }
   }

   // Beginning of quote
   if (get_char(car(token_list)) == '\'') {
      int nl = add_list_item(cur_list, cons(car(token_list), nil));
      return parse_iter(cons(nl, list_stack), nl, cdr(token_list));
   }

   // Add symbols, strings, floats and integers to the current list
   if (is_type(car(token_list))) {
      int nl = add_list_item(cur_list, car(token_list));
      if (is_tos_a_quote_char) {
         return parse_iter(cddr(list_stack)), caddr(list_stack), cdr(token_list));
      } else {
         return parse_iter(cdr(list_stack)), cadr(list_stack), cdr(token_list));
      }
   }
       
   // Something is wrong.
   Serial.println(F("Error parsing. Unrecognized token.");
   return 0;

}

void test_parse() {
   assert_int_equals(F("parse.cpp"), 0, parse(0));
   int test1 = cons(make_char('X'),nil);
   assert_int_equals(F("parse.cpp"), 0, parse(test1));
   //Pass (foobar) to parse.
   int test2 = parse(cons(make_char('('),cons(make_str("foobar"), cons(make_char(')'), nil))));
   assert_str_equals(F("parse.cpp"), F("foobar"), get_str(car(test2)));
}

/*   Examples
#######
((1 2) (3 4))

n1
add_nodes[(1 2) (3 4)), n1]
N1->n2
add_nodes[1 2) (3 4)), n2]
N1-n2-1
add_nodes[2) (3 4)), n2]
N1-n2-1,2
add_nodes[) (3 4)), n2]
N1-n2-1,2
add_nodes[(3 4)), n1]
N1-n2-1,2
  -n3
add_nodes[3 4)), n3]
N1-n2-1,2
  -n3-3
add_nodes[4)), n3]
N1-n2-1,2
  -n3-3,4
add_nodes[)), n3]
Add_nodes[), n1] -> returns n1



####################
(Bar)
N1
add_nodes[bar), n1]
N1->bar
add_nodes[), n1] -> returns n1
*/
