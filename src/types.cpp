#include "Arduino.h"
#include <test.h>

//Provde storage and accessor functions for creating and storing types:
// character (C),
// integers (I),
// floats (F),
// strings (X).
//Note: Object pointers run from 10000 to 10100 in order to make them distinguishable from 
//      Pair pointers which run from 1 to 100.

// TBD Need to factor out usage of strings because using them will fill up the heap and 
//     crash the system.
const int TS_OFFSET = 10000;
const int TS_SZ = 1000;

int op = 0;
//char obj_type[TS_SZ];
//String obj_store[TS_SZ];
char obj_store[TS_SZ];

void clear_types() {
   op = 0;
}

boolean is_type(int type_ptr) {
   if ((type_ptr - TS_OFFSET) >= 1 && (type_ptr - TS_OFFSET) < TS_SZ) {
      return true;
   } else {
      return false;
   }
}

int make_char(char c) {
   if (op < 0 || op >= TS_SZ) {
      Serial.println("Error in make_char. Out of memory.");
      return 0; //Error
   } else {
      op++;
      //obj_type[op] = 'C';
      obj_store[op] = 'C';
      op++;
      obj_store[op] = c;
      return op + 10000;
   }
}

char get_char(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return 0;   //error
   } else {
      if (obj_store[ptr] != 'C') {
         return 0;   //error
      } else {
         return obj_store[ptr+1];
     }
   }
}

// Integers are limited to +/- 10,000.  Each integer burns 7 bytes in the
// object store.
int make_int(int a) {

   if (op < 0 || op >= TS_SZ) {
      return 0; //Error
   }

   if (a < -10000 || a > 10000) {
      Serial.println(F("Integer Overflow. Integers must be less than 10,000 in magnitude."));
      return 0;
   }

   op++;
   obj_store[op] = 'I';
   char int_a[7];
   itoa(a, int_a, 10);
   for (int i=0; i<7; i++) {
      op++;
      obj_store[op] = int_a[i];
   }
   // It's redundant but null terminate the string.
   obj_store[6] = 0;
   return op + 10000;
}

int get_int(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return 0;   //error
   } else {
      if (obj_store[ptr] != 'I') {
         return 0;   //error
      } else {
         return atoi(obj_store + ptr + 1) ;
     }
   }
}

int make_str(char * str) {
   if (op < 0 || op >= TS_SZ) {
      return 0; //Error
   } else {
      op++;
      obj_store[op] = 'X';
      strcpy(str, obj_store + 1);
      op = op + strlen(str) + 1;
      return op + 10000;
   }
}

String get_str(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return "";   //error
      Serial.println(F("Error in types.cpp: Not a string."));
   } else {
      if (obj_store[ptr] != 'X') {
         return "";   //error
      } else {
         return obj_store + ptr + 1;  
     }
   }
}

void print_result(int ptr) {
   Serial.println(F("print_result is not implemented."));
}

int test_types() {

   return 0;

}

