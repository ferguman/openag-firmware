#ifndef OPENAG_PULSE_ACTUATOR_H
#define OPENAG_PULSE_ACTUATOR_H

#include "Arduino.h"
#include <openag_module.h>

class PulseActuator : public Module {
  public:
    // Constructor
    PulseActuator(int pin, bool is_active_low, int pulse_ms=500, int update_ms=4000);

    // Public functions
    uint8_t begin();
    uint8_t update();
    void print_readings_as_csv();
    int cmd(int args);
    uint8_t set_cmd(const char *cmd);
    //- uint8_t set_cmd(bool cmd);

  private:
    // Private functions
    uint8_t bool2command(bool isHigh);
    uint8_t set(bool cmd);

    // Private variables
    int _pin;
    bool _is_active_low;
    bool _state; // Is ON? or OFF?
    uint32_t _pulse_ms;
    uint32_t _update_ms;
    uint32_t _last_cmd;
};

#endif
