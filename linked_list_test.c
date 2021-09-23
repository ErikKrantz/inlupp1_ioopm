#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_create_and_destroy(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_destroy(list);
  CU_ASSERT(true);
}

void test2(void)
{
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
    (NULL == CU_add_test(test_suite1, "test1: create and destroy", test_create_and_destroy)) ||
    (NULL == CU_add_test(test_suite1, "test 2", test2))
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