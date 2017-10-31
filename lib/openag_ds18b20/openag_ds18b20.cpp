/**
 *  \file openag_ds18b20.cpp
 *  \brief Sensor module for temperature.
 */
#include "openag_ds18b20.h"

// 9/10/2017 - There is free software on the web that will poll the One wire bus and infer the 
//             address of all the devices that are on the bus.  I ran the software (using Arduino on
//             on my Mac and direct connecting the Arduino) and got the following value for the Ds18b20
//             that is currenlty connected to fc3.  
//
//             0x28, 0xFF, 0x4D, 0x0F, 0x62, 0x17, 0x03, 0x5D
//
Ds18b20::Ds18b20(int pin) : _oneWire(pin) {
  _sensors = DallasTemperature(&_oneWire);
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

float Ds18b20::get_temperature() {
  return _temperature;
}

int Ds18b20::cmd(int args) {

   return -1;
}
