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

/* -
// Child are expected to override this if they want to support a set_cmd.
//
uint8_t Module::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}
*/

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

bool Module::str_to_bool(const char *cmd_str){

    bool cmd = false;

    if (strlen(cmd_str) == 4) {
        if ((cmd_str[0] == 'T' || cmd_str[0] == 't') && 
	    (cmd_str[1] == 'R' || cmd_str[1] == 'r') &&	    
	    (cmd_str[2] == 'U' || cmd_str[2] == 'u') &&	    
	    (cmd_str[3] == 'E' || cmd_str[3] == 'e')) {
            cmd = true;
	}
    }	

    return cmd;
}
