#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <apply.h>
#include <interpret.h>
#include <types.h>
#include <pair.h>
#include <built_in_functions.h>
#include <test.h>

int get_operands(int exp) { return cdr(exp); }
int get_operator(int exp) { return car(exp); }
int first_operand(int exp) {return car(exp); }
int rest_operands(int exp) {return cdr(exp); }
int no_operands(int exp) { return (exp == nil); }
int interpret(int exp);
boolean self_evaluating(int exp);
int list_of_values(int exps, int env); 
boolean is_variable(int exp);
int lookup_variable(int exp);
boolean is_quoted(int exp);
int quoted_item(int exp);

// Accept a parse tree as input.  
//
int interpret(int exp) {

   //Serial.print("eval-current char: "); Serial.println(get_char(caar(exp)));

   if (self_evaluating(exp)) { return exp; };

   // lookup_varible should invoke the built in function lookup code and rture a pair
   // pair -> cons('M' or 'B', Integer) M-> module function, B-> built in fucniotn 
   // Integer -> index of function.
   //
   if (is_variable(exp)) { return lookup_variable(exp); }

   if (is_quoted(exp)) { return quoted_item(exp); }

   if (is_pair(exp)) {
      return apply(interpret(get_operator(exp)), 
                   list_of_values(get_operands(exp),0), 0);
   } else {
      Serial.println("interpret.cpp: Unkown input");
      return 0;
   }
}

boolean is_quoted(int exp) {

   if (is_char(car(exp)) && get_char(car(exp)) == '\'') {
      return true;
   } else {
      return false;
   }
}

int quoted_item(int exp) { 

   return cdr(exp);
}

int lookup_variable(int exp) {

   int result = find_built_in_function(cdr(exp));

   if (is_pair(result)) {
      return result;
   } else {
      Serial.println(F("interpret.cpp lookup_variable: Error: Cannot find referenced variable."));
      return nil;
   }
}

boolean is_variable(int exp) {

   if (is_pair(exp) && is_char(car(exp)) && get_char(car(exp)) == 'S') {
      return true;
   } else { 
      return false;
   }
}

boolean self_evaluating(int exp) {
   if (is_type(exp)) {
      return true;
   } else {
      return false;
   }
}

int list_of_values(int exps, int env) {

   if (no_operands(exps)) {
      return nil;
   } else { 
      return cons(interpret(first_operand(exps)), list_of_values(rest_operands(exps), 0));
   }
}

void test_interpret() {

   String tn = F("test_interpret:");


}
