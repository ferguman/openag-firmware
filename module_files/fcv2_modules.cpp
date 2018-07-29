#include "Arduino.h"
#include <process_message.h>
#include <pair.h>
#include <types.h>

// Put includes for all the system's modules here.
//
#include <openag_am2315.h>
#include <openag_ds18b20.h>
#include <openag_mhz16.h>
#include <openag_atlas_ph.h>
#include <openag_atlas_ec.h>

#include <openag_binary_sensor.h>
#include <openag_binary_actuator.h>
#include <openag_pwm_actuator.h>
#include <openag_doser_pump.h>
#include <openag_pulse_actuator.h>
#include <openag_tone_actuator.h>

// Internal function declarations
bool str2bool(String str);
bool checkModule(Module &module, String name);
bool beginModule(Module &module, String name);
void sendModuleStatus(Module &module, String name);

// Put instantiations of all the system's modules here.  
//
// Sensors
//
Am2315 am2315_1;                                   // Air Temp and Humidity, I2C, Addr = 92
Ds18b20 ds18b20_1(5);                              // Water Temp, One Wire
MHZ16 mhz16_1(77);                                 // CO2, I2C, Addr = 77
AtlasPh atlas_ph_1(99);                            // PH, I2C, Addr = 99
AtlasEc atlas_ec_1(100);                           // EC, I2C, Addr = 100
BinarySensor water_level_sensor_high_1(3, false);
BinarySensor water_level_sensor_low_1(4, false);
//
// Actuator Instances. Sorted by pin number.
//
ToneActuator chiller_compressor_1(9, false, 140, -1);
DoserPump pump_1_nutrient_a_1(28, true);
DoserPump pump_2_nutrient_b_1(29, true);
PulseActuator pump_3_ph_up_1(30, true);
PulseActuator pump_4_ph_down_1(31, true);
BinaryActuator pump_5_water_1(32, true, 10000);
BinaryActuator chiller_fan_1(33, true, 10000);
BinaryActuator chiller_pump_1(34, true, 10000);
BinaryActuator heater_core_2_1(35, true, 10000);
BinaryActuator air_flush_1(36, true, 10000);
BinaryActuator water_aeration_pump_1(37, true, 10000);
BinaryActuator water_circulation_pump_1(38, true, 10000);
BinaryActuator chamber_fan_1(39, true, 10000);
PwmActuator led_blue_1(40, true, 0);
PwmActuator led_white_1(41, true, 0);
PwmActuator led_red_1(42, true, 0);
BinaryActuator heater_core_1_1(43, true, 10000);

// Put pointers to instantiated modules into the mod_ptr_array.
//
const uint8_t NMODS = 24;
const uint8_t ACTUATOR_OFFSET = 7;

Module *mod_ptr_array[] = {
   &am2315_1,                   //0 - Put sensors at the head of the list.
   &ds18b20_1,                  //1
   &mhz16_1,                    //2
   &atlas_ph_1,                 //3
   &atlas_ec_1,                 //4
   &water_level_sensor_high_1,  //5
   &water_level_sensor_low_1,   //6
   &air_flush_1,                //7 - First Actuator in the list.
   &chamber_fan_1,              //8
   &chiller_fan_1,              //9
   &led_blue_1,                 //10
   &led_white_1,                //11
   &led_red_1,                  //12
   &chiller_compressor_1,       //13
   &chiller_pump_1,             //14
   &heater_core_2_1,            //15
   &water_aeration_pump_1,      //16
   &water_circulation_pump_1,   //17
   &heater_core_1_1,            //18
   &pump_1_nutrient_a_1,        //19
   &pump_2_nutrient_b_1,        //20
   &pump_5_water_1,             //21
   &pump_3_ph_up_1,             //22
   &pump_4_ph_down_1            //23
};

