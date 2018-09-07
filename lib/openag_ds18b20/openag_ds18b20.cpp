/**
 *  \file openag_ds18b20.cpp
 *  \brief Sensor module for temperature.
 */
#include "openag_ds18b20.h"

// Note that the serial monitor command (ow_addr pin_no) will display the address of the connected slave.
// This module assumes one connected slave on the designated PIN.  It looks up the address of the slave
// automatically within the begin routine.
//
Ds18b20::Ds18b20(int pin) : _oneWire(pin) {
//- Ds18b20::Ds18b20(int pin) {
  //- _oneWire ow(5);
  _sensors = DallasTemperature(&_oneWire);
  //- _sensors = DallasTemperature(&ow);
  _sensors.setWaitForConversion(false);
}

uint8_t Ds18b20::begin() {
  _sensors.begin();
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
  _waiting_for_conversion = false;
  _time_of_last_query = 0;
  if (!_sensors.getAddress(_address, 0)) {
    status_level = ERROR;
    status_code = CODE_COULDNT_FIND_ADDRESS;
    status_msg = "Unable to find address for sensor";
  }
  return status_level;
}

uint8_t Ds18b20::update() {
  if (_waiting_for_conversion) {
    if (_sensors.isConversionComplete()) {
      status_level = OK;
      status_code = CODE_OK;
      status_msg = "";
      _waiting_for_conversion = false;
      _temperature = _sensors.getTempC(_address);
      _send_temperature = true;
    }
    else if (millis() - _time_of_last_query > _min_update_interval) {
      status_level = ERROR;
      status_code = CODE_NO_RESPONSE;
      status_msg = "Sensor isn't responding to queries";
    }
  }
  if (millis() - _time_of_last_query > _min_update_interval) {
    _sensors.requestTemperatures();
    _waiting_for_conversion = true;
    _time_of_last_query = millis();
  }
  return status_code;
}

uint8_t Ds18b20::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}

float Ds18b20::get_temperature() {
  return _temperature;
}

void Ds18b20::print_readings_as_csv() {
   Serial.print(_temperature);
}

int Ds18b20::cmd(int args) {

   char read[] = "read";
   char addr[] = "addr";

   if (this->is_cmd(args, read)) {
      Serial.print(F("Water Temp.: ")); Serial.println(_temperature);
      return make_int(status_code);
   }

   if (this->is_cmd(args, addr)) {
       Serial.println(F("this command is not implemented."));  
   }
   return Module::common_cmd(args);
}
