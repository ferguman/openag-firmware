#include "Arduino.h"
#include <Adafruit_AM2315.h>
#include <apply.h>
#include <interpret.h>
#include <types.h>
#include <pair.h>

int get_operands(int exp) { return cdr(exp); }
int get_operator(int exp) { return car(exp); }
int first_operand(int exp) {return car(exp); }
int rest_operands(int exp) {return cdr(exp); }
int no_operands(int exp) { return (exp == nil); }
int interpret(int exp);
boolean self_evaluating(int exp);

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

// Accept a parse tree as input.  
//
int interpret(int exp) {

   if (self_evaluating(exp)) { return exp; };

   // lookup_varible should invoke the built in function lookup code and rture a pair
   // pair -> cons('M' or 'B', Integer) M-> module function, B-> built in fucniotn 
   // Integer -> index of function.

   //if (is_variable) { return lookup_variable(exp); };

   if (is_pair(exp)) {
      // TODO: redo apply to use the above function pair struction to invoke the proper function.
      return apply(get_operator(exp), list_of_values(get_operands(exp),0), 0);
   } else {
      Serial.println("interpret.cpp: Unkown input");
      return 0;
   }
}
