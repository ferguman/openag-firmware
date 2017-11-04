#include "openag_doser_pump.h"

DoserPump::DoserPump(int pin, bool is_active_low, float lowerBound, float upperBound) {
  _pin = pin;
  _is_active_low = is_active_low;
  _lowerBound = lowerBound;
  _upperBound = upperBound;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t DoserPump::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, bool2command(false));
  _on_duration = 0;
  _off_duration = _dosingFreq;
  return status_level;
}

uint8_t DoserPump::update() {

  uint32_t curr_time = millis();

  // If shutoff time has passed, with no call to set_rate, set the pumps to OFF
  if(_isOn && curr_time - _last_cmd > _shutoff_ms){
    _isOn = false;
    digitalWrite(_pin, bool2command(_isOn));
    status_level = WARN;
    status_code = CODE_STALE_COMMAND;
    status_msg = "There has been no set command recently, stopping dosing.";
    return status_level;
  }

  // Dose or not dose based on the current set frequencies
  if(_isOn && curr_time - _last_pulse > _on_duration){
    _isOn = false;
    _last_pulse = curr_time;
  }
  if(!_isOn && curr_time - _last_pulse > _off_duration){
    _isOn = true;
    _last_pulse = curr_time;
  }
  digitalWrite(_pin, bool2command(_isOn));
  return status_level;
}

// Set the pump driver to pump the rate (ml/h).
uint8_t DoserPump::set_cmd(float rate) {

  Serial.print("In Doser set_cmd. Rate = ");
  Serial.println(rate);

  uint32_t curr_time = millis();

  if(rate > 0 && rate < _lowerBound){
    status_level = ERROR;
    status_code = CODE_LOWER_BOUND;
    status_msg = "A command to dose lower than the maximum precision was sent.";
    return status_level;
  }
  if(rate > _upperBound){
    status_level = ERROR;
    status_code = CODE_UPPER_BOUND;
    status_msg = "A command to dose higher than the maximum flow rate was sent.";
    return status_level;
  }

  // onRatio cannot be greater than 1 if we check for UPPER_BOUND above
  // Therefore we can assume that _on_duration is less than dosingFreq
  // and _off_duration is positive.
  float onRatio = rate / _upperBound;

  _on_duration = onRatio * _dosingFreq;
  _off_duration = _dosingFreq - _on_duration;

  // Update the most recent command
  _last_cmd = curr_time;

  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
  return status_level;
}

// Output a HIGH/LOW signal based on _is_active_low and the boolean passed to
// always turn on when passed true.
uint8_t DoserPump::bool2command(bool isOn){
  bool realValue = _is_active_low ? !isOn : isOn;
  return realValue ? HIGH : LOW;
}

void DoserPump::show_state() {

   Serial.print(F("Dosing Period (msec): ")); Serial.println(_dosingFreq);
   Serial.print(F("Dosing Rate Lower Bound: ")); Serial.println(_lowerBound);
   Serial.print(F("Dosing Rate Upper Bound: ")); Serial.println(_upperBound);

   Serial.print(F("Auto shutoff time (msec): ")); Serial.println(_shutoff_ms);
   Serial.print(F("Current On Duration (msec): ")); Serial.println(_on_duration);
   Serial.print(F("Current Off Duration (msec): ")); Serial.println(_off_duration);

   Serial.print(F("Last Command: ")); Serial.println(_last_cmd);
   Serial.print(F("Last Change: ")); Serial.println(_last_pulse);
   Serial.print(F("Control Pin Active High: ")); Serial.println(!_is_active_low);
   Serial.print(F("Control Pin On: ")); Serial.println(_isOn);
}

int DoserPump::cmd(int args) {

   Serial.println("in doser pump cmd");

   char set_cmd[] = "set";
   char state_cmd[] = "state";

   if (this->is_cmd(args, set_cmd)) {
      return make_int(this->set_cmd((float)get_float(car(cddr(args)))));
   }

   if (this->is_cmd(args, state_cmd)) {
      this->show_state();
      return make_int(OK);
    }


   return Module::common_cmd(args);
}
