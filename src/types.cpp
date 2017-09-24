#include "Arduino.h"
#include <test.h>

//Provde storage and accessor functions for creating and storing types:
// character (C),
// integers (I),
// floats (F),
// strings (X).
//Note: Object pointers run from 10000 to 10100 in order to make them distinguishable from 
//      Pair pointers which run from 1 to 100.

const int TS_OFFSET = 10000;
const int TS_SZ = 100;

int op = 0;
char obj_type[TS_SZ];
String obj_store[TS_SZ];

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
      obj_type[op] = 'C';
      obj_store[op] = c;
      return op + 10000;
   }
}

char get_char(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return 0;   //error
   } else {
      if (obj_type[ptr] != 'C') {
         return 0;   //error
      } else {
         return obj_store[ptr][0];
     }
   }
}

int make_int(int a) {
   if (op < 0 || op >= TS_SZ) {
      return 0; //Error
   } else {
      op++;
      obj_type[op]  = 'I';
      obj_store[op] = (String) a;
      return op + 10000;
   }
}

int get_int(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
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

int make_str(String x) {
   if (op < 0 || op >= TS_SZ) {
      return 0; //Error
   } else {
      op++;
      obj_type[op] = 'X';
      obj_store[op] = x;
      return op + 10000;
   }
}

String get_str(int obj_ptr) {

   int ptr = obj_ptr - 10000;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return "";   //error
      Serial.println(F("Error in types.cpp: Not a string."));
   } else {
      if (obj_type[ptr] != 'X') {
         return "";   //error
      } else {
         return obj_store[ptr];
     }
   }
}

