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

void test_create_and_destroy(void)
{
  // Int-based hash-keys
  ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_int_hash);
  ioopm_hash_table_destroy(ht);

  // String-based hash-keys
  ht = ioopm_hash_table_create(get_int_hash_key, compare_str_hash);
  ioopm_hash_table_destroy(ht);
}


void test_table_insert_lookup(void)
{
    // Test for int hash
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_int_hash);
    char *value[No_Buckets] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q"};
    bool *success = calloc(1, sizeof(bool));
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_hash_table_insert(ht,int_elem(i),str_elem(value[i]));
    }
    for (int i = 0; i < No_Buckets; i++)
    {
        CU_ASSERT(ioopm_hash_table_lookup(ht,int_elem(i),success).s == value[i]);
    }

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
    }
    */

    free(success);
    ioopm_hash_table_destroy(ht);

}

/*
void test_table_lookup(void){
    ioopm_hash_table_t_old* ht = ioopm_hash_table_create();
    ioopm_hash_table_insert(ht,-1,"a");
    bool success;

    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,16,&success), "a"));
    CU_ASSERT(strcmp(ioopm_hash_table_lookup(ht,1, &success), "a"));
    ioopm_hash_table_lookup(ht,16, &success);
    CU_ASSERT(success);
    ioopm_hash_table_lookup(ht,1, &success);
    CU_ASSERT(!success);
    ioopm_hash_table_insert(ht,3,NULL);
    ioopm_hash_table_lookup(ht,3, &success);
    CU_ASSERT(success);
    ioopm_hash_table_lookup(ht,0,&success);
    CU_ASSERT(!success);
    ioopm_hash_table_destroy(ht);
}
*/

