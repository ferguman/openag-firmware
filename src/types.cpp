#include "Arduino.h"
#include <pair.h>
#include <types.h>
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
const int INT_STORAGE_SIZE = 8;

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
boolean is_str(int obj_ptr) {return is_typeof('X', obj_ptr);}

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
   op = op + (INT_STORAGE_SIZE - 1);

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

int make_float(int int_part, int frac_part ) {

   // Floats take 17 characters in the object store.
   if (op < 0 || op > TS_SZ - 17) {
      return 0; //Error
   }

   op++;
   int float_ptr = op;
   obj_store[float_ptr] = 'F';

   int int_part_ptr = make_int(int_part);
   int frac_part_ptr = make_int(frac_part);

   if (int_part_ptr + INT_STORAGE_SIZE != frac_part_ptr) {
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

      if (int_part < 0) {
         return (float)int_part - produce_float(frac_part, 1,  MAX_INT_MAG);
      } else {
         return (float)int_part + produce_float(frac_part, 1,  MAX_INT_MAG);
      }
   }
}

float produce_float(int frac_part, int cur_denom,  int max_int_mag) {

   if (frac_part > max_int_mag) {
      Serial.println(F("types.cpp: produce_float: number is to large."));
      return 0;
   }

   if (frac_part < cur_denom) {
      return (float)frac_part / (float)cur_denom;
   }

   return produce_float(frac_part, cur_denom * 10, max_int_mag);

}

// The pair of characters \" is interpretted as the single character ". 
// This allows higher level routines to allow embedded "'s in user
// strings that are specified as string literals, such as at the command line:
// eg. (cmd "string arg with \" inside it.")
//
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

void print_result(int args) {

   if (args == -1) {
      //Assume everything worked ok and there is nothing to print.
      return;
   }

   if (args == 0) {
      //Assume a failure with nothing to print.
      Serial.println(F("Processing failed."));
      return;
   }

   if (is_str(args)) {
      Serial.println(get_str(args));
      return;
   }

   if (is_pair(args)) {
      Serial.println(get_str(car(args)));
      return;
   }

   Serial.print(F("Error: Unknown result returned by the processing: "));
   Serial.println(args);
   return;
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
   assert_char_equals(tn, 'I', obj_store[test-TS_OFFSET+INT_STORAGE_SIZE+1]);
   assert_int_equals(tn, 45, get_int(test+1));
   assert_int_equals(tn, 175, get_int(test+9));

   assert_float_equals(tn, 34, (int) (10 * (get_float(make_float(3, 4)))));
   assert_float_equals(tn, 1, (int) (10 * (get_float(make_float(0, 1)))));
   assert_float_equals(tn, 8, (int) (10 * (get_float(make_float(0, 8)))));

   assert_char_equals(tn, 'U', get_char(make_char('U')));

   char test2[] = "foobar";
   assert_c_str_equals(tn, test2, get_str(make_str(test2)));

   int test3 = (int) (10000.0 * produce_float(1234, 1,  MAX_INT_MAG));
   assert_int_equals(tn, 1234, test3);

   // Test "foo\"bar"
   char test4[] = {102, 111, 111, 92, 34, 98, 97, 114, 0}; 
   char *test5 = get_str(make_str(test4));
   assert_true(tn, test5[0] == 102);
   assert_true(tn, test5[2] == 111);
   assert_true(tn, test5[3] == 34);
   assert_true(tn, test5[6] == 114);
   assert_true(tn, test5[7] == 0);
}
