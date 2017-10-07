#include "Arduino.h"
#include <openag_module.h>
#include <process_message.h>

//New stuff
#include <src.h>

Am2315 am2315_1;
Ds18b20 ds18b20_1(5);

// These functions are defined in the Arduino.h and are the framework.
void setup() {

  Serial.begin(9600);
  Serial.println("Open Ag Serial Monitor Starting.");
  while(!Serial){
    // wait for serial port to connect, needed for USB
  }
  message.reserve(MESSAGE_LENGTH);

  beginModule(am2315_1, "AM2315 #1");
  beginModule(ds18b20_1, "DS18B20 #1");

/*
  // Begin sensors
  beginModule(am2315_1, "AM2315 #1");
  beginModule(mhz16_1, "MHZ16 #1");
  beginModule(ds18b20_1, "DS18B20 #1");
  beginModule(water_level_sensor_low_1, "Water Level Low sensor");
  beginModule(water_level_sensor_high_1, "Water Level High sensor");
  beginModule(atlas_ph_1, "Atlas pH #1");
  beginModule(atlas_ec_1, "Atlas EC #1");

  // Begin Actuators
  beginModule(pump_1_nutrient_a_1, "Pump 1, Nutrient A");
  beginModule(pump_2_nutrient_b_1, "Pump 2, Nutrient B");
  beginModule(pump_3_ph_up_1, "Pump 3, pH Up");
  beginModule(pump_4_ph_down_1, "Pump 4, pH Down");
  beginModule(pump_5_water_1, "Pump 5, Water");
  beginModule(chiller_fan_1, "Chiller Fan");
  beginModule(chiller_pump_1, "Chiller Pump");
  beginModule(heater_core_2_1, "Heater core #2");
  beginModule(air_flush_1, "Air Flush");
  beginModule(water_aeration_pump_1, "Water Aeration Pump");
  beginModule(water_circulation_pump_1, "Water Circulation Pump");
  beginModule(chamber_fan_1, "Chamber Circulation Fan");
  beginModule(led_blue_1, "LED Blue");
  beginModule(led_white_1, "LED White");
  beginModule(led_red_1, "LED Red");
  beginModule(heater_core_1_1, "Heater Core #1");
  beginModule(chiller_compressor_1, "Chiller Compressor #1");
*/
}

void loop() {

/*
  updateLoop();
*/

  // If we have not received a message, then do nothing.  This lets our ROS
  // node control the message traffic.  For every message sent to this arduino
  // code, one is sent back.
  if(! stringComplete){
    return;
  }

  procMsg();

/*
  checkActuatorLoop();
  actuatorLoop();
*/

/*
  bool allSensorSuccess = checkSensorLoop();
  if(allSensorSuccess){
    sensorLoop();
  }
*/

}

// Runs inbetween loop()s, just takes any input serial to a string buffer.
// Runs as realtime as possible since loop has no delay() calls. (It shouldn't!)
// Note: the internal buffer in the Serial class is only 64 bytes!
void serialEvent() {
  if(stringComplete){
    resetMessage();
  }
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString but first check for potential overflow:
    // (this can happen if a few partial lines are received sequentially without newlines)
    if (message.length() == (MESSAGE_LENGTH - 2)) {
      message += '\n'; // 1 byte add + null terminator makes the full message length
      stringComplete = true;
      return;
    }
    message += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      return;
    }
  }
}

void procMsg() {

  // If no message has arrived then do nothing.
  if(! stringComplete){
    return;
  }
  
   //Process message.;
   process_message(&message);


  // We've already used this message
  resetMessage();
}

