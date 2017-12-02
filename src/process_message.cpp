#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <tokenize.h>
#include <test.h>
#include <interpret.h>
#include <parse.h>
#include <pair.h>
#include <types.h>

const int FC_CMD_DETECTED = 1;

// Removes backspaces characters (ASCII 8) by shifting everything left. Note that this
// routine does not resize the String. So that after it operates the resulting String
// will the equivalent of a copy of itself with backspaces removed concatented with 
// enough characters to keep its lengtht he same.  It is assumed that downstream processing
// scans the string from left to right and ends at some token(s) such as carriage return and
// line feeds within the string thereby ignoring the junk on the end.
//
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
            //
            print_result(interpret(parse_tree));
         } 
      }
   }

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
   assert_str_equals(tn, test2, test1.substring(0, 3));

}
