#include "Arduino.h"
#include <test.h>
#include <types.h>
#include <pair.h>

// A vector store of pairs based upon a fixed sized vector
// Interface functions cons(int, int), car(int), and cdr(int)

int pp = 0;
const int PS_SZ = 300;

// Create a vector to hold LISP like pair values. This will allow arbitrary data structions
// (e.g. list, trees) to be created. 
// The int value stored in the carValue or cdrValue array points to the associated data. Two pointer types are currently 
// recognized by the system: Pair pointer and Type pointer. Pair pointers range from 1 to 100.  Type pointers range
// from 10001 to 10100.

int carValue[PS_SZ];
int cdrValue[PS_SZ];

// Add an element to a list and return the tail of the list.
int add_to_list(int lst, int val_to_add) {
   if (is_pair(lst) && (is_pair(val_to_add) || is_type(val_to_add))) {
      int list_tail = cons(val_to_add, nil);
      set_cdr(lst, list_tail);
      return list_tail;
   } else {
      Serial.println("Error: add_to_list -> arguments are not types.");
      return 0;
   }
}


boolean is_pair(int pair_index) {
   if (pair_index <= 0 || pair_index >= 100) {
      return false;
   } else {
      return true;
   }
}

int cons(int type1_index, int type2_index) {
   //Check for buffer overflow.
   if (pp == 100) {
      return 0;   //error - no more Vector space left.
   }

   //Move to next available pair.
   pp++;
   carValue[pp] = type1_index;
   cdrValue[pp] = type2_index;
   return pp;
}

// Returns the vector pointer to the cdr of the pair located at vector location pairIndex.
int cdr(int pairIndex) {

   if (pairIndex <= 0 || pairIndex >= 100) {
      return 0;   //error
   } else {
      return cdrValue[pairIndex];
   }
}

// Returns the vector pointer to the car of the pair located at vector location pairIndex.
int car(int pairIndex) {
   if (pairIndex <= 0 || pairIndex >= 100) {
      return 0;   //error
   } else {
      return carValue[pairIndex];
   }
}

int caar(int pairIndex) { return car(car(pairIndex)); }
int cadr(int pairIndex) { return car(cdr(pairIndex)); }
int caddr(int pairIndex) { return car(cdr(cdr(pairIndex))); }
int caadr(int pairIndex) { return car(car(cdr(pairIndex))); }
int caaddr(int pairIndex) { return car(car(cdr(cdr(pairIndex)))); }

void set_cdr(int lst, int item) {
   cdrValue[lst] = item;
}

void set_car(int lst, int item) {
   carValue[lst] = item;
}

void test_cons() {
   pp = 0;
   assert_int_equals(F("pair.cpp"), cons(2, 3), 1);
   assert_int_equals(F("pair.cpp"), carValue[1], 2);
   assert_int_equals(F("pair.cpp"), cdrValue[1], 3); 
}

void test_pair() {

   test_cons();

   int test2 = cons(cons(make_char('C'), nil),nil);

   assert_char_equals(F("pair.cpp"), 'C', get_char(car(car(test2))));
}
