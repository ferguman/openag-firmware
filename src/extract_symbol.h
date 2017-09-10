#ifndef EXTRACT_HEADER
#define EXTRACT_HEADER

// Meant to be public
int extract_sym(String *str, int cur_pos);
void test_extract_symbol();

// Meant to be private
int extract_sym_iter(String *str, unsigned int start_pos, unsigned int cur_pos);
boolean is_sym_char(String *str, int char_pos);

#endif
