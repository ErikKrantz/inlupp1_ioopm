#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets[17];
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
  CU_ASSERT(ht->buckets[key%17]->key == 0);
  CU_ASSERT(true);
}

void test_table_insert_lookup(void)
{
    ioopm_hash_table_t* ht = ioopm_hash_table_create();
    char *value[17] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q"};
    for (int i = 0; i < 17; i++)
    {
        ioopm_hash_table_insert(ht,i,value[i]);
    }
    for (int i = 0; i < 17; i++)
    {
        CU_ASSERT(ioopm_hash_table_lookup(ht,i).answer == value[i]);
    }
    ioopm_hash_table_insert(ht,35,"r");
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,35).answer, "r"));
    ioopm_hash_table_insert(ht,18,"s");
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,18).answer, "s"));
    CU_ASSERT(true);
}

void test_table_lookup(void){
    ioopm_hash_table_t* ht = ioopm_hash_table_create();
    ioopm_hash_table_insert(ht,-1,"a");
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(ht,16).answer, "a"));
    CU_ASSERT(strcmp(ioopm_hash_table_lookup(ht,1).answer, "a"));
    CU_ASSERT(ioopm_hash_table_lookup(ht,16).success);
    CU_ASSERT(!ioopm_hash_table_lookup(ht,1).success);
    ioopm_hash_table_insert(ht,3,NULL);
    CU_ASSERT(ioopm_hash_table_lookup(ht,3).success);
    CU_ASSERT(!ioopm_hash_table_lookup(ht,0).success);
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
    (NULL == CU_add_test(test_suite1, "test 2: insert", test_table_insert_lookup))
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