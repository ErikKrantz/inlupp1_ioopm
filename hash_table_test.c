#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <string.h>
#define No_Buckets 17


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
  /// TODO: update the names of the fields to something better? 
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  //char c;
  char *s;
};

static int get_int_hash_key(elem_t key){
  return key.i;
}

static bool compare_int_hash(elem_t a, elem_t b){
  return a.i == b.i;
}

static bool compare_str_hash(elem_t a, elem_t b){
  return !strcmp(a.s,b.s);
}

// Will not work so consistent for shorter string keys
int string_sum_hash(const char *str)
{
  int result = 0;
  do
    {
      result += *str;
    }
  while (*++str != '\0');
  return result;
}

void test_create_and_destroy(void)
{
  // Int-based hash-keys
  ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_int_hash);
  ioopm_hash_table_destroy(ht);

  // String-based hash-keys
  ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  ioopm_hash_table_destroy(ht);
}


void test_insert_lookup(void)
{ 
    // Test for int hash
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
    char *value[No_Buckets] = {"a","b","c","d","e","f","g","h","i","j","k","l","n","o","p","q"};
    bool success = false;
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_hash_table_insert(ht,int_elem(i),str_elem(value[i]));
    }
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_hash_table_lookup(ht,int_elem(i),&success);
        CU_ASSERT(success);
    }

    ioopm_hash_table_destroy(ht);
    

    /*
    ioopm_hash_table_destroy(ht);

    // TODO: Test for str hash 
    ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);

    for (int i = 0; i < No_Buckets; i++)
    {
        //char *s = itoa(i);
        ioopm_hash_table_insert(ht,str_elem(i),str_elem(value[i]));
    }
    for (int i = 0; i < No_Buckets; i++)
    {
        //char *s = itoa(i);
        CU_ASSERT(ioopm_hash_table_lookup(ht,str_elem(i),success).s == value[i]);
    }*/
    
    CU_ASSERT(true);
}


void test_table_lookup(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key,compare_str_hash);
    bool success=true;
    //Check value in an empty hash table
    ioopm_hash_table_lookup(ht,int_elem(16),&success);
    CU_ASSERT_FALSE(success);

    //Check value that does not exist in a non emmpty list
    ioopm_hash_table_insert(ht,int_elem(-1),str_elem("a"));
    ioopm_hash_table_lookup(ht,int_elem(16),&success);
    CU_ASSERT_FALSE(success);
    ioopm_hash_table_lookup(ht,int_elem(1), &success);
    CU_ASSERT_FALSE(success);

    //Check value in a list of size 1;
    CU_ASSERT_STRING_EQUAL((ioopm_hash_table_lookup(ht,int_elem(-1), &success)).s, "a");
    CU_ASSERT(success);
    ioopm_hash_table_lookup(ht,int_elem(1), &success);
    CU_ASSERT_FALSE(success);

    //Check values in the middle and last in a bucket
    ioopm_hash_table_insert(ht,int_elem(16),str_elem("b"));
    ioopm_hash_table_insert(ht,int_elem(33),str_elem(NULL));

    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_lookup(ht,int_elem(16), &success).s,"b");
    CU_ASSERT(success);

    ioopm_hash_table_lookup(ht,int_elem(50),&success);
    CU_ASSERT_FALSE(success);

    ioopm_hash_table_lookup(ht,int_elem(33), &success);
    CU_ASSERT(success);
    
    ioopm_hash_table_destroy(ht);
}


void test_remove(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);

    ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
    ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
    ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
    
  
    //check removal i mitten
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(1)).s, "a"); 

    //check removal i början
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(-16)).s, "b"); 
    
    //check removal i slutet
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(18)).s, "c"); 
    
    //check removal av icke-existerande entry
    CU_ASSERT_PTR_NULL(ioopm_hash_table_remove(ht,int_elem(5)).s);

    ioopm_hash_table_destroy(ht);
}


void test_hashtable_size(){
  ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_int_hash);

  ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
  ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
  ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
  /*
  CU_ASSERT(ioopm_hash_table_size(ht)==0);
  ioopm_hash_table_insert(ht,1,"a");
  CU_ASSERT(ioopm_hash_table_size(ht)==1);
  ioopm_hash_table_insert(ht,-16,"b");
  ioopm_hash_table_insert(ht,18,"c");
  CU_ASSERT(ioopm_hash_table_size(ht)==3);
  ioopm_hash_table_destroy(ht);
  */
 ioopm_hash_table_destroy(ht);
}

void test_is_empty(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}


void test_clear(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  ioopm_hash_table_clear(ht);

  ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_table_keys(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  
  // Test empty keys
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(keys), 0);
  ioopm_linked_list_destroy(keys);
  
  //int keys_length = sizeof(*keys) / sizeof(int);
  
  // Test non-empty keys
  ioopm_hash_table_insert(ht,int_elem(10),str_elem("a"));
  keys = ioopm_hash_table_keys(ht);
  size_t key_amount = 1;
  CU_ASSERT_EQUAL(ioopm_linked_list_size(keys), key_amount);
  
  ioopm_linked_list_destroy(keys);
  
  ioopm_hash_table_destroy(ht);
}


void test_table_values(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(values), 0);
  ioopm_linked_list_destroy(values);
  
  //int keys_length = sizeof(*keys) / sizeof(int);
  
  // Test non-empty keys
  ioopm_hash_table_insert(ht,int_elem(10),str_elem("a"));
  values = ioopm_hash_table_keys(ht);
  size_t key_amount = 1;
  CU_ASSERT_EQUAL(ioopm_linked_list_size(values), key_amount);
  ioopm_linked_list_destroy(values);
  
  

  ioopm_hash_table_destroy(ht);
}


