#ifndef OPENAG_MODULE
#define OPENAG_MODULE

#include <pair.h>
#include <types.h>

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

static const uint8_t OK = 0;
static const uint8_t WARN = 1;
static const uint8_t ERROR = 2;

static const uint8_t CODE_OK = 0;
static const uint8_t NO_SET_CMD = 1;

class Module {
  public:
    // Public methods
    virtual ~Module() {}; // destructor
    virtual uint8_t begin() = 0;
    virtual uint8_t update() = 0;
    virtual void print_readings_as_csv() = 0;
    virtual uint8_t set_cmd(const char *cmd) = 0;

    uint8_t status_level;
    String status_msg;
    uint8_t status_code;

    virtual int cmd(int args) = 0;

//  private:
    int show_status();
    boolean is_cmd(int args, char *cmd);
    int common_cmd(int args);
};

#endif
