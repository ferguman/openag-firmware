#include "Arduino.h"
#include <process_message.h>
#include <openag_module.h>
#include <types.h>
#include <pair.h>
#include <openag_modules.h>

// Set AUTO_START equal to false to disable the invocation of the
// Food Computer module begin methods in the Arduino setup() function.
boolean AUTO_START = true;

// The following flag turns the Food Computer control loop on or off.
boolean fc_loop_on = true;

// Message string
String message = "";
bool stringComplete = false;
const int COMMAND_LENGTH = 18; // status + num_actuators
const unsigned int MESSAGE_LENGTH = 500;

// Main logic
void actuatorLoop();

// Utility functions
void send_invalid_message_length_error(String msg);
void resetMessage();
int split(String messages, String* splitMessages,  char delimiter=',');
int procMsg();
boolean fc_cmd_detected();

// These functions are defined in the Arduino.h and are the framework.
void setup() {

  Serial.begin(115200);

  if (!fc_loop_on) {Serial.println("Open Ag Serial Monitor Starting.");}

  while(!Serial){
    // wait for serial port to connect, needed for USB
  }

  message.reserve(MESSAGE_LENGTH);

  if (AUTO_START) {
     beginModules();
  }
}

void loop() {

   if (fc_loop_on == true) {
      updateLoop();
   }

   // If we have not received a message, then do nothing.  This lets our ROS
   // node control the message traffic.  For every message sent to this arduino
   // code, one is sent back.
   if(!stringComplete){
      return;
   }

   // If command is not a food computer actuator command or the food computer loop is off then
   // interpret the incoming message as a serial command.
   if (!fc_cmd_detected() || !fc_loop_on) { 
      procMsg();
      return;
   }

   // An actuator command has been detected and the food computer loop is enabled so
   // check the actuators and sensors, set the actuators as per the received command and return
   // all the sensor readings.
   //
   checkActuatorLoop();
   actuatorLoop();

   bool allSensorSuccess = checkSensorLoop();

   if(allSensorSuccess){
      sensorLoop();
   }
}

// Runs inbetween loop()s, just takes any input serial to a string buffer.
// Runs as realtime as possible since loop has no delay() calls. (It shouldn't!)
// Note: the internal buffer in the Serial class is only 64 bytes!
//
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

int procMsg() {

   //If no message has arrived then do nothing.
   if(!stringComplete){
      return 0;
   }
  
   //Process message.
   int result = process_message(&message);

   // We've already used this message
   resetMessage();

   return result;
}

void actuatorLoop(){

  // If serial message, actuate based on it.
  if(!stringComplete){
    return;
  }

  //Refactor to use C style strings.  Avoid C++ objects in this tinker toy system.
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

  set_actuators(splitMessages);
}

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

bool any(bool *all){
  int length = sizeof(all)/sizeof(all[0]);
  for(int i=0; i < length; i++){
    if(all[i]){
      return true;
    }
  }
  return false;
}

boolean fc_cmd_detected() {
   return (message.substring(0,2).equals("0,"));
}
