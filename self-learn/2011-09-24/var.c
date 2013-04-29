/*
 * =====================================================================================
 *
 *       Filename:  var.c
 *
 *    Description:  Test variables storage and initial value
 *
 *        Version:  1.0
 *        Created:  09/24/2011 05:54:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongli Lv (), andrewlv@google.com
 *        Company:  Google Inc.
 *
 * =====================================================================================
 */

#include <stdio.h>


/* variables in section B/b will be initialized by compiler to zero. */
int integer_1; //in the uninitialized data section (known as BSS). B(global).
char char_1; //in the uninitialized data section (known as BSS). B(global).
char *char_pointer_1; //in the uninitialized data section (known as BSS). B(global).
char *str_1 = "This is a global string literal";//in the initialized data section. D

int integer_2 = 2;//in the initialized data section. D
char char_2 = 'c';//in the initialized data section. D
char *char_pointer_2 = &char_2;//in the initialized data section. D

static int static_integer_1;//in the uninitialized data section (known as BSS). b(local).
static char static_char_1;//in the uninitialized data section (known as BSS). b(local).
static char *static_char_pointer_1;//in the uninitialized data section (known as BSS). b(local).
static char *static_str_1 = "This is a local string literal";//in the initialized data section. d

static int static_integer_2 = 3;//in the initialized data section. d
static char static_char_2 = 'd';//in the initialized data section. d
static char *static_char_pointer_2 = &static_char_2;//in the initialized data section. d



const int const_integer_1;// B.
const char const_char_1;// B.
const char *pointer_to_const_char_1; // B.
char const *const_pointer_to_char_1; // B.
const char *const_str_1 = "This is a const global string literal"; // D.
char const *const_str_2 = "This is a const global string literal"; // D.

const int const_integer_2 = 2;//in a read only data section. R.
const char const_char_2 = 'a';//in a read only data section. R.
const char *pointer_to_const_char_2 = &const_char_2;// D.
char const *const_pointer_to_char_2 = &const_char_2;// D.

static const int static_const_integer_2 = 3;// r.
static const char static_const_char_2 = 'd';// r.
static const char *static_pointer_to_const_char_2 = &static_const_char_2;// d.
static const char *static_const_str_1 = "This is a const local string literal"; // d.



extern char *str_3; //not in local symbol table.

/* test_function is in global text(code) section. T. */
/* if declare with static, it will be in local text(code) section. t. */
void test_function(int x, int y) {
  int integer_2;
  char char_2;
  char *char_pointer_2;
  char *str_2 = "This is a string literal";

  static int static_integer_2; // b.
  static char static_char_2;// b.
  static char *static_char_pointer_2;// b.
  static char *static_str_2 = "This is a static string literal";//in the initialized data section. d

}


int main(int argc, char *argv[]) {
  printf("%d\n", integer_1);
}
