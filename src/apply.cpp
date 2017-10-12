#include "Arduino.h"
#include <test.h>
#include <types.h>
#include <pair.h>
#include <apply.h>
#include <built_in_functions.h>

int apply(int op, int args, int env) { 

//Serial.print("args is a pair? ");Serial.println(is_pair(args));
//Serial.print("args value:"); Serial.println(get_str(car(args)));

//   int fi = find_built_in_function(op) + 0;

//   if (fi) {
   if (is_pair(op)) {
      return apply_built_in_function(op, args);
   } else {
      return 0;
   }

   //char result[] = "Error - Unknown function.";
   //return cons(make_str(result), nil);

}

void test_apply() {

}
