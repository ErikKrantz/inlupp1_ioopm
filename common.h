#pragma once
#include <stdbool.h>

#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }
#define uns_int_elem(x) (elem_t) { .u=(x) }
#define bool_elem(x) (elem_t) { .b=(x) }
#define float_elem(x) (elem_t) { .f=(x) }
#define char_elem(x) (elem_t) { .c=(x) }
#define str_elem(x) (elem_t) { .s=(x) }

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char c;
  char *s;
};

typedef union elem elem_t;
typedef struct list_iterator ioopm_list_iterator_t;
typedef struct list ioopm_list_t;

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

//Returns an index for a given key
typedef int(*hash_function)(elem_t a);