void test_remove(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create(get_int_hash_key, compare_int_hash);

    ioopm_hash_table_insert(ht,int_elem(1),str_elem("a"));
    ioopm_hash_table_insert(ht,int_elem(-16),str_elem("b"));
    ioopm_hash_table_insert(ht,int_elem(18),str_elem("c"));
    
  
    //check removal i mitten
    CU_ASSERT(ioopm_hash_table_remove(ht,int_elem(1)).s == "a"); 

    //check removal i början
    CU_ASSERT(ioopm_hash_table_remove(ht,int_elem(-16)).s == "b"); 
    
    //check removal i slutet
    CU_ASSERT(ioopm_hash_table_remove(ht,int_elem(18)).s == "c"); 
    
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
}
/*
void test_empty_hashtable(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht,-16,"b");
  CU_ASSERT(!ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_clear_hashtable(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  //ioopm_hash_table_clear(ht);
  ioopm_hash_table_insert(ht,1,"a");
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void test_table_keys(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  int *keys = ioopm_hash_table_keys(ht);
  CU_ASSERT(keys==NULL);
  ioopm_hash_table_insert(ht,10,"a");
  free(keys);
  keys = ioopm_hash_table_keys(ht);
  int keys_length = sizeof(*keys) / sizeof(int);
  CU_ASSERT(keys_length==1);
  CU_ASSERT(*keys==10);
  free(keys);
  ioopm_hash_table_destroy(ht);
}

void test_table_values(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  //char *values[] = ioopm_hash_table_values(ht);
  char **values = ioopm_hash_table_values(ht);
  CU_ASSERT_PTR_NULL(*values);
  free(values);

  ioopm_hash_table_insert(ht,10,"b");
  values = ioopm_hash_table_values(ht);
  //CU_ASSERT_STRING_EQUAL(values[0], "b");
  CU_ASSERT_STRING_EQUAL(*values, "b");
  //++values;
  //CU_ASSERT_PTR_NULL(*values)
  CU_ASSERT_PTR_NULL(values[1]);
  free(values);

  ioopm_hash_table_insert(ht,9,"a");
  values = ioopm_hash_table_values(ht);
  CU_ASSERT_STRING_EQUAL(*values, "a");
  CU_ASSERT_STRING_EQUAL(values[1], "b");
  CU_ASSERT_PTR_NULL(values[2]);
  free(values);

  ioopm_hash_table_destroy(ht);
}

void test_has_key(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  bool test = ioopm_hash_table_has_key(ht,10);
  CU_ASSERT_FALSE(test);
  ioopm_hash_table_insert(ht, 17, "abc");
  test = ioopm_hash_table_has_key(ht,17);
  CU_ASSERT(test);
  ioopm_hash_table_insert(ht, 34, "abc");
  test = ioopm_hash_table_has_key(ht,34);
  CU_ASSERT(test);
  test = ioopm_hash_table_has_key(ht,1);
  CU_ASSERT_FALSE(test);
  ioopm_hash_table_destroy(ht);
}

char *ioopm_strdup(char *str)
{
  size_t len = strlen(str);
  char *result = calloc(len + 1, sizeof(char));
  strncpy(result, str, len);
  return result;
}

void test_has_value(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  bool test = ioopm_hash_table_has_value(ht,"abc");
  CU_ASSERT_FALSE(test);
  ioopm_hash_table_insert(ht, 17, "a");
  
  test = ioopm_hash_table_has_value(ht,"a");
  CU_ASSERT(test);

  ioopm_hash_table_insert(ht, 34, "b");
  char *test_string = "b";
  char *target = ioopm_strdup(test_string);
  test = ioopm_hash_table_has_value(ht,"b");
  CU_ASSERT(test);
  test = ioopm_hash_table_has_value(ht,target);
  CU_ASSERT(test);
  test = ioopm_hash_table_has_value(ht,"abc");
  CU_ASSERT_FALSE(test);

  free(target);
  ioopm_hash_table_destroy(ht);

}

static bool value_equiv(int key, char *value, void *x){
  char **other_value_ptr = x;
  char *other_value = *other_value_ptr;
  return (strcmp(value, other_value) == 0);
}

void test_hash_table_all(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  char *test_string = "a";
  //empty table
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_equiv, &test_string));
  //all values the same
  ioopm_hash_table_insert(ht, 17, "a");
  ioopm_hash_table_insert(ht, 3, "a");
  CU_ASSERT(ioopm_hash_table_all(ht, value_equiv, &test_string));
  //different values
  ioopm_hash_table_insert(ht, 20, "b");
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, value_equiv, &test_string));

  ioopm_hash_table_destroy(ht);
}

static void change_value(int key, char **entry, void *string_to_insert)
{
  // change the value of the entry
  char **string_to_insert_ptr = string_to_insert;
  char *string = *string_to_insert_ptr;
  *entry = string;
}

void test_hash_apply_to_all(){
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  char *string = "kalas";
  ioopm_hash_table_insert(ht, -17, "a");
  ioopm_hash_table_insert(ht, 0, "b");
  ioopm_hash_table_insert(ht, 17, "c");
  ioopm_hash_table_insert(ht, 34, "d");
  CU_ASSERT(!ioopm_hash_table_all(ht, value_equiv, &string));
  ioopm_hash_table_apply_to_all(ht,change_value,&string);
  CU_ASSERT(ioopm_hash_table_all(ht, value_equiv, &string));

  ioopm_hash_table_destroy(ht);
}
*/
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
    (NULL == CU_add_test(test_suite1, "test 2: insert", test_table_insert_lookup)) || 
    (NULL == CU_add_test(test_suite1, "test 3: entry remove", test_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 4: hash size", test_hashtable_size))
//    (NULL == CU_add_test(test_suite1, "test 5: hash table is empty", test_empty_hashtable)) ||
  //  (NULL == CU_add_test(test_suite1, "test 6: hash table clear", test_clear_hashtable)) ||
    //(NULL == CU_add_test(test_suite1, "test 7: table keys", test_table_keys)) ||
//    (NULL == CU_add_test(test_suite1, "test 8: table values", test_table_values)) || 
  //  (NULL == CU_add_test(test_suite1, "test 9: table has key",test_has_key)) ||
    //(NULL == CU_add_test(test_suite1, "test 10: table has value", test_has_value)) ||
//    (NULL == CU_add_test(test_suite1, "test 11: hash table all",test_hash_table_all)) ||
  //  (NULL == CU_add_test(test_suite1, "test 12: hash table apply all",test_hash_apply_to_all))
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