void test_has_key(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  
  // Check before inserts 
  bool test = ioopm_hash_table_has_key(ht,int_elem(10));
  CU_ASSERT_FALSE(test);

  // Check after inserts
  ioopm_hash_table_insert(ht, int_elem(17), str_elem("abc"));
  test = ioopm_hash_table_has_key(ht,int_elem(17));
  CU_ASSERT(test);
  ioopm_hash_table_insert(ht, int_elem(34), str_elem("abc"));
  test = ioopm_hash_table_has_key(ht,int_elem(34));
  CU_ASSERT(test);
  
  // Check after removal
  ioopm_hash_table_remove(ht, int_elem(17));
  test = ioopm_hash_table_has_key(ht,int_elem(17));
  CU_ASSERT_FALSE(test);

  // Check after clear
  ioopm_hash_table_clear(ht);
  test = ioopm_hash_table_has_key(ht,int_elem(34));
  CU_ASSERT_FALSE(test);

  ioopm_hash_table_destroy(ht);
}

/*
elem_t ioopm_strdup(elem_t str)
{
  size_t len = strlen(str.s);
  char *result = calloc(len + 1, sizeof(char));
  elem_t result_t = str_elem(result);
  strncpy(result_t.s, str.s, len);
  free(result);
  return result_t;
}
*/

void test_has_value(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  //test empty ht
  bool test = ioopm_hash_table_has_value(ht,str_elem("abc"));
  CU_ASSERT_FALSE(test);
  //test one element ht
  ioopm_hash_table_insert(ht, int_elem(17), str_elem("a"));
  test = ioopm_hash_table_has_value(ht,str_elem("a"));
  CU_ASSERT(test);

  //test multipe element ht
  ioopm_hash_table_insert(ht, int_elem(34), str_elem("b"));
  elem_t test_string = str_elem("b");
  //elem_t target = ioopm_strdup(test_string);
  test = ioopm_hash_table_has_value(ht,str_elem("b"));
  CU_ASSERT(test);
  
  test = ioopm_hash_table_has_value(ht,test_string);
  CU_ASSERT(test);
  
  //test value that does not exist in a multipe element ht
  test = ioopm_hash_table_has_value(ht,str_elem("abc"));
  CU_ASSERT_FALSE(test);

  ioopm_hash_table_destroy(ht);

}


//Checks if *compare_string" == value
static bool string_equal(elem_t key, elem_t value, void *compare_string, ioopm_eq_function elem_cmp, hash_function hash_func){
  elem_t *other_value_ptr = compare_string;
  elem_t other_value = *other_value_ptr;
  return (elem_cmp(value, other_value));
}

void test_hash_table_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  elem_t test_string = str_elem("a");
  //empty table
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, string_equal, &test_string));
  //all values the same
  ioopm_hash_table_insert(ht, int_elem(17), str_elem("a"));
  ioopm_hash_table_insert(ht, int_elem(34), str_elem("a"));
  ioopm_hash_table_insert(ht, int_elem(3), str_elem("a"));
  CU_ASSERT(ioopm_hash_table_all(ht, string_equal, &test_string));
  //different values
  ioopm_hash_table_insert(ht, int_elem(20), str_elem("b"));
  ioopm_hash_table_insert(ht, int_elem(51), str_elem("b"));
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, string_equal, &test_string));

  ioopm_hash_table_destroy(ht);
}

//Changes *entry = *string_to_insert
static void change_value(elem_t key, elem_t *entry, void *string_to_insert)
{
  // change the value of the entry
  elem_t *string_to_insert_ptr = string_to_insert;
  elem_t string = *string_to_insert_ptr;
  *entry = string;
}

void test_hash_apply_to_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  elem_t string = str_elem("kalas");
  //test apply to empty list
  ioopm_hash_table_apply_to_all(ht,change_value,&string);
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, string_equal, &string));
  //different values
  ioopm_hash_table_insert(ht, int_elem(-17), str_elem("a"));
  ioopm_hash_table_insert(ht, int_elem(0),str_elem("b"));
  ioopm_hash_table_insert(ht, int_elem(17), str_elem("c"));
  ioopm_hash_table_insert(ht, int_elem(34), str_elem("d"));
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, string_equal, &string));
  //apply same string and test
  ioopm_hash_table_apply_to_all(ht,change_value,&string);
  CU_ASSERT(ioopm_hash_table_all(ht, string_equal, &string));

  ioopm_hash_table_destroy(ht);
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
    (NULL == CU_add_test(test_suite1, "test 1: table create and destroy", test_create_and_destroy)) ||
    (NULL == CU_add_test(test_suite1, "test 2: insert", test_insert_lookup)) || 
    (NULL == CU_add_test(test_suite1, "test 3: entry remove", test_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 4: hash size", test_hashtable_size)) ||
    (NULL == CU_add_test(test_suite1, "test 5: is empty", test_is_empty)) ||
    (NULL == CU_add_test(test_suite1, "test 6: clear", test_clear)) ||
    (NULL == CU_add_test(test_suite1, "test 7: table keys", test_table_keys)) ||
    (NULL == CU_add_test(test_suite1, "test 8: table values", test_table_values)) || 
    (NULL == CU_add_test(test_suite1, "test 9: table has key",test_has_key)) ||
    (NULL == CU_add_test(test_suite1, "test 10: table has value", test_has_value)) ||
    (NULL == CU_add_test(test_suite1, "test 11: hash table all",test_hash_table_all)) ||
    (NULL == CU_add_test(test_suite1, "test 12: hash table apply all",test_hash_apply_to_all)) ||
    (NULL == CU_add_test(test_suite1, "test 13: lookup", test_table_lookup))

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