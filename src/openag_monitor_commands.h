#ifndef OPENAG_MONITOR_COMMANDS
#define OPENAG_MONITOR_COMMANDS

int find_module_function(int function_name);
int apply_module_function(int func, int args);
int openag_help(int args);
int show_open_ag_module_status(int status_code);

#endif
