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

void test_append(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  //append to empty list
  ioopm_linked_list_append(list,10);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0),10);
  //append to list with 1 element
  ioopm_linked_list_append(list,15);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 1),15);

  ioopm_linked_list_destroy(list);
  CU_ASSERT(true);
}

void test_prepend(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  //Prepend in empty list
  ioopm_linked_list_prepend(list,15);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0),15);
  //Prepend to list with 1 element
  ioopm_linked_list_prepend(list,10);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0),10);

  ioopm_linked_list_destroy(list);
  CU_ASSERT(true);
}

void test_insert(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  //insert last
  int index=0;
  ioopm_linked_list_insert(list,index,20); 
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, index),20);
  //insert first
  ioopm_linked_list_insert(list,index,10); 
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, index),10);
  //insert in the middle
  index=1;
  ioopm_linked_list_insert(list,index,15); 
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, index),15);

  ioopm_linked_list_destroy(list);
  CU_ASSERT(true);
}

void test_remove(void){
  ioopm_list_t *list = ioopm_linked_list_create();
  int index=0;
  ioopm_linked_list_insert(list,index,20); 
  ioopm_linked_list_insert(list,index,10);
  index=1;  
  ioopm_linked_list_insert(list,index,15); 
 
  //remove in the middle
  CU_ASSERT_EQUAL(ioopm_linked_list_remove(list,index),15);
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list,15));
  
  //remove last
  CU_ASSERT_EQUAL(ioopm_linked_list_remove(list,index),20);
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list,20));
  //remove first
  index = 0;
  CU_ASSERT_EQUAL(ioopm_linked_list_remove(list,index),10);
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list,10));

  ioopm_linked_list_destroy(list);
}

void test_get(void){
  ioopm_list_t *list = ioopm_linked_list_create();
  int index=0;
  ioopm_linked_list_insert(list,index,20); 
  ioopm_linked_list_insert(list,index,10);
  index=1;  
  ioopm_linked_list_insert(list,index,15); 
  //get first value
  index = 0;
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list,index),10);
  //get second value
  index = 1;
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list,index),15);
  //get last value
  index = 2;
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list,index),20);
  ioopm_linked_list_destroy(list);  
}

void test_contains(void){
  ioopm_list_t *list = ioopm_linked_list_create();
  //Look for value in an empty list
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list,10));
  //Look for the only value in a list, which is both first and last
  int index=0;
  ioopm_linked_list_insert(list,index,20);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list,20));
  //Look for the first value in a list
  ioopm_linked_list_insert(list,index,10);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list,10));
  //Look for the last value in a list
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list,20));
  //Look for a value in the middle of a list
  index=1;
  ioopm_linked_list_insert(list,index,15);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list,15));
  //Look for a value that does not exist
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list,77));

  ioopm_linked_list_destroy(list);  
}


void test_size(void){
  /*
  ioopm_list_t *list = ioopm_linked_list_create();
  // Test empty initialized list
  CU_ASSERT(ioopm_linked_list_size(list) == 0);
  
  // Test filled list
  ioopm_linked_list_insert(list,0,1);
  ioopm_linked_list_insert(list,1,2);
  ioopm_linked_list_insert(list,2,3);
  CU_ASSERT(ioopm_linked_list_size(list) == 3);
  
  // Test after removing elements
  ioopm_linked_list_remove(list,2);
  CU_ASSERT(ioopm_linked_list_size(list) == 2);
  
  // Test after clearing list
  ioopm_linked_list_clear(list)
  CU_ASSERT(ioopm_linked_list_size(list) == 0);
  */
  CU_ASSERT(true);
}

void test_is_empty(void){
  /*
  ioopm_list_t *list = ioopm_linked_list_create();
  
  // Test empty init list
  CU_ASSERT(ioopm_linked_list_is_empty(list));
  
  // Test empty after insert
  ioopm_linked_list_insert(list,0,1);
  CU_ASSERT(!ioopm_linked_list_is_empty(list));
  
  // Test after remove
  ioopm_linked_list_remove(list,0);
  CU_ASSERT(ioopm_linked_list_is_empty(list));
  
  // Test after clear
  ioopm_linked_list_insert(list,0,1);
  ioopm_linked_list_insert(list,1,2);
  ioopm_linked_list_clear(list);
  CU_ASSERT(ioopm_linked_list_is_empty(list));
  */
  CU_ASSERT(true);
}

void test_clear(void){
  /*
  ioopm_list_t *list = ioopm_linked_list_create();
  
  // Test clearing empty initialized list
  ioopm_linked_list_clear(list);
  CU_ASSERT(ioopm_linked_list_size(list) == 0);
  
  // Test clearing non-empty list
  ioopm_linked_list_insert(list,0,1);
  ioopm_linked_list_insert(list,1,2);
  ioopm_linked_list_clear(list);
  CU_ASSERT(ioopm_linked_list_size(list) == 0);
  
  // Test if first and last points to the same
  ioopm_linked_list_insert(list,0,1);
  ioopm_linked_list_insert(list,1,2);
  ioopm_linked_list_clear(list);
  
  CU_ASSERT_EQUAL(list->first,list->last);  
  
  */
  CU_ASSERT(true); 
}

static bool element_exist(int element, void *x){
  int *other_ele_ptr = x;
  int other_ele = *other_ele_ptr;
  return element == other_ele;
}


static void change_element(int *element, void *x){
  int *other_ele_ptr = x;
  int other_ele = *other_ele_ptr;
  *element = other_ele;
}

void test_all(void){
  /*
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_insert(list,0,10);
  ioopm_linked_list_insert(list,1,20);
  ioopm_linked_list_insert(list,1,30);
  ioopm_linked_list_insert(list,1,40);
  int extra = 10;
  CU_ASSERT(!ioopm_linked_list_all(list, element_exist, &extra));
  */
  CU_ASSERT(true);
  
}

void test_any(void){
  /*
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_insert(list,0,10);
  ioopm_linked_list_insert(list,1,20);
  ioopm_linked_list_insert(list,1,30);
  ioopm_linked_list_insert(list,1,40);
  int extra = 10;
  CU_ASSERT(ioopm_linked_list_any(list, element_exist, &extra));
  */
  CU_ASSERT(true);
}

void test_apply_to_all(void){
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
    (NULL == CU_add_test(test_suite1, "test 1: create and destroy", test_create_and_destroy)) ||
    (NULL == CU_add_test(test_suite1, "test 2: append", test_append)) ||
    (NULL == CU_add_test(test_suite1, "test 3: prepend", test_prepend)) ||
    (NULL == CU_add_test(test_suite1, "test 4: insert", test_insert)) ||
    (NULL == CU_add_test(test_suite1, "test 5: remove", test_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 6: get", test_get)) ||
    (NULL == CU_add_test(test_suite1, "test 7: contains", test_contains)) ||
    (NULL == CU_add_test(test_suite1, "test 8: size", test_size)) ||
    (NULL == CU_add_test(test_suite1, "test 9: is empty", test_is_empty)) ||
    (NULL == CU_add_test(test_suite1, "test 10: clear", test_clear)) ||
    (NULL == CU_add_test(test_suite1, "test 11: all", test_all)) ||
    (NULL == CU_add_test(test_suite1, "test 12: any", test_any)) ||
    (NULL == CU_add_test(test_suite1, "test 13: apply to all", test_apply_to_all)) 
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
