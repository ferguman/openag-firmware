/**
 *  \file openag_atlas_ph.cpp
 *  \brief Potential hydrogen sensor.
 */
#include "openag_atlas_ph.h"

AtlasPh::AtlasPh(int i2c_address) {
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
  _time_of_last_query = 0;
  _waiting_for_response = false;
  _i2c_address = i2c_address;
}

uint8_t AtlasPh::begin() {
  Wire.begin();
  Wire.setTimeout(40);
  return status_level;
}

uint8_t AtlasPh::update() {
  if (_waiting_for_response) {
    if (millis() - _time_of_last_query > 1800) {
      read_response();
    }
  }
  else if (millis() - _time_of_last_query > _min_update_interval) {
    send_query();
  }
  return status_level;
}

void AtlasPh::print_readings_as_csv() {
  Serial.print(_water_potential_hydrogen);
}

float AtlasPh::get_water_potential_hydrogen() {
  return _water_potential_hydrogen;
}

uint8_t AtlasPh::set_midpoint_calibration(double msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[14];
  sprintf(buf, "Cal,mid,%.2f", msg);
  Wire.print(buf);
  Wire.endTransmission();
  return status_level;
}

uint8_t AtlasPh::set_lowpoint_calibration(double msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[14];
  sprintf(buf, "Cal,low,%.2f", msg);
  Wire.print(buf);
  Wire.endTransmission();
  return status_level;
}

uint8_t AtlasPh::set_highpoint_calibration(double msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[15];
  sprintf(buf, "Cal,high,%.2f", msg);
  Wire.print(buf);
  Wire.endTransmission();
  return status_level;
}

void AtlasPh::send_query() {
  _time_of_last_query = millis();
  Wire.beginTransmission(_i2c_address);
  Wire.print("R");
  Wire.endTransmission();
  _waiting_for_response = true;
}

void AtlasPh::read_response() {
  Wire.requestFrom(_i2c_address, 20, 1);
  byte response;
  if(Wire.available()){
    response = Wire.read(); // increment buffer by a byte
  }

  // Check for failure
  if (response == 255) {
    status_level = ERROR;
    status_code = CODE_NO_RESPONSE;
    status_msg = "No response";
    _waiting_for_response = false;
  }
  else if (response == 254) {
    // Request hasn't been processed yet
  }
  else if (response == 2) {
    status_level = ERROR;
    status_code = CODE_REQUEST_FAILED;
    status_msg = "Request failed";
    _waiting_for_response = false;
  }
  else if (response == 1) {
    String string = Wire.readStringUntil(0);
    status_level = OK;
    status_code = CODE_OK;
    status_msg = "";
    _water_potential_hydrogen = string.toFloat();
    _waiting_for_response = false;
  }
  else {
    status_msg = "Unknown error";
    status_code = CODE_UNKNOWN_ERROR;
    status_level = ERROR;
  }
}

int AtlasPh::cmd(int args) {

   char read[] = "read";

   if (this->is_cmd(args, read)) {
      Serial.print(F("PH: ")); Serial.println(_water_potential_hydrogen);
      return make_int(OK);
   }

   return Module::common_cmd(args);
}

uint8_t AtlasPh::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}
