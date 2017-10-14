#ifndef MODULE_CONTAINER
#define MODULE_CONTAINER

#include "Arduino.h"

class module_container {

   public:
      int add_module(int args);

   private:
      Am2315 *am2315_1;
};

#endif
