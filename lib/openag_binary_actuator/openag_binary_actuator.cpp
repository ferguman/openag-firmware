#include "openag_binary_actuator.h"

BinaryActuator::BinaryActuator(int pin, bool is_active_low, uint32_t shutoff_ms) {
  _pin = pin;
  _is_active_low = is_active_low;
  _shutoff_ms = shutoff_ms;
  _last_cmd = 0;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t BinaryActuator::begin() {

  pinMode(_pin, OUTPUT);

  if (_is_active_low) {
    digitalWrite(_pin, HIGH);
  }
  else {
    digitalWrite(_pin, LOW);
  }

  return status_level;
}

uint8_t BinaryActuator::update() {

  uint32_t curr_time = millis();

  if ((curr_time - _last_cmd) > _shutoff_ms) {
    if (_is_active_low) {
      digitalWrite(_pin, HIGH);
    }
    else {
      digitalWrite(_pin, LOW);
    }
  }

  return status_level;
}

uint8_t BinaryActuator::set_cmd(const char *cmd_str) {

    /*
    bool cmd = false;
    if (strlen(cmd_str) == 4) {
        if ((cmd_str[0] == 'T' || cmd_str[0] == 't') && 
	    (cmd_str[1] == 'R' || cmd_str[1] == 'r') &&	    
	    (cmd_str[2] == 'U' || cmd_str[2] == 'u') &&	    
	    (cmd_str[3] == 'E' || cmd_str[3] == 'e')) {
            cmd = true;
	}
    }	

    return this->set(cmd);
    */

   return this->set(Module::str_to_bool(cmd_str));
}

uint8_t BinaryActuator::set(bool cmd) {

    _last_cmd = millis();
    
    if (cmd ^ _is_active_low) {       //^ == XOR
        digitalWrite(_pin, HIGH);
    }
    else {
        digitalWrite(_pin, LOW);
    }

    return status_level;
}

void BinaryActuator::show_state() {

   Serial.print(F("Binary Actuator is :"));

   if (digitalRead(_pin) ^ _is_active_low) {
      Serial.println(F(" On."));
   } else {
      Serial.println(F(" Off."));
   } 
}

int BinaryActuator::cmd(int args) {

   char set_cmd[] = "set";
   char state[] = "state";

   if (this->is_cmd(args, set_cmd)) {
      return make_int(this->set((boolean)get_int(car(cddr(args)))));
   }

   if (this->is_cmd(args, state)) {
      this->show_state();
      return make_int(status_level);
   }

   return Module::common_cmd(args);
}

// Actuators don't support this function. 
void BinaryActuator::print_readings_as_csv() {

    Serial.print("ERROR - This module doesn't support printing readings.");
}
