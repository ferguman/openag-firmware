#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <tokenize.h>
#include <test.h>
#include <interpret.h>
#include <parse.h>
#include <pair.h>
#include <types.h>

boolean remove_backspaces(String *input_str, unsigned int input_cur_pos, unsigned int output_cur_pos) {

   // Stop when you reach the end of the input string.
   if (input_cur_pos >= input_str->length()) {
      return true;
   }

   // Implement character removal when backspace is encountered.
   if (input_str->charAt(input_cur_pos) == 8) {
      if (output_cur_pos > 0) {
         return remove_backspaces(input_str, input_cur_pos + 1, output_cur_pos - 1);
      } else {
         return remove_backspaces(input_str, input_cur_pos + 1, output_cur_pos);
      }
   } 

   // Write current character to it's proper space in the string
   input_str->setCharAt(output_cur_pos, input_str->charAt(input_cur_pos));
   return remove_backspaces(input_str, input_cur_pos + 1, output_cur_pos + 1);
}

int process_message(String *input) {

   //int foobar = run_tests(0);

   //Scan the input and removed backspaced characters
   
   if (remove_backspaces(input, 0, 0)) {

      //Tokenize the input. Returns a list of tokens.
      int token_list  = tokenize(input);

      if (is_pair(token_list)) {
	
         //Parse the token list. Returns a parse tree.
         int parse_tree = parse(token_list);

         if (is_pair(parse_tree)) {
            //Intepret the parse tree. The result can be a list or 
            //one of the built-in types.
            //int interpret_return = interpret(parse_tree);

            //if (is_pair(interpret_return)) {
               print_result(interpret(parse_tree));
            //} 
         } 
      }
   }


   print_type_stats(); print_pair_stats();
   clear_pairs();
   clear_types();

   Serial.println("OK");

   return 0;
}

void test_process_message() {

   String tn = F("process_message.cpp");

   String test1 = F("(ab\x8)");
   String test2 = "(a)";
   assert_true(tn, remove_backspaces(&test1, 0, 0));
   assert_str_equals(tn, test2, test1);

}
