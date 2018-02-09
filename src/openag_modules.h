#ifndef OPENAG_MODULES
#define OPENAG_MODULES

int openag_modules_show_mod_status(int arg);
Module *find_module(int module_name); 
void openag_modules_show_mods(); 
void set_actuators(String splitMessages[]);
bool checkActuatorLoop();
void beginModules();
void updateLoop();
bool checkSensorLoop();
void sensorLoop();

#endif
