#include "openag_pwm_actuator.h"

PwmActuator::PwmActuator(int pin, bool is_active_low, float threshold) {
  _pin = pin;
  _is_active_low = is_active_low;
  _threshold = threshold;
  _last_cmd = 0;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t PwmActuator::begin() {
  pinMode(_pin, OUTPUT);
  if (_is_active_low) {
    digitalWrite(_pin, HIGH);
  }
  else {
    digitalWrite(_pin, LOW);
  }
  return status_level;
}

uint8_t PwmActuator::update() {
  uint32_t curr_time = millis();
  if ((curr_time - _last_cmd) > _max_update_interval) {
    if (_is_active_low) {
      analogWrite(_pin, 255);
    }
    else {
      analogWrite(_pin, 0);
    }
  }
  return status_level;
}


uint8_t PwmActuator::set_cmd(const char *cmd_str) {

  float val = atof(cmd_str);
  // TBD - put in error checking for the conversion from cmd_str to val.
  //
  return this->set(val);
}

uint8_t PwmActuator::set(float cmd) {

  _last_cmd = millis();

  float val = cmd;

  if (val < 0 || val > 1) {
    status_level = WARN;
    status_code = CODE_INVALID_COMMAND;
    status_msg = "Invalid command received: ";
    status_msg += val;
    return status_level;
  }
  else {
    status_level = OK;
    status_code = OK;
    status_msg = "";
  }
  val = _threshold + (1-_threshold)*val;
  if (_is_active_low) {
    val = 1-val;
  }
  int pwm_value = val*255;
  analogWrite(_pin, pwm_value);
  return status_level;
}

// Actuators don't support this function. 
void PwmActuator::print_readings_as_csv() {

    Serial.print("ERROR - This module doesn't support printing readings.");
}

int PwmActuator::cmd(int args) {

   char set_cmd[] = "set";

   if (this->is_cmd(args, set_cmd)) {
      return make_int(this->set(get_float(car(cddr(args)))));
   }

   return Module::common_cmd(args);
}
