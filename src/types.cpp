#include "Arduino.h"

//Provde storage and accessor functions for creating and storing types: integers (I), symbols (S), strings (X), etc.
//Note: Object pointers run from 10000 to 10100 in order to make them distinguishable from 
//      Pair pointers which run from 1 to 100.

const int TS_SZ = 100;

int op = 0;
char obj_type[TS_SZ];
String obj_store[TS_SZ];

boolean is_type(int type_ptr) {
   if (type_ptr >= 1 && type_ptr < TS_SZ) {
      return true;
   } else {
      return false;
   }
}

int make_int(int a) {
   if (op <= 0 || op >= 100) {
      return 0; //Error
   } else {
      op++;
      obj_type[op]  = 'I';
      obj_store[op] = (String) a;
      return op + 10000;
   }
}

int make_char(char c) {
   if (op <= 0 || op >= 100) {
      return 0; //Error
   } else {
      op++;
      obj_type[op] = 'C';
      obj_store[op] = c;
      return op + 10000;
   }
}

int make_str(String x) {
   if (op <= 0 || op >= 100) {
      return 0; //Error
   } else {
      op++;
      obj_type[op] = 'X';
      obj_store[op] = x;
      return op + 10000;
   }
}

int get_int(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=100) {
      return 0;   //error
   } else {
      if (obj_type[ptr] != 'I') {
         return 0;   //error
      } else {
         //const char *cstr = str.c_str();j
         return atoi(obj_store[ptr].c_str());
     }
   }
}
