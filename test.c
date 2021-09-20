#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <string.h>
#define No_Buckets 17


typedef struct entry ioomp_entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  ioomp_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  ioomp_entry_t *buckets[No_Buckets];
};

struct answer
{
  bool  success;
  char * answer;
};

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_table_create(void)
{
  ioopm_hash_table_t* ht = ioopm_hash_table_create();
  int key = 18;
  CU_ASSERT(ht->buckets[key%No_Buckets]->key == 0);
  CU_ASSERT(true);
  ioopm_hash_table_destroy(ht);
}

void test_table_insert_lookup(void)
{
    ioopm_hash_table_t* ht = ioopm_hash_table_create();
    char *value[No_Buckets] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q"};
    bool *success = calloc(1, sizeof(bool));
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_hash_table_insert(ht,i,value[i]);
    }
    for (int i = 0; i < No_Buckets; i++)
    {
        CU_ASSERT(ioopm_hash_table_lookup(ht,i,success) == value[i]);
    }
    ioopm_hash_table_insert(ht,35,"r");
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,35, success), "r"));
    ioopm_hash_table_insert(ht,18,"s");
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,18, success), "s"));
    CU_ASSERT(true);
    free(success);
    ioopm_hash_table_destroy(ht);

}

void test_table_lookup(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create();
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

void test_remove(void){
    ioopm_hash_table_t *ht = ioopm_hash_table_create();
    ioopm_hash_table_insert(ht,1,"a");
    ioopm_hash_table_insert(ht,-16,"b");
    ioopm_hash_table_insert(ht,18,"c");
  
    //check removal i mitten
    CU_ASSERT(!strcmp(ioopm_hash_table_remove(ht,1), "a")); 

    //check removal i början
    CU_ASSERT(!strcmp(ioopm_hash_table_remove(ht,-16), "b"));
    
    //check removal i slutet
    CU_ASSERT(!strcmp(ioopm_hash_table_remove(ht,18), "c"));
    
    //check removal av icke-existerande entry
    //char *nullstr = NULL;

    CU_ASSERT_PTR_NULL(ioopm_hash_table_remove(ht,5));

    ioopm_hash_table_destroy(ht);
}

void test_hashtable_size(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT(ioopm_hash_table_size(ht)==0);
  ioopm_hash_table_insert(ht,1,"a");
  int x = ioopm_hash_table_size(ht);
  printf("%d\n", x);
  CU_ASSERT(ioopm_hash_table_size(ht)==1);
  ioopm_hash_table_insert(ht,-16,"b");
  ioopm_hash_table_insert(ht,18,"c");
  CU_ASSERT(ioopm_hash_table_size(ht)==3);
  ioopm_hash_table_destroy(ht);
}

void test_empty_hashtable(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht,-16,"b");
  CU_ASSERT(!ioopm_hash_table_is_empty(ht));
  
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_clear_hashtable(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  //ioopm_hash_table_clear(ht);
  ioopm_hash_table_insert(ht,1,"a");
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void test_table_keys(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int *keys = ioopm_hash_table_keys(ht);
  CU_ASSERT_PTR_NULL(*keys);
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
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char **values = ioopm_hash_table_values(ht);
  CU_ASSERT_PTR_NULL(*values);
  free(values);

  ioopm_hash_table_insert(ht,10,"b");
  values = ioopm_hash_table_values(ht);
  CU_ASSERT_STRING_EQUAL(*values, "b");
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
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
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

void test_has_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
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
    (NULL == CU_add_test(test_suite1, "test 1: table create", test_table_create)) ||
    (NULL == CU_add_test(test_suite1, "test 2: insert", test_table_insert_lookup)) ||
    (NULL == CU_add_test(test_suite1, "test 3: entry remove", test_remove)) ||
    (NULL == CU_add_test(test_suite1, "test 4: hash size", test_hashtable_size)) ||
    (NULL == CU_add_test(test_suite1, "test 5: hash table is empty", test_empty_hashtable)) ||
    (NULL == CU_add_test(test_suite1, "test 6: hash table clear", test_clear_hashtable)) ||
    (NULL == CU_add_test(test_suite1, "test 7: table keys", test_table_keys)) ||
    (NULL == CU_add_test(test_suite1, "test 8: table values", test_table_values)) || 
    (NULL == CU_add_test(test_suite1, "test 9: table has key",test_has_key)) ||
    (NULL == CU_add_test(test_suite1, "test 10: table has value", test_has_value))
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