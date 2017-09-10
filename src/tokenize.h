#ifndef TOKENIZE
#define TOKENIZE

// Meant to be public
int tokenize(String *input);

// Meant to be private
int token_iter(String *str, unsigned int cur_pos, int token_list);

#endif