// Put the name of the modules in the mname_array data structure.
//
//This array holds the names of all the modules.
const char *mname_array[NMODS] = {
   "air_temp_hum",
   "water_temp",
   "co2",
   "ph",
   "ec",
   "water_high",
   "water_low",
   "air_flush",
   "chamber_fan",
   "chiller_fan",
   "blue_led",
   "white_led",
   "red_led",
   "chiller_comp",
   "chiller_pump",
   "heater_core_2",
   "water_aeration",
   "water_circ_pump",
   "heater_core_1",
   "nut_pump_1",
   "nut_pump_2",
   "water_pump",
   "ph_up",
   "ph_down"
};

// Put lines for all the FC V2 actuators below. If you modify this function
// then you must make sure the calling code in src.cpp is updated to send 
// the correct number of actuator messages in the correct order, furthermore you can
// no longer use the OpenAg ROS code that communicates with the firmware without
// modifying it to account for the new sensor and actuator set.
//
// The OpenAg FC V2 control loop calls this method in order to send actuator
// commands.  
//
void set_actuators(String splitMessages[]) {

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

// Put checkModule(...) calls in this function for each Actuator.
//
// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Actuators.
//
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

// Put checkModule(...) calls in this function for each Sensor.
//
// The Openag V2 FC calls this function (from src.cpp) in order to check all the 
// Sensors.
//
bool checkSensorLoop() {

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

// Put code here for each Sensor.  This routine is invoked (via the Arduino serial port and src.cpp) by the 
// OpenAG FC V2 Controller (.e.g. Raspberry PI Model 3B) ROS Node and thus changes here will make the code 
// incompatible with the current Controller code.
// 
// Prints the data in CSV format via serial.
// Columns: status,hum,temp,co2,water_temperature,water_low,water_high,ph,ec
//
void sensorLoop(){

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
// Put a call to beginModule here for each Module.
//
void beginModules() {

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
}

// Put an update method call here for each Module.
//
// Runs the update loop - The update method is what causes Sensors to take new readings.  The update
// method causes actuators to invoke their built-in control loop and change the actuator outputs as 
// per the control loop's logic.
//
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

bool beginModule(Module &module, String name){
  bool status = module.begin() == OK;
  if(!status){
    sendModuleStatus(module, name);
  }
  return status;
}
void sendModuleStatus(Module &module, String name){
  Serial.print(module.status_level); Serial.print(',');
  Serial.print(name);  Serial.print(',');
  Serial.print(module.status_code);  Serial.print(',');
  Serial.print(module.status_msg);   Serial.print('\n');
  Serial.flush();
}

//If a the module does not have OK status then print it's 
//status.
//
bool checkModule(Module &module, String name){

  bool status = module.status_code == OK;

  if(!status) {
    sendModuleStatus(module, name);
  }
  return status;
}

// Look for a module class that matches the name given.  If one is found then 
// return the class.
//
Module *find_module(int module_name) {

   char *mn = get_str(module_name); 

   if (!mn){
      return 0;
   }
  
   // Look for the module 
   for (int i = 0; i < NMODS; i++) {
      if (strcmp(mn, mname_array[i]) == 0) {
         return mod_ptr_array[i];
      }
   }

   return 0;
}

void openag_modules_show_mods() {

   Serial.println(F("The following modules are installed on this system:"));

   for (int i=0; i < NMODS; i++) {

      if (i == 0) {
         Serial.println(F("SENSORS:"));
      }

      if (i == ACTUATOR_OFFSET) {
         Serial.println(F(""));
         Serial.println(F("ACTUATORS:"));
      }

      Serial.println(mname_array[i]);
   }

}

int openag_modules_show_mod_status(int status_code) {

   if (status_code == OK) {
      Serial.println(F("Module response: OK"));
      return -1;      
   }
   if (status_code == WARN) {
      Serial.println(F("Module response: WARN"));
      return -1;      
   }
   if (status_code == ERROR) {
      Serial.println(F("Module response: ERROR"));
      return -1;      
   }

   Serial.println(F("Error: Unknown module status code."));
   return 0;
}

bool str2bool(String str){
  str.toLowerCase();
  return str.startsWith("true");
}
