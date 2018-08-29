#include "Arduino.h"
#include <openag_module.h>


int Module::show_status() {

   Serial.print("Status Level: "); Serial.println(status_level);
   Serial.print("Status Msg: "); Serial.println(status_msg);
   Serial.print("Status Code: "); Serial.println(status_code);

   return status_code;
}

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
   char status[] = "status";

   if (this->is_cmd(args, begin)) {
      return make_int(this->begin());
   }

   if (this->is_cmd(args, update)) {
      return make_int((int) this->update());
   }

   if (this->is_cmd(args, status)) {
      return make_int(this->show_status());
   }

   return 0;
}

