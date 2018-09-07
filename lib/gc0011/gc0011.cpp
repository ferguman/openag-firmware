/** 
 *  \file sensor_gc0011.cpp
 *  \brief Sensor module for air co2, temperature, and humidity.
 *  \details See sensor_gc0011.h for details.
 *
 *  Temperature and Humidity are factory installed options thus not all sensors will be able to 
 *  read these two attributes.  See the data sheet at:
 *  http://www.co2meters.com/Documentation/Manuals/Manual-GSS-Sensors.pdf for more details.
 *
 */
#include "gc0011.h"

SensorGc0011::SensorGc0011(int rx_pin, int tx_pin) {
  rx_pin_ = rx_pin;
  tx_pin_ = tx_pin;
  timeout_ = 40; // milliseconds
}

uint8_t SensorGc0011::begin() { 

  /* -
  if (NULL != ss_) {
    delete ss_;
  }
  */

  ss_ = new SoftwareSerial(rx_pin_,tx_pin_);
  ss_->begin(9600);
  delay(100);
  String message;
  sendMessage("K 2");             //set sensor to polling mode
  message = receiveMessage();
  sendMessage("A 32");            //set sensor to default digital filtering value
  message = receiveMessage();
  ss_->end();

  return status_level;
}

uint8_t SensorGc0011::update() {

  if (millis() - _time_of_last_reading > _min_update_interval) {
      getSensorData();
      _time_of_last_reading = millis();
  }

  return status_level;
}

uint8_t SensorGc0011::set_cmd(const char *cmd) {

   status_code = NO_SET_CMD;
   status_level = ERROR;

   return status_level;
}

float SensorGc0011::get_co2() {
   return co2_;
}
float SensorGc0011::get_temperature() {
   return temperature_;
}
float SensorGc0011::get_humidity() {
   return humidity_;
}

void SensorGc0011::getSensorData() {

  // Reset Values
  co2_ = 0;
  temperature_ = 0;
  humidity_ = 0;
  
  // Open Connection
  ss_->begin(9600);

  // Get CO2
  String message;
  sendMessage("Z");
  message = receiveMessage();
  if(message[1] == 'Z') {
    co2_ = (float)(message.substring(3,8).toInt());
    co2_ = round(co2_/10)*10;
  }

  if (TEMPERATURE_AND_HUMIDITY_ENABLED) {
	  // Get Temperature
	  sendMessage("T");
	  message = receiveMessage();
	  if (message[1] == 'T') {
	    temperature_ = 0.1*(float)(message.substring(3,8).toInt()-1000);
	  }
	  
	  // Get Humidity
	  sendMessage("H");
	  message = receiveMessage();
	  if (message[1] == 'H') {
	    humidity_ = 0.1*(float)(message.substring(3,8).toInt());
	  }
   }

  // Close Connection
  ss_->end();
}

void SensorGc0011::sendMessage(String message) {
  message += "\r\n";
  ss_->print(message);
}

String SensorGc0011::receiveMessage(void) {
  String message="";
  char incoming_char;
  uint32_t start_time = millis();
  while (millis() - start_time < timeout_){
    if (ss_->available()) {
      incoming_char = ss_->read();
      message += incoming_char;
      if (incoming_char == '\n') {
        break;
      }
    }
  }
  return message;
}

void SensorGc0011::print_readings_as_csv() {

    Serial.print(co2_);

    if (TEMPERATURE_AND_HUMIDITY_ENABLED == true) {
        Serial.print(',');
        Serial.print(temperature_); Serial.print(',');
        Serial.print(humidity_);
    }
}

int SensorGc0011::cmd(int args) {

   char read_cmd[] = "read";
   char serial_cmd[] = "ser";
   char help_cmd[] = "help";

   if (this->is_cmd(args, read_cmd)) {

      Serial.print(F("CO2: ")); Serial.println(co2_);
      
      if (TEMPERATURE_AND_HUMIDITY_ENABLED == true) {
          Serial.print(F("Humidity: ")); Serial.println(humidity_);
          Serial.print(F("Temperature: ")); Serial.println(temperature_);
      }

      return make_int(status_level);
   }

   if (this->is_cmd(args, help_cmd)) {
       Serial.println(F("(c 'mod_name 'ser \"command\") -> Send a command to the sensor and then display the result: e.g. (c 'co2 'ser \"Z\")."));
       Serial.println(F("                                See the GSS Sensor User's Manual for information on available commands."));

       return make_int(status_level);
   }

   // TBD - Not sure how Arduino tasks switches.  Is it possible the main loop is in serial read/write loop when
   // this routine is called? 
   if (this->is_cmd(args, serial_cmd)) {
       
       Serial.println("Serial command detected");
       Serial.println(get_str(caddr(args)));
       
       ss_->begin(9600);
       sendMessage(get_str(caddr(args)));
       Serial.println(receiveMessage());
       ss_->end();

       return make_int(status_level);
   }

   return Module::common_cmd(args);
}
