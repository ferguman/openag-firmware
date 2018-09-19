#include "openag_pulse_actuator.h"

PulseActuator::PulseActuator(int pin, bool is_active_low, int pulse_ms, int update_ms) {
  _pin = pin;
  _is_active_low = is_active_low;
  _pulse_ms = pulse_ms;
  _update_ms = update_ms;
  _last_cmd = 0;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t PulseActuator::begin() {
  pinMode(_pin, OUTPUT);
  if (_is_active_low) {
    digitalWrite(_pin, HIGH);
  }
  else {
    digitalWrite(_pin, LOW);
  }
  return status_level;
}

uint8_t PulseActuator::update() {
  uint32_t curr_time = millis();
  // If it's been _pulse_ms of time ON, turn oFF.
  if(_state && (curr_time - _last_cmd) > _pulse_ms){
    _state = false;
    _last_cmd = curr_time;
  }
  digitalWrite(_pin, bool2command(_state));
  return status_level;
}

uint8_t PulseActuator::set_cmd(const char *cmd_str) {

  return this->set(Module::str_to_bool(cmd_str));
}

uint8_t PulseActuator::set(bool cmd) {

  uint32_t curr_time = millis();

  // Only pulse once every update_ms
  if ((curr_time - _last_cmd) < _update_ms) {
    return status_level;
  }

  _last_cmd = curr_time;
  _state = cmd;

  return status_level;
}

// Actuators don't support this function. 
void PulseActuator::print_readings_as_csv() {

    Serial.print("ERROR - This module doesn't support printing readings.");
}

// Take true/false and convert to HIGH/LOW based on is_active_low
uint8_t PulseActuator::bool2command(bool isOn){
  bool realState = _is_active_low ? !isOn : isOn;
  return realState ? HIGH : LOW;
}

int PulseActuator::cmd(int args) {

   char set_cmd[] = "set";

   if (this->is_cmd(args, set_cmd)) {
      return make_int(this->set((boolean)get_int(car(cddr(args)))));
   }

   return Module::common_cmd(args);
}
