#include "Arduino.h"
#include <test.h>

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

int cons(int pair1, int pair2) {
   //Check for buffer overflow.
   if (pp == 100) {
      return 0;   //error - no more Vector space left.
   }

   //Move to next avialble pair.
   pp++;
   carValue[pp] = pair1;
   cdrValue[pp] = pair2;
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

void test_cons() {
   pp = 0;
   assert_int_equals("Cons1", cons(2, 3), 1);
   assert_int_equals("Cons2", carValue[1], 2);
   assert_int_equals("Cons3", cdrValue[1], 3); 
}

void test_pair() {
   test_cons();
}
