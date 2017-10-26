#include <openag_am2315.h>
#include <openag_ds18b20.h>

#ifndef OPENAG_MODULES
#define OPENAG_MODULES

extern Am2315 am2315_1;
extern Ds18b20 ds18b20_1;

int find_module_function(int function_name);
int apply_module_function(int func, int args);

#endif
