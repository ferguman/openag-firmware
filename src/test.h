#ifndef TEST
#define TEST

// Meant to be public
void assert_int_equals(String test_name, int a, int b);
void assert_int_not_equals(String test_name, int a, int b);
void assert_char_equals(String test_name, char a, char b);
void assert_str_equals(String test_name, String a, String b);
int run_tests(int arg_list);

#endif
