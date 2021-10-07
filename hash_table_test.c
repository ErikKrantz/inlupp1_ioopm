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

static int get_int_hash_key(elem_t key){
  return key.i;
}

static int int_key_cmp(elem_t a, elem_t b){
  return a.i - b.i;
}

bool int_value_cmp (elem_t a, elem_t b){
  return a.i == b.i;
}

static bool string_value_cmp(elem_t a, elem_t b){
  return !strcmp(a.s,b.s);
}

static int str_key_cmp(elem_t a, elem_t b){
  return strcmp(a.s,b.s);
}

// Will not work so consistent for shorter string keys
int string_sum_hash(elem_t string)
{
  char *str = string.s;
  int result = 0;
  do
    {
      result += *str;
    }
  while (*++str != '\0');
  return result;
}

//
// Helper function for test_hash_table_all and test_hash_table_any
//
//Checks if *compare_string" == value
static bool string_equal(elem_t key, elem_t value, void *compare_string, ioopm_hash_table_t *ht){
  elem_t *other_value_ptr = compare_string;
  elem_t other_value = *other_value_ptr;
  return (string_value_cmp(value, other_value));
}

//
// Helper function for test_hash_table_apply to all
//
// Changes *entry = *string_to_insert
static void change_value(elem_t key, elem_t *entry, void *string_to_insert)
{
  // change the value of the entry
  elem_t *string_to_insert_ptr = string_to_insert;
  elem_t string = *string_to_insert_ptr;
  *entry = string;
}

void test_create_and_destroy(void)
{
  // Int-based hash-keys
  ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  ioopm_hash_table_destroy(ht);

  // String-based hash-keys
  ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  ioopm_hash_table_destroy(ht);
}

void test_insert(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
    char *value[No_Buckets] = {"a","b","c","d","e","f","g","h","i","j","k","l","n","o","p","q"};
    
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_hash_table_insert(ht,int_elem(i),str_elem(value[i]));
    }
    ioopm_hash_table_destroy(ht);
}

void test_insert_lookup_int(void)
{ 
    // Test for int hash
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
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
}


void test_insert_lookup_str(void){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, int_value_cmp, str_key_cmp);
    char *keys[No_Buckets] = {"a","b","c","d","e","f","g","h","i","j","k","l","n","o","p","q"};
    bool success = false;
    for (int i = 0; i<16; i++)
    {
      ioopm_hash_table_insert(ht,str_elem(keys[i]), int_elem(i));
    }
    for (int i = 0; i < 16; i++)
    {
        ioopm_hash_table_lookup(ht,str_elem(keys[i]),&success);
        CU_ASSERT(success);
    }
    ioopm_hash_table_destroy(ht);
}

void test_char_for_keys(void){
  ioopm_hash_table_t* ht = ioopm_hash_table_create(string_sum_hash, int_value_cmp, str_key_cmp);
  bool success = true;
  ioopm_hash_table_lookup(ht, str_elem("a"), &success);
  CU_ASSERT_FALSE(success);
  //Check value that does not exist in a non emmpty list
    ioopm_hash_table_insert(ht,str_elem("a"),int_elem(-1));
    ioopm_hash_table_lookup(ht,str_elem("b"),&success);
    CU_ASSERT_FALSE(success);
    int value = ioopm_hash_table_lookup(ht,str_elem("a"), &success).i;
    CU_ASSERT_EQUAL(value,-1);
    CU_ASSERT(success);
    ioopm_hash_table_lookup(ht,str_elem("hej"), &success);
    CU_ASSERT_FALSE(success);
    ioopm_hash_table_destroy(ht);
}

void test_lookup(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key,string_value_cmp, int_key_cmp);
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
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
    bool success = true;
    //check removal av icke-existerande entry i tom ht
    CU_ASSERT_PTR_NULL(ioopm_hash_table_remove(ht,int_elem(5), &success).s);
    CU_ASSERT_FALSE(success);

    ioopm_hash_table_insert(ht,int_elem(1),str_elem("b"));
    ioopm_hash_table_insert(ht,int_elem(-16),str_elem("a"));
    ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
    
  
    //check removal i mitten
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(1), &success).s, "b"); 
    CU_ASSERT(success);

    //check removal i början
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(-16), &success).s, "a"); 
    CU_ASSERT(success);

    //check removal i slutet
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_remove(ht,int_elem(18), &success).s, "c"); 
    CU_ASSERT(success);

    //check removal av icke-existerande entry som har funnits
    CU_ASSERT_PTR_NULL(ioopm_hash_table_remove(ht,int_elem(1), &success).s);
    CU_ASSERT_FALSE(success);

    ioopm_hash_table_destroy(ht);
}


void test_hashtable_size(){
  ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  //Check empty hash table
  CU_ASSERT(ioopm_hash_table_size(ht)==0);

  //Check hash table of one element
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
  CU_ASSERT(ioopm_hash_table_size(ht)==1);

  //Check size after inserting multiple elements
  ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
  ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
  CU_ASSERT(ioopm_hash_table_size(ht)==3);

  // Check size after inserting a key that already exists
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
  CU_ASSERT(ioopm_hash_table_size(ht)==3);

  // Check size after inserting multiple keys that already exists
  ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
  ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
  CU_ASSERT(ioopm_hash_table_size(ht)==3);

  // Check size after clearing all elements in hashtable
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_size(ht)==0);

 ioopm_hash_table_destroy(ht);
}

