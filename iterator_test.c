#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "linked_list.h"
#include "iterator.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *s;
};

void test_has_next(void){
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);

    //Test next in empty list
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));
    ioopm_iterator_destroy(iter);

    //Test ext in list with one elemet
    ioopm_linked_list_insert(list,0,int_elem(10));
    iter = ioopm_list_iterator(list);
    CU_ASSERT(ioopm_iterator_has_next(iter));
    
    //test next standing on the last element in a list
    ioopm_iterator_next(iter);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_next(void)
{
    ioopm_list_t *list = ioopm_linked_list_create();

    //Test iterating over a list
    ioopm_linked_list_insert(list,0,int_elem(20));
    ioopm_linked_list_insert(list,0,int_elem(10));
    ioopm_list_iterator_t *iter =  ioopm_list_iterator(list);
    CU_ASSERT(ioopm_iterator_next(iter).i == 10);
    CU_ASSERT(ioopm_iterator_next(iter).i == 20);

    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_reset(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_insert(list,0,int_elem(20));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  //moves iter to last link
  ioopm_iterator_next(iter);
  //copy is at last link
  ioopm_list_iterator_t *copy = iter;
  //placing iter back to first link
  ioopm_iterator_reset(iter);
  CU_ASSERT(iter != copy);
}

void test_current(void){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_insert(list,0,int_elem(20));
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  //moves iter from dummy to first "real" linkmake
  ioopm_iterator_next(iter);
  CU_ASSERT(ioopm_iterator_current(iter).i == 20);
  
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);  
  CU_ASSERT(true);
}

void test_destroy(void){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
  
  CU_ASSERT(true);
}

int main()
{
  CU_pSuite test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
    (NULL == CU_add_test(test_suite1, "test 1: has next", test_has_next)) ||
    (NULL == CU_add_test(test_suite1, "test 2: next", test_next)) ||
    (NULL == CU_add_test(test_suite1, "test 3: reset", test_next)) ||
    (NULL == CU_add_test(test_suite1, "test 4: current", test_next)) ||
    (NULL == CU_add_test(test_suite1, "test 5: destroy", test_destroy))
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
