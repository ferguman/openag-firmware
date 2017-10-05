#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <tokenize.h>
#include <test.h>
#include <interpret.h>
#include <parse.h>
#include <pair.h>
#include <types.h>

//const int ESML = 65;  //Includes terminating null character.

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

/*
boolean remove_backspaces_v2(String *input_str, char *output_str, unsigned int input_cur_pos, unsigned int output_cur_pos) {

   // Stop when you reach the end of the input string.
   if (input_cur_pos >= input_str->length()) {
      // set end of string marker for output string.
      if (output_cur_pos < ESML) {
         output_str[output_cur_pos] = 0;
         return true;
      } else {
         Serial.println("remove_backspaces: output string overflow");
         output_str[ESML-1] = 0;
         return false;
      }
   }

   // Implement character removal when backspace is encountered.
   if (input_str->charAt(input_cur_pos) == 8) {
      if (output_cur_pos > 0) {
         return remove_backspaces(input_str, output_str, input_cur_pos + 1, output_cur_pos - 1);
      } else {
         remove_backspaces(input_str, output_str, input_cur_pos + 1, output_cur_pos);
      }
   } 

   if (output_cur_pos < ESML) {
      output_str[output_cur_pos] = input_str->charAt(input_cur_pos);
      return remove_backspaces(input_str, output_str, input_cur_pos + 1, output_cur_pos + 1);
   } else {
      Serial.println("remove_backspaces: output string overflow");
      return false;
   }
}
*/

int process_message(String *input) {
//int foobar = run_tests(0);

   //Scan the input and removed backspaced characters
   //char eval_str[ESML];
   //if (remove_backspaces_v2(input, eval_str, 0, 0)) {
   
   if (remove_backspaces(input, 0, 0)) {

      //Tokenize the input. Returns a list of tokens.
      int token_list  = tokenize(input);

      if (is_pair(token_list)) {
	
         //Parse the token list. Returns a parse tree.
         int parse_tree = parse(token_list);

         if (is_pair(parse_tree)) {
            //Intepret the parse tree. The result can be a list or 
            //one of the built-in types.
            int interpret_return = interpret(parse_tree);

            if (is_pair(interpret_return)) {
               print_result(interpret_return);
               Serial.println("OK");
            }
         } 
      }
   }

   clear_pairs();
   clear_types();
   return 0;
}

void test_process_message() {

   String test1 = F("(ab\x8)");
   String test2 = "(a)";
   assert_true(F("process_message.cpp"), remove_backspaces(&test1, 0, 0));
   assert_str_equals(F("process_message.cpp"), test2, test1);

}
