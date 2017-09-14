#ifndef TYPES
#define TYPES

// Meant to be public
boolean is_type(int type_ptr);
int make_int(int a);
int make_char(char c);
int get_char(int obj_ptr);
int make_str(String x);
int get_int(int obj_ptr);
const int nil = 0;

#endif