/*
void actuatorLoop(){
  // If serial message, actuate based on it.
  if(! stringComplete){
    return;
  }

  String splitMessages[COMMAND_LENGTH];
  for(int i = 0; i < COMMAND_LENGTH; i++){
    splitMessages[i] = "";
  }
  int command_count = split(message, splitMessages);
  if( command_count != COMMAND_LENGTH ){
    //don't send the error message back, it happens too frequently and is 
    //   more of a debugging message at this point.  2017-08-02 rbaynes.
    // String warn = message;
    // warn += " comma counts: ";
    // warn += command_count;
    // warn += " != ";
    // warn += COMMAND_LENGTH;
    // send_invalid_message_length_error(warn);
    splitMessages[0] = "2";
  }

  // We've already used this message
  resetMessage();

  if(splitMessages[0] != "0"){
    return;
  }
  pump_1_nutrient_a_1.set_cmd(splitMessages[1].toFloat());        // DoserPump float flow_rate
  pump_2_nutrient_b_1.set_cmd(splitMessages[2].toFloat());        // DoserPump float flow_rate
  pump_3_ph_up_1.set_cmd(str2bool(splitMessages[3]));             // PulseActuator bool
  pump_4_ph_down_1.set_cmd(str2bool(splitMessages[4]));           // PulseActuator bool
  pump_5_water_1.set_cmd(str2bool(splitMessages[5]));             // BinaryActuator bool
  chiller_fan_1.set_cmd(str2bool(splitMessages[6]));              // BinaryActuator bool
  chiller_pump_1.set_cmd(str2bool(splitMessages[7]));             // BinaryActuator bool
  heater_core_2_1.set_cmd(str2bool(splitMessages[8]));            // BinaryActuator bool
  air_flush_1.set_cmd(splitMessages[9].toFloat());                // BinaryActuator float CFM 0 - 12ish
  water_aeration_pump_1.set_cmd(str2bool(splitMessages[10]));     // BinaryActuator bool
  water_circulation_pump_1.set_cmd(str2bool(splitMessages[11]));  // BinaryActuator bool
  chamber_fan_1.set_cmd(str2bool(splitMessages[12]));             // BinaryActuator bool
  led_blue_1.set_cmd(splitMessages[13].toFloat());                // PwmActuator float 0-1
  led_white_1.set_cmd(splitMessages[14].toFloat());               // PwmActuator float 0-1
  led_red_1.set_cmd(splitMessages[15].toFloat());                 // PwmActuator float 0-1
  heater_core_1_1.set_cmd(str2bool(splitMessages[16]));           // BinaryActuator bool
  chiller_compressor_1.set_cmd(str2bool(splitMessages[17]));      // ToneActuator bool on/off
}
*/

/*
// Run the update loop
void updateLoop(){
  pump_1_nutrient_a_1.update();
  pump_2_nutrient_b_1.update();
  pump_3_ph_up_1.update();
  pump_4_ph_down_1.update();
  pump_5_water_1.update();
  chiller_fan_1.update();
  chiller_pump_1.update();
  heater_core_2_1.update();
  air_flush_1.update();
  water_aeration_pump_1.update();
  water_circulation_pump_1.update();
  chamber_fan_1.update();
  led_blue_1.update();
  led_white_1.update();
  led_red_1.update();
  heater_core_1_1.update();
  chiller_compressor_1.update();

  am2315_1.update();
  mhz16_1.update();
  ds18b20_1.update();
  water_level_sensor_low_1.update();
  water_level_sensor_high_1.update();
  atlas_ph_1.update();
  atlas_ec_1.update();
}
*/

/*
bool checkActuatorLoop(){
  bool allActuatorSuccess = true;

  allActuatorSuccess = checkModule(pump_1_nutrient_a_1, "Pump 1 Nutrient A") && allActuatorSuccess;
  allActuatorSuccess = checkModule(pump_2_nutrient_b_1, "Pump 2 Nutrient B") && allActuatorSuccess;
  allActuatorSuccess = checkModule(pump_3_ph_up_1, "Pump 3 pH Up") && allActuatorSuccess;
  allActuatorSuccess = checkModule(pump_4_ph_down_1, "Pump 4 pH Down") && allActuatorSuccess;
  allActuatorSuccess = checkModule(pump_5_water_1, "Pump 5 Water") && allActuatorSuccess;
  allActuatorSuccess = checkModule(chiller_fan_1, "Chiller Fan") && allActuatorSuccess;
  allActuatorSuccess = checkModule(chiller_pump_1, "Chiller Pump") && allActuatorSuccess;
  allActuatorSuccess = checkModule(heater_core_2_1, "Heater core #2") && allActuatorSuccess;
  allActuatorSuccess = checkModule(air_flush_1, "Air Flush") && allActuatorSuccess;
  allActuatorSuccess = checkModule(water_aeration_pump_1, "Water Aeration Pump") && allActuatorSuccess;
  allActuatorSuccess = checkModule(water_circulation_pump_1, "Water Circulation Pump") && allActuatorSuccess;
  allActuatorSuccess = checkModule(chamber_fan_1, "Chamber Circulation Fan") && allActuatorSuccess;
  allActuatorSuccess = checkModule(led_blue_1, "LED Blue") && allActuatorSuccess;
  allActuatorSuccess = checkModule(led_white_1, "LED White") && allActuatorSuccess;
  allActuatorSuccess = checkModule(led_red_1, "LED Red") && allActuatorSuccess;
  allActuatorSuccess = checkModule(heater_core_1_1, "Heater Core #1") && allActuatorSuccess;
  allActuatorSuccess = checkModule(chiller_compressor_1, "Chiller Compressor #1") && allActuatorSuccess;

  return allActuatorSuccess;
}
*/

