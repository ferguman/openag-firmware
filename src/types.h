#ifndef TYPES
#define TYPES

// Meant to be public
boolean is_type(int type_ptr);
void clear_types();
int make_int(int a);
int make_char(char c);
char get_char(int obj_ptr);
int make_str(char *x);
char *get_str(int obj_ptr);
int get_int(int obj_ptr);
const int nil = 0;
void print_result(int ptr); 
void test_types();

#endif
