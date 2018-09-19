#include "openag_tone_actuator.h"

// TODO decide if we want to keep the tone_duration parameter since we already have a _shutoff_ms.
ToneActuator::ToneActuator(int pin, bool is_active_low, int tone_frequency, int _tone_duration) {
  _pin = pin;
  _is_active_low = is_active_low;
  _tone_frequency = tone_frequency;
  _tone_duration = _tone_duration;
  _last_cmd = 0;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t ToneActuator::begin() {
  return status_level;
}

uint8_t ToneActuator::update() {
  uint32_t curr_time = millis();
  if ((curr_time - _last_cmd) > _shutoff_ms) {
    noTone(_pin);
  }
  return status_level;
}

// Actuators don't support this function. 
void ToneActuator::print_readings_as_csv() {

    Serial.print("ERROR - This module doesn't support printing readings.");
}

uint8_t ToneActuator::set_cmd(const char *cmd_str) {

   return set(Module::str_to_bool(cmd_str));
}

uint8_t ToneActuator::set(bool cmd) {

  _last_cmd = millis();

  bool actual_command = _is_active_low ? !cmd : cmd;
  if (actual_command) {
    if (_tone_duration > 0){
      tone(_pin, _tone_frequency, _tone_duration);
    }else{
      tone(_pin, _tone_frequency);
    }
  }
  else {
    noTone(_pin);
  }
  return status_level;
}

int ToneActuator::cmd(int args) {

   char set_cmd[] = "set";

   if (this->is_cmd(args, set_cmd)) {
      return make_int(this->set((boolean)get_int(car(cddr(args)))));
   }

   return Module::common_cmd(args);
}