/*
bool checkSensorLoop(){
  bool allSensorSuccess = true;

  // Run Update on all sensors
  allSensorSuccess = checkModule(am2315_1, "AM2315 #1") && allSensorSuccess;
  allSensorSuccess = checkModule(mhz16_1, "MHZ16 #1") && allSensorSuccess;
  allSensorSuccess = checkModule(ds18b20_1, "DS18B20 #1") && allSensorSuccess;
  allSensorSuccess = checkModule(water_level_sensor_low_1, "Water Level Low sensor") && allSensorSuccess;
  allSensorSuccess = checkModule(water_level_sensor_high_1, "Water Level High sensor") && allSensorSuccess;
  allSensorSuccess = checkModule(atlas_ph_1, "Atlas pH #1") && allSensorSuccess;
  allSensorSuccess = checkModule(atlas_ec_1, "Atlas EC #1") && allSensorSuccess;

  return allSensorSuccess;
}
*/

/*
void sensorLoop(){
  // Prints the data in CSV format via serial.
  // Columns: status,hum,temp,co2,water_temperature,water_low,water_high,ph,ec
  Serial.print(OK);                                             Serial.print(',');
  Serial.print(am2315_1.get_air_humidity());                    Serial.print(',');
  Serial.print(am2315_1.get_air_temperature());                 Serial.print(',');
  Serial.print(mhz16_1.get_air_carbon_dioxide());               Serial.print(',');
  Serial.print(ds18b20_1.get_temperature());                    Serial.print(',');
  Serial.print(water_level_sensor_low_1.get_is_on());           Serial.print(',');
  Serial.print(water_level_sensor_high_1.get_is_on());          Serial.print(',');
  Serial.print(atlas_ph_1.get_water_potential_hydrogen());      Serial.print(',');
  Serial.print(atlas_ec_1.get_water_electrical_conductivity()); Serial.print('\n');
  // https://www.arduino.cc/en/serial/flush
  // Wait until done writing.
  Serial.flush();
}
*/

void send_invalid_message_length_error(String msg) {
  String clean_msg = msg;
  clean_msg.replace(',', '_');
  clean_msg.replace("\n", "");
  String warn = "2,Arduino,1,";
  warn += "Invalid message";
  warn += " ";
  warn += msg.length();
  warn += " bytes: |";
  warn += clean_msg;
  warn += "|\n";
  Serial.print(warn);
  Serial.flush();
}

// Resets our global string and flag.
void resetMessage() {
  for(unsigned i=0; i < MESSAGE_LENGTH; i++){
    message.setCharAt(i, '\0');
  }
  message = "";
  stringComplete = false;
}

int split(String messages, String* splitMessages,  char delimiter){
  int indexOfComma = 0;
  int chunk_count = 0;
  for(int i = 0; i < COMMAND_LENGTH; i++){
    int nextIndex = messages.indexOf(delimiter, indexOfComma+1);
    String nextMessage;

    // The first message doesn't have an initial comma, so account for that.
    if(indexOfComma == 0){
      indexOfComma = -1;
    }
    if(nextIndex == -1){
      nextMessage = messages.substring(indexOfComma+1);
    }else{
      nextMessage = messages.substring(indexOfComma+1, nextIndex);
    }
    splitMessages[i] = nextMessage;
    indexOfComma = nextIndex;
    chunk_count++;
    if(nextIndex == -1) break; // make sure to exit the loop if we've reached the last message.
  }
  if(indexOfComma != -1){
    return -1; //if there are more commas than (COMMAND_LENGTH - 1) it's a long read
  }
  return chunk_count;
}

void sendModuleStatus(Module &module, String name){
  Serial.print(module.status_level); Serial.print(',');
  Serial.print(name);  Serial.print(',');
  Serial.print(module.status_code);  Serial.print(',');
  Serial.print(module.status_msg);   Serial.print('\n');
  Serial.flush();
}


bool beginModule(Module &module, String name){
  bool status = module.begin() == OK;
  if(!status){
    sendModuleStatus(module, name);
  }
  return status;
}

/*
bool checkModule(Module &module, String name){
  bool status = module.status_code == OK;
  if(!status){
    sendModuleStatus(module, name);
  }
  return status;
}
*/

/*
bool any(bool *all){
  int length = sizeof(all)/sizeof(all[0]);
  for(int i=0; i < length; i++){
    if(all[i]){
      return true;
    }
  }
  return false;
}
*/

bool str2bool(String str){
  str.toLowerCase();
  return str.startsWith("true");
}
