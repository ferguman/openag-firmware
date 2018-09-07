#ifndef DFROBOT_EC
#define DFROBOT_EC

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <openag_module.h>

class SensorEc : public Module {

    public:
        SensorEc(int probe_pin, int pwr_relay_ctrl_pin);
	uint8_t begin();
	uint8_t update();
        uint8_t set_cmd(const char *cmd);
	int cmd(int args);
	void print_readings_as_csv();
	float get_ec();

    private:
	int probe_pin_;
	int pwr_relay_ctrl_pin_;
	float ec_;
	float ec_calibration_offset = 0;
	float ec_calibration_gain = 1;
};

#endif
