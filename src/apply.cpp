#include "Arduino.h"
#include <test.h>
#include <types.h>
#include <pair.h>
#include <apply.h>
#include <built_in_functions.h>
#include <environment.h>

int apply(int op, int args, int env) { 

   if (is_pair(op)) {
      return apply_function(op, args);
   } else {
      return 0;
   }

}

void test_apply() {

}
