#include "ec.h"

SensorEc::SensorEc(int probe_pin, int pwr_relay_ctrl_pin) {

    probe_pin_ = probe_pin;
    pwr_relay_ctrl_pin_ = pwr_relay_ctrl_pin;
}

uint8_t SensorEc::begin() {
   //Turn power relay off.

   pinMode(pwr_relay_ctrl_pin_, OUTPUT);

   status_level = OK;
   return status_level;
}

uint8_t SensorEc::update() {

    // TBD - May need to goto a more sophisticated power management scheme.
    // Power up the probe and let it debounce for 3 milliseconds.
    digitalWrite(pwr_relay_ctrl_pin_, HIGH);
    delay(3);
    
    ec_ = ec_calibration_offset + (((float)analogRead(probe_pin_) * (5.0/1024.0)) * ec_calibration_gain);

    // Turn probe power off. 
    digitalWrite(pwr_relay_ctrl_pin_, LOW);
    delay(3);

    status_level = OK;
    return status_level;
}

uint8_t SensorEc::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}

float SensorEc::get_ec() {
    return ec_;
}

void SensorEc::print_readings_as_csv() {
    Serial.print(ec_);
}

int SensorEc::cmd(int args) {

   char read_cmd[] = "read";
   char set_relay_cmd[] = "relay";

   if (this->is_cmd(args, read_cmd)) {
      Serial.print(F("EC: ")); Serial.println(ec_);
      return make_int(status_level);
   }

   if (this->is_cmd(args, set_relay_cmd)) {
      int cmd = get_int(cdr(args));
      if (cmd == 0) {
	   digitalWrite(pwr_relay_ctrl_pin_, LOW);
      } else if (cmd == 1) {
	   digitalWrite(pwr_relay_ctrl_pin_, HIGH);
      } else {
          Serial.println(F("ERROR: Illegal value. Specify 1 or 0 for the pin state."));
      }	  
      return make_int(status_level);
   }

   return Module::common_cmd(args);
}
