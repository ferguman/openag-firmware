/** 
 *  \file sensor_dht22.cpp
 *  \brief Sensor module for air temperature and humidity.
 *  \details See sensor_dht22.h for details.
 *  Last edited on 20180730 by ferguman for openag firmware compatibility
 */
#include "dht22.h"


SensorDht22::SensorDht22(int pin){

  pin_ = pin;
  count_ = COUNT;

  first_reading_ = true;

  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
}

uint8_t SensorDht22::begin() {
  pinMode(pin_, INPUT);
  digitalWrite(pin_, HIGH);
  last_read_time_ = 0;

  return status_level;
}

float SensorDht22::get_air_temperature() {
   return temperature_raw_;
}

float SensorDht22::get_air_humidity() {
   return humidity_raw_;
}

uint8_t SensorDht22::update() {
  getRawSensorData();

  return status_level;
}

void SensorDht22::getRawSensorData() {
  humidity_raw_ = 0;
  temperature_raw_ = 0;
  if (read()) {
    humidity_raw_ = data[0];
    humidity_raw_ *= 256;
    humidity_raw_ += data[1];
    humidity_raw_ /= 10;
    
    temperature_raw_ = data[2] & 0x7F;
    temperature_raw_ *= 256;
    temperature_raw_ += data[3];
    temperature_raw_ /= 10;
    if (data[2] & 0x80) {
      temperature_raw_ *= -1;
    }    
  }
}

boolean SensorDht22::read() {

  uint8_t last_state = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long current_time;

  digitalWrite(pin_, HIGH);
  delay(2); // old delay time was 250

  current_time = millis();
  if (current_time < last_read_time_) {
    // ie there was a rollover
    last_read_time_ = 0;
  }
  if (!first_reading_ && ((current_time - last_read_time_) < 2000)) {
    return true; // return last correct measurement
    // delay(2000 - (currenttime - _lastreadtime));
  }
  first_reading_ = false;
  last_read_time_ = millis();

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
  // now pull it low for ~20 milliseconds
  // Step 1 - pulse SDA digital line low for at least 800 usec (typ:1 ms)
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
  delay(20);
  //cli();
  digitalWrite(pin_, HIGH);
  delayMicroseconds(40);
  pinMode(pin_, INPUT);

  // read in timings
  // MAXTIMINGS = 85
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (digitalRead(pin_) == last_state) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    last_state = digitalRead(pin_);

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > count_)
        data[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits and that the checksum matches
  if ((j >= 40) && 
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {

     status_level = OK;
     status_code =  CODE_OK;    
     status_msg = "";
     return true;

  } else {

     status_level = ERROR;
     status_code =  CODE_CHECKSUM_FAILURE;    
     status_msg = "checksum or timing failure";
     return false;
  }
}

int SensorDht22::cmd(int args) {

   char temp_hum[] = "read";

   if (this->is_cmd(args, temp_hum)) {
      Serial.print(F("Air Temp.: ")); Serial.println(temperature_raw_);
      Serial.print(F("Humidity: ")); Serial.println(humidity_raw_);
      return make_int(status_level);
   }

   return Module::common_cmd(args);
}

/*- This function isn't used anywhere?
String SensorDht22::floatToString( double val, unsigned int precision) {
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)
  String str = "";
  str += int(val);  //prints the int part
  str += "."; // print the decimal point
  unsigned int frac;
  if(val >= 0) {
    frac = (val - int(val)) * precision;
  }
  else {
    frac = (int(val)- val ) * precision;
  }
  str += int(frac);
  return str;
} 
*/

