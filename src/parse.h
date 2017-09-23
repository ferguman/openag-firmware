#ifndef PARSE
#define PARSE

// Meant for public use
int parse(int token_list);
void test_parse();

// Meant for internal use.
int parse_iter(int level, int parse_list, int token_list); 
int add_list_item(int pair, int item);

#endif
