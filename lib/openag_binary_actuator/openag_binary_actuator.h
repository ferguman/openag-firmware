#ifndef OPENAG_BINARY_ACTUATOR_H
#define OPENAG_BINARY_ACTUATOR_H

#include "Arduino.h"
#include <openag_module.h>

class BinaryActuator : public Module {
  public:
    // Constructor
    BinaryActuator(int pin, bool is_active_low, uint32_t shutoff_ms);

    // Public functions
    uint8_t begin();
    uint8_t update();
    uint8_t set_cmd(bool cmd);
    void print_readings_as_csv();
    int cmd(int args);

  private:
    // Private variables
    int _pin;
    bool _is_active_low;
    uint32_t _shutoff_ms = 10000;
    uint32_t _last_cmd;
    void show_state();
};

#endif
