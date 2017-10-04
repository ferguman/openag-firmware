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

//int foobar = run_tests(0);

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

   clear_pairs();
   clear_types();
   return 0;
}
