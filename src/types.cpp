#include "Arduino.h"
#include <test.h>

// function signatures
float produce_float(int frac_part, int cur_numer,  int max_int_mag);

//TODO - Need to refactor each make_ function and make sure you check for buffer overflow
//       while building the types.

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
const int TS_MAX_INDEX = TS_SZ - 1;
const int MAX_INT_MAG = 10000;

int op = 0;
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

boolean is_typeof(char type_sym, int obj_ptr) {
   if (is_type(obj_ptr)) {
      if (obj_store[obj_ptr - TS_OFFSET] == type_sym) {
         return true;
      } else {
         return false;
      }
   } else {
      return false;
   }
}

boolean is_int(int obj_ptr) {return is_typeof('I', obj_ptr);}
boolean is_char(int obj_ptr) {return is_typeof('C', obj_ptr);}

/*
   int ptr = obj_ptr - TS_OFFSET;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return false; 
   } else {
      if (obj_store[ptr] == 'C') {
         return true;
      } else {
         return false;
      }
   } 
}
*/

int make_char(char c) {
   if (op < 0 || op >= TS_SZ) {
      Serial.println("Error in make_char. Out of memory.");
      return 0; //Error
   } else {
      op++;
      obj_store[op] = 'C';
      op++;
      obj_store[op] = c;
      return op + TS_OFFSET - 1;
   }
}

char get_char(int obj_ptr) {

   int ptr = obj_ptr - TS_OFFSET;

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

   if (a < - MAX_INT_MAG || a > MAX_INT_MAG) {
      Serial.println(F("Integer Overflow. Integers must be less than 10,000 in magnitude."));
      return 0;
   }

   op++;
   obj_store[op] = 'I';
   int int_ptr = op;
   itoa(a, &obj_store[op+1], 10);
   op = op + 7;
   return int_ptr + TS_OFFSET;
}

int get_int(int obj_ptr) {

   int ptr = obj_ptr - TS_OFFSET;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return 0;
   } else {
      if (obj_store[ptr] != 'I') {
         return 0;
      } else {
         return atoi(&obj_store[ptr + 1]) ;
     }
   }
}

int make_float(int numer, int denom) {

   // Floats take 17 characters in the object store.
   if (op < 0 || op > TS_SZ - 17) {
      return 0; //Error
   }

   int float_ptr = op;
   obj_store[float_ptr] = 'F';

   int numer_ptr = make_int(numer);
   int denom_ptr = make_int(denom);

   if (numer_ptr + 8 != denom_ptr) {
      Serial.println(F("types.cpp: Error creating float.  Non-consecutive type pointers issued."));
      return 0;
   }

   return float_ptr + TS_OFFSET;

}

void diag_float(int obj_ptr) {

   Serial.print("Object Pointer: "); Serial.println(obj_ptr);

   if (obj_ptr <= TS_OFFSET || obj_ptr > (TS_OFFSET + TS_SZ - 17)) {
      Serial.println(F("diag_float: Illegal float pointer."));
      return;
   }

   if (obj_store[obj_ptr - TS_OFFSET] != 'F') {
      Serial.println(F("diag_float: Non-float type."));
      return;
   }

   Serial.print("Numerator: "); Serial.println(get_int(obj_ptr+1));
   Serial.print("Denomintor: "); Serial.println(get_int(obj_ptr+9));
}

float get_float(int obj_ptr) {

   int ptr = obj_ptr - TS_OFFSET;

   if (ptr <= 0 || ptr > TS_SZ - 17) {
      Serial.println(F("types.cpp: Illegal float pointer in get_float."));
      return 0;
   }

   if (obj_store[ptr] != 'F') {
      Serial.println(F("types.cpp: Attempt to get a float on a non-float type."));
      return 0;
   } else {

      int int_part = get_int(ptr + 1 + TS_OFFSET);
      int frac_part = get_int(ptr + 9 + TS_OFFSET); 

      return (float)int_part + produce_float(frac_part, 10,  MAX_INT_MAG);
   }
}


float produce_float(int frac_part, int cur_numer,  int max_int_mag) {

   if (frac_part > max_int_mag) {
      Serial.println(F("types.cpp: produce_float: number is to large."));
      return 0;
   }

   if (frac_part < cur_numer) {
      return (float)frac_part / (float)cur_numer;
   }

   return produce_float(frac_part, cur_numer * 10, max_int_mag);

}

int make_str(char *str) {

   if (op < 0 || op >= TS_MAX_INDEX) {
      Serial.println(F("types.cpp. Type buffer overflow in make_str()"));
      return 0; 
   } else {

      op++;
      obj_store[op] = 'X';
      int str_ptr = op;
      int input_str_ptr = 0;

      while (str[input_str_ptr]) {
         if (op < TS_MAX_INDEX) {
            op++;
//Serial.print("Putting ASCII "); Serial.print((int) str[input_str_ptr]); Serial.println(" into string.");
            obj_store[op] = str[input_str_ptr];
            input_str_ptr++;
         } else {
            Serial.println(F("types.cpp. Type buffer overflow in make_str()"));
            op = str_ptr - 1;
            return 0;
         }
      }      

     // Add null pointer to end of the string.
     if (op < TS_MAX_INDEX) {
        op++;
        obj_store[op] = 0;
        return str_ptr + TS_OFFSET;
     } else {
        Serial.println(F("types.cpp. Type buffer overflow in make_str()"));
        op = str_ptr - 1;
        return 0;
     }
   }
}

char *get_str(int obj_ptr) {

   int ptr = obj_ptr - TS_OFFSET;

   if (ptr <= 0 || ptr >=TS_SZ) {
      return 0;   //error
      Serial.println(F("Error in types.cpp: Not a string."));
   } else {
      if (obj_store[ptr] != 'X') {
         return 0;   //error
      } else {
         return obj_store + ptr + 1;  
     }
   }
}

void print_result(int ptr) {
   Serial.println(F("print_result is not implemented."));
}

void print_type_stats() {

   Serial.print("The Type system used "); Serial.print(op); 
   Serial.print(" bytes out of a maximum ");
   Serial.print(TS_SZ); Serial.println(" possible bytes.");
}

void test_types() {

   String tn = F("types.cpp");

   assert_int_equals(tn, -3069, get_int(make_int(-3069)));
   assert_int_equals(tn, 3456, get_int(make_int(3456)));

   int test = make_float(45, 175);
   assert_char_equals(tn, 'F', obj_store[test-TS_OFFSET]);
   assert_char_equals(tn, 'I', obj_store[test-TS_OFFSET+1]);
   assert_char_equals(tn, 'I', obj_store[test-TS_OFFSET+1]);
   assert_int_equals(tn, 45, get_int(test+1));
   assert_int_equals(tn, 175, get_int(test+9));

   assert_float_equals(tn, 34, (int) (10 * (get_float(make_float(3, 4)))));

   assert_char_equals(tn, 'U', get_char(make_char('U')));

   char test2[] = "foobar";
   assert_c_str_equals(tn, test2, get_str(make_str(test2)));

   int test3 = (int) (10000.0 * produce_float(1234, 10,  MAX_INT_MAG));
   assert_int_equals(tn, 1234, test3);
}
