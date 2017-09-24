#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <tokenize.h>
#include <test.h>
#include <interpret.h>
#include <parse.h>

int process_message(String *input) {

   //Tokenize the input. Returns a list of tokens.
   int token_list  = tokenize(input);

   //Parse the token list. Returns a parse tree.
   int parse_tree = parse(token_list);

   //Intepret the parse tree. The result can be a list or 
   //one of the built-in types.
   int interpret_return = interpret(parse_tree);

   //run all the unit tests. TODO: Need to figoure out a way to turn this on and off.
   //create a command that calls this. then it can be called by the interpretter.
   //run_tests(0);

   Serial.println("OK");
   return interpret_return;
}
