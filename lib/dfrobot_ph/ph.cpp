#include "ph.h"

SensorPh::SensorPh(int pin) {

   // Set the analong input pin that will be used to read the Ph.
   pin_ = pin;

}

uint8_t SensorPh::begin() {

    //Nothing to be done.
    status_level = OK;
    return status_level;
}

// Calibration adjustments are provided to allow interpretation of the ph_ readings as points on a line.
uint8_t SensorPh::update() {

	ph_ = ph_calibration_offset_ + (((float)analogRead(pin_)* (5.0/1024.0)) * ph_calibration_coefficient_);

    status_level = OK;
    return status_level; 
}

uint8_t SensorPh::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}

float SensorPh::get_ph() {
    return ph_;
}

void SensorPh::print_readings_as_csv() {
    Serial.print(ph_);
}

int SensorPh::cmd(int args) {

   char read_cmd[] = "read";

   if (this->is_cmd(args, read_cmd)) {
      Serial.print(F("Ph: ")); Serial.println(ph_);
      return make_int(status_level);
   }

   return Module::common_cmd(args);
}
