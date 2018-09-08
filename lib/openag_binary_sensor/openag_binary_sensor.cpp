/**
 *  \file openag_binary_sensor.cpp
 *  \brief Binary Sensor for OpenAg brain.
 */
/***************************************************

 ****************************************************/
#include "openag_binary_sensor.h"

BinarySensor::BinarySensor(int pin, bool is_active_low) {
  _pin = pin;
  _is_active_low = is_active_low;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t BinarySensor::begin() {
  pinMode(_pin, INPUT);
  _is_on = false;
  _time_of_last_reading = 0;
  return status_level;
}

uint8_t BinarySensor::update() {
  if (millis() - _time_of_last_reading > _min_update_interval) {
    readData();
    _time_of_last_reading = millis();
  }
  return status_level;
}

uint8_t BinarySensor::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}

void BinarySensor::print_readings_as_csv() {
    if (_is_on) {
       Serial.print(1);
    } else {
       Serial.print(0);
    }
}

bool BinarySensor::get_is_on() {
    if (_is_on) {
       return 1;
    } else {
       return 0;
    }
}

void BinarySensor::readData() {
  _is_on = digitalRead(_pin) ^ _is_active_low;
}

int BinarySensor::cmd(int args) {

   char read[] = "read";

   if (this->is_cmd(args, read)) {

      Serial.print(F("Sensor is "));

      if (_is_on) {
         Serial.println("on.");
      } else {
         Serial.println("off.");
      }
      
      return make_int(status_level);

   }

   return Module::common_cmd(args);
}

