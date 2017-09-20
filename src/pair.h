#ifndef PAIR
#define PAIR

// Meant to be public
boolean is_pair(int pair);
int add_to_list(int lst, int val_to_add);
int cons(int pair1, int pair2);
int cdr(int pairIndex);
void test_pair();
int caar(int pairIndex);
int cadr(int pairIndex);
int caddr(int pairIndex);
int caadr(int pairIndex);
int caaddr(int pairIndex);
int car(int pairIndex);

// Meant to be private
void set_cdr(int lst, int list_tail);

#endif
