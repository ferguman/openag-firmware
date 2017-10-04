#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <tokenize.h>
#include <test.h>
#include <interpret.h>
#include <parse.h>
#include <pair.h>
#include <types.h>

int process_message(String *input) {

   //TODO need to deal with backspaces. there should probably be
   // a routine in tokenize that looks for backspaces and skips
   // the backspaced characters.

   //Tokenize the input. Returns a list of tokens.
Serial.println(*input);

   int token_list  = tokenize(input);
Serial.println(token_list);
   if (is_pair(token_list)) {
Serial.println("in parse area.");
	
      //Parse the token list. Returns a parse tree.
      int parse_tree = parse(token_list);

      if (is_pair(parse_tree)) {
Serial.println("in interpret area.");
         //Intepret the parse tree. The result can be a list or 
         //one of the built-in types.
         int interpret_return = interpret(parse_tree);

         if (is_pair(interpret_return)) {
Serial.println("interpret over.");
            print_result(interpret_return);
            Serial.println("OK");
         }
      } 
   }

   clear_pairs();
   clear_types();
   return 0;
}
