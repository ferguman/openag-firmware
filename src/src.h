#ifndef SRC
#define SRC

//FC Version 2 - 7 sensors
extern Am2315 am2315_1;
extern Ds18b20 ds18b20_1;
extern MHZ16 mhz16_1;
extern AtlasPh atlas_ph_1;
extern AtlasEc atlas_ec_1;
extern BinarySensor water_level_sensor_high_1;
extern BinarySensor water_level_sensor_low_1;

//FC Version 2 - 17 actuators
extern BinaryActuator air_flush_1;
extern BinaryActuator chamber_fan_1;
extern BinaryActuator chiller_fan_1;
extern PwmActuator led_blue_1;
extern PwmActuator led_white_1;
extern PwmActuator led_red_1;

extern BinaryActuator pump_5_water_1;
extern BinaryActuator chiller_pump_1;
extern BinaryActuator heater_core_2_1;
extern BinaryActuator water_aeration_pump_1;
extern BinaryActuator water_circulation_pump_1;
extern BinaryActuator heater_core_1_1;

extern DoserPump pump_1_nutrient_a_1;
extern DoserPump pump_2_nutrient_b_1;
extern PulseActuator pump_3_ph_up_1;
extern PulseActuator pump_4_ph_down_1;
extern ToneActuator chiller_compressor_1;

#endif
