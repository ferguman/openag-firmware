#ifndef PAIR
#define PAIR

// Meant to be public
boolean is_pair(int pair);
void clear_pairs();
int add_list_item(int pair, int item);
// int add_to_list(int lst, int val_to_add);
int cons(int pair1, int pair2);
int car(int pairIndex);
int cdr(int pairIndex);
void test_pair();
void set_car(int lst, int item);
int caar(int pairIndex);
int cadr(int pairIndex);
int cdar(int pairIndex);
int cddr(int pairIndex);
int cdddr(int pairIndex); 
int cadddr(int pairIndex);
int caddr(int pairIndex);
int caadr(int pairIndex);
int caaddr(int pairIndex);
int car(int pairIndex);

// Meant to be private
void set_cdr(int lst, int list_tail);

#endif