void test_is_empty(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  //Check hash empty table
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  //Check hash table with one element
  ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  
  //Check hash table after clearing all elements
  ioopm_hash_table_insert(ht,int_elem(-13),str_elem("c"));
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}


void test_clear(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  //Check clear of empty hash table
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  //Chekc clear after inserting elements
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_destroy(ht);
}

void test_table_keys(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  
  // Test creating a list when no keys exists
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(keys), 0);
  ioopm_linked_list_destroy(keys);
  
  
  // Test creating a list when keys do exist
  ioopm_hash_table_insert(ht,int_elem(10),str_elem("a"));
  keys = ioopm_hash_table_keys(ht);
  size_t key_amount = 1;
  CU_ASSERT_EQUAL(ioopm_linked_list_size(keys), key_amount);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(keys,0).s,10);

  ioopm_linked_list_destroy(keys);
  
  ioopm_hash_table_destroy(ht);
}


void test_table_values(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  
  //Test for an empty hash table
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(values), 0);
  ioopm_linked_list_destroy(values);
    
  // Test non-empty hash table
  ioopm_hash_table_insert(ht,int_elem(10),str_elem("a"));
  values = ioopm_hash_table_values(ht);
  
  CU_ASSERT_EQUAL(ioopm_linked_list_size(values), 1);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(values,0).s,"a");
  ioopm_linked_list_destroy(values);

  ioopm_hash_table_destroy(ht);
}


void test_has_key(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  
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
  
  bool success = false;
  // Check after removal
  ioopm_hash_table_remove(ht, int_elem(17), &success);
  CU_ASSERT(success);
  test = ioopm_hash_table_has_key(ht,int_elem(17));
  CU_ASSERT_FALSE(test);

  // Check after clear
  ioopm_hash_table_clear(ht);
  test = ioopm_hash_table_has_key(ht,int_elem(34));
  CU_ASSERT_FALSE(test);

  ioopm_hash_table_destroy(ht);
}



void test_has_value(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
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

void test_hash_table_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
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

void test_apply_to_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
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

void test_growing_buckets(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(get_int_hash_key, string_value_cmp, int_key_cmp);
  
  // Add 10 elements and check that buckets has not increased from initial size of 17
  for (int i = 0; i<10; i++)
  {
    ioopm_hash_table_insert(ht,int_elem(i), str_elem("bucket_test"));
  }
  size_t size_test = 17;
  CU_ASSERT_EQUAL(ioopm_hash_table_capacity(ht), size_test);

  // Add 5 additional element to check that passing threshold 12.5 has increased buckets size to 31
  for (int i = 10; i<15; i++)
  {
    ioopm_hash_table_insert(ht,int_elem(i), str_elem("bucket_test_threshold"));
  }
  size_test = 31;
  CU_ASSERT_EQUAL(ioopm_hash_table_capacity(ht), size_test);

  for(int i = 0; i<15; i++){
    bool success = false;
    ioopm_hash_table_lookup(ht,int_elem(i),&success);
    CU_ASSERT(success);
  }

  // Add 100 additional element to check increased buckets size to 127
  for (int i = 15; i<115; i++)
  {
    ioopm_hash_table_insert(ht,int_elem(i), str_elem("bucket_test_threshold"));
  }
  size_test = 127;

  for(int i = 0; i<115; i++){
    bool success = false;
    ioopm_hash_table_lookup(ht,int_elem(i),&success);
    CU_ASSERT(success);
  }

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
    (NULL == CU_add_test(test_suite1, "test 1: test char for keys",test_char_for_keys)) ||
    (NULL == CU_add_test(test_suite1, "test 2: table create and destroy", test_create_and_destroy)) ||
    (NULL == CU_add_test(test_suite1, "test 3: insert", test_insert)) || 
    (NULL == CU_add_test(test_suite1, "test 4: remove", test_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 5: hash size", test_hashtable_size)) ||
    (NULL == CU_add_test(test_suite1, "test 6: is empty", test_is_empty)) ||
    (NULL == CU_add_test(test_suite1, "test 7: clear", test_clear)) ||
    (NULL == CU_add_test(test_suite1, "test 8: linked list keys", test_table_keys)) ||
    (NULL == CU_add_test(test_suite1, "test 9: linked list values", test_table_values)) || 
    (NULL == CU_add_test(test_suite1, "test 10: has key",test_has_key)) ||
    (NULL == CU_add_test(test_suite1, "test 11: has value", test_has_value)) ||
    (NULL == CU_add_test(test_suite1, "test 12: all",test_hash_table_all)) ||
    (NULL == CU_add_test(test_suite1, "test 13: apply all",test_apply_to_all))||
    (NULL == CU_add_test(test_suite1, "test 14: insert", test_insert_lookup_int)) ||
    (NULL == CU_add_test(test_suite1, "test 15: insert", test_insert_lookup_str)) ||  
    (NULL == CU_add_test(test_suite1, "test 16: lookup", test_lookup)) ||
    (NULL == CU_add_test(test_suite1, "test 17: growing bucket", test_growing_buckets)) 

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