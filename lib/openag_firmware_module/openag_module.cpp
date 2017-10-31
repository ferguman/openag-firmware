#include "Arduino.h"
#include <openag_module.h>

boolean Module::is_cmd(int args, char *cmd) {

   if (get_char(caadr(args)) == 'S' && strcmp(cmd, get_str(cdadr(args))) == 0) {
      return true;
   } else {
      return false;
   }
} 

int Module::common_cmd(int args) {

   char begin[] = "begin";
   char update[] = "update";

   if (this->is_cmd(args, begin)) {
      //return show_open_ag_module_status(this->begin());
      return make_int(this->begin());
   }

   if (this->is_cmd(args, update)) {
      return make_int((int) this->update());
   }
   return -1;
}

