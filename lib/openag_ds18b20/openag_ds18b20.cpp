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

   // If no address has been found then look again.
   // This was added to hack around the condition when the sensor address is not found during the
   // call to the begin() method.
   if (status_code == CODE_COULDNT_FIND_ADDRESS) { 
      if (!_sensors.getAddress(_address, 0)) {
         status_level = ERROR;
         status_code = CODE_COULDNT_FIND_ADDRESS;
         status_msg = "Unable to find address for sensor";
	 return status_level;
      }
   }

   // The address has been found so now take a measurement
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

uint8_t Ds18b20::addr_cmd() {
	
   Serial.print(F("Address: "));

   for(int i=7; i>=0; i--) {
      Serial.print("0x");

      if (_address[i] < 16) {
         Serial.print('0');
      }

      if (i == 0) {
         Serial.print(_address[i], HEX);
      }
      if (i > 0) {
         Serial.print(_address[i], HEX);
         Serial.print(", ");
      }
   }

   if (OneWire::crc8(_address, 7) != _address[7]) {
      Serial.print(F("CRC code is not valid. Device sent CRC code: 0x"));
      Serial.print(_address[7], HEX);
      Serial.print(F(". The calcualted crc is: 0x"));
      Serial.println(OneWire::crc8(_address, 7), HEX);
   } else {
      Serial.println(F(""));
   }
   
   return status_code; 
}

int Ds18b20::cmd(int args) {

   char read[] = "read";
   char addr[] = "addr";

   if (this->is_cmd(args, read)) {
      Serial.print(F("Water Temp.: ")); Serial.println(_temperature);
      return make_int(status_code);
   }

   if (this->is_cmd(args, addr)) {
      return make_int(addr_cmd());
      //- Serial.println(F("this command is not implemented."));  
   }

   return Module::common_cmd(args);
}
