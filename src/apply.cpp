#include "Arduino.h"
#include <test.h>
#include <types.h>
#include <pair.h>
#include <apply.h>
#include <built_in_functions.h>

int apply(int op, int args, int env) { 

   int fi = find_built_in_function(op) + 0;

   if (fi) {
      return apply_built_in_function(fi, args);
   } 

   char result[] = "Error - Unknown function.";
   return cons(make_str(result), nil);

}

void test_apply() {

}
