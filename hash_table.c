#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "hash_table.h"

#define Inital_capacity 17
#define _XOPEN_SOURCE 700
#define Load_Factor 0.75
//#include <errno.h>

// New generic type implementation

union elem
{
  /// TODO: update the names of the fields to something better? 
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char c;
  char *s;
};


struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  size_t capacity;
  hash_function hash_function; 
  ioopm_eq_function value_cmp;
  ioopm_eq_function_key key_cmp;
  ioopm_entry_t **buckets; // OLD: ioopm_entry_t *buckets[No_Buckets];
};

static int possitve_modulo (int n, int mod){
  n = n % mod;
  if(n<0){
    n += mod;
  }
  return n;
}

static ioopm_entry_t *entry_create(elem_t key, elem_t value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

/*
Returns: The entry before key, if key exists
         else, the entry that "should" be before key
*/
static ioopm_entry_t *find_previous_entry_for_key(elem_t key, ioopm_eq_function_key key_cmp, ioopm_entry_t *cursor)
{
 ioopm_entry_t *tmp = cursor->next;
 while (tmp != NULL)
 {
    if (key_cmp(tmp->key, key) >= 0){
      return cursor;
      }
  cursor = tmp;
  tmp = cursor->next;
 }
  return cursor;
}

static void reorder_entrys(ioopm_hash_table_t *ht, size_t old_capacity){
  size_t new_capacity = ht->capacity;
  hash_function hash_fun = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  //Goes over each old bucket, the new ones only consist of dummys
  for(int i = 0; i<old_capacity; ++i){
    ioopm_entry_t *pre_entry_old = ht->buckets[i];
    ioopm_entry_t *entry = pre_entry_old->next;
    while (entry != NULL)
    {
      int new_bucket = possitve_modulo(hash_fun(entry->key),new_capacity);
      ioopm_entry_t *next_entry_old = entry->next;
      //If entry is not placed in the right bucket, move to the right bucket
      if(new_bucket!=i){
        ioopm_entry_t *pre_entry_new = find_previous_entry_for_key(entry->key, key_cmp, ht->buckets[new_bucket]);
        ioopm_entry_t *next_entry_new = pre_entry_new->next;
        pre_entry_new->next = entry;                     // Places entry in new bucket
        entry->next = next_entry_new;                    // Update entrys new next
        
        //remove entry from previous bucket 
        pre_entry_old->next = next_entry_old;  
      }
      //If entry is placed in the right bucket, move to the next entry
      else pre_entry_old = entry;
      entry = next_entry_old;   
    }
  }
}

static void create_buckets(ioopm_entry_t **buckets_test, ioopm_hash_table_t *ht, size_t old_capacity){
  size_t new_capacity = ht->capacity;
  buckets_test = (ioopm_entry_t**)realloc(buckets_test, new_capacity*sizeof(ioopm_entry_t*));
   for (int i = old_capacity; i <  new_capacity; i++)
    {
      buckets_test[i] = entry_create(int_elem(0),ptr_elem(NULL),NULL);
    }
  (ht->buckets) = buckets_test;
}

//Allocates new buckets and reorder entrys
void ioopm_hash_table_resize(ioopm_hash_table_t *ht){
size_t old_capacity = ht->capacity;
  size_t primes[] = {0, 17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
  size_t length_primes = 11;
  size_t new_capacity = 0;
  //Findes a new prime bucket size
  for (size_t i = 0; i<length_primes; i++)
    {
     if (primes[i] == old_capacity){
     new_capacity = primes[i+1];
     ht->capacity = new_capacity;
     }
    }
  create_buckets(ht->buckets,ht, old_capacity);
  reorder_entrys(ht, old_capacity);
}



ioopm_hash_table_t *ioopm_hash_table_create(hash_function hash_func, ioopm_eq_function value_cmp, ioopm_eq_function_key key_cmp){
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to ioopm_entry_t's
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->capacity = 0;
  result->hash_function = hash_func; 
  result->value_cmp = value_cmp;       
  result->key_cmp = key_cmp;
  ioopm_hash_table_resize(result);
  return result;
}

//Change the value of an existing key
static void update_entry_value(elem_t value, ioopm_entry_t *entry){
    entry->value = value;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value){

  if (ioopm_hash_table_size(ht) >= Load_Factor*ht->capacity){
    ioopm_hash_table_resize(ht);
  }

  /// Calculate the right bucket for entry
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  int hashed_key = hash_func(key);
  int bucket = possitve_modulo(hashed_key, ht->capacity);

  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);
  ioopm_entry_t *cur_entry = pre_entry->next;

  // Updates value if key already exists
  if(cur_entry != NULL && 0==key_cmp(key, cur_entry->key)){ 
    update_entry_value(value, pre_entry->next);
  }
  else{// Add a new entry and place it in right position
  cur_entry = entry_create(key,value,pre_entry->next); 
  pre_entry->next = cur_entry;
  }
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, bool *success){
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  int bucket = possitve_modulo(hash_func(key), ht->capacity);
  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, key_cmp,ht->buckets[bucket]);
  ioopm_entry_t *cur_entry = pre_entry->next;
  if (cur_entry != NULL && 0 == key_cmp(cur_entry->key, key))
  {
    *success = true;
    return cur_entry->value;
  }
  *success = false;
  return ptr_elem(NULL);
}

// Destory the entry and free its allocated memory
static void entry_destroy(ioopm_entry_t *entry){
    free(entry);
    entry = NULL;
}

//TODO: felmeddelande om vi vill remova något som inte finns
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, bool *success){
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;

  int bucket = possitve_modulo(hash_func(key), ht->capacity);

  ioopm_entry_t *pre_entry= find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);
  ioopm_entry_t *delete_entry = pre_entry->next;

  //Nothing to delete
  if(delete_entry == NULL || 0 != key_cmp(delete_entry->key, key)){ 
    *success = false;
    return ptr_elem(NULL);
  }
  elem_t value_deleted = delete_entry->value;
  pre_entry->next = delete_entry->next; 
  entry_destroy(delete_entry);
  *success = true;
  return value_deleted;
}

// Iterate and call entry_destroy over each individual entry in a bucket
static void bucket_destroy(ioopm_entry_t *current_entry)
{
  ioopm_entry_t *next_entry = NULL;
   while (current_entry->next != NULL)
 {
   next_entry = current_entry->next;
   entry_destroy(current_entry);
   current_entry = next_entry;
 }
 entry_destroy(current_entry);
 
}

// Iterate over all buckets and call bucket_destroy for each index
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht){
  for(int i = 0; i<ht->capacity; ++i){
    bucket_destroy(ht->buckets[i]);
  }
  free(ht->buckets);
  ht->buckets=NULL;
  free(ht);
  ht = NULL;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < ht->capacity; ++i)
  {
    if (ht->buckets[i]->next != NULL)     //If there is an entry besides the dummy
    {
      return false;
    }
  }
  return true;
}

 void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if(ht->buckets[i]->next != NULL){       //If there is an entry besides the dummy
      bucket_destroy(ht->buckets[i]->next); //Delete all entrys linked to the dummy
      ht->buckets[i]->next = NULL;
    }
  }
}

// Go through all entries in a bucket and add 1 to a counter for each entry passed through
static void itterate_over_bucket(size_t *counter, ioopm_entry_t *entry)
{
  while (entry->next != NULL)
  {
    ++*counter;
    entry = entry->next;
  }
}

// For all buckets call iterate_over_bucket with the adress of a counter
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht){
 size_t c = 0;
 for (int i = 0; i < ht->capacity; i++)
 {
   ioopm_entry_t *first_entry = ht->buckets[i];
   itterate_over_bucket(&c, first_entry);
 }
 return c;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  ioopm_list_t *list = ioopm_linked_list_create(ht->key_cmp);
  size_t no_keys = ioopm_hash_table_size(ht);
  ioopm_entry_t *entry = NULL;
  // If there exist no keys return the empty linked list;
  if (no_keys==0)
  {
    return list;
  }
  for (int i = 0; i < ht->capacity; i++){
     entry = ht->buckets[i]->next;
     while (entry != NULL){
      ioopm_linked_list_append(list, entry->key);
      entry = entry->next;
    }
  }
  return list;
}


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht){
  ioopm_list_t *list = ioopm_linked_list_create(ht->value_cmp);
  size_t no_values = ioopm_hash_table_size(ht);
  ioopm_entry_t *entry = NULL;
  // If there exist no keys return the empty linked list;
  if (no_values==0) 
  {
    return list;
  } 
  for (int i = 0; i < ht->capacity; i++){
    entry = ht->buckets[i]->next;
    while (entry != NULL){
    ioopm_linked_list_append(list,entry->value);
    entry = entry->next;
    }  
  }
  return list;
}

// Compares a key and a given pointer and check whether they are equal or not
static bool key_equiv(elem_t key, elem_t value, void *x,  ioopm_hash_table_t *ht ){
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return (0==key_cmp(key, other_key));
}

//Checks equaltiy of a given value and a void pointer(value)
static bool value_equiv(elem_t key, elem_t value, void *x, ioopm_hash_table_t *ht){
  ioopm_eq_function value_cmp = ht->value_cmp;
  elem_t *other_value_ptr = x;
  elem_t other_value = *other_value_ptr;
  return value_cmp(value, other_value);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  ioopm_entry_t *entry = NULL; 
  for (size_t i = 0; i < ht->capacity; i++){
    entry = ht->buckets[i]->next;
    while (entry!=NULL){
      // For each entry apply the given condition and check if ANY of them is true
      if (pred(entry->key, entry->value, arg, ht)){
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  ioopm_entry_t *entry = NULL;
  if(ioopm_hash_table_is_empty(ht)){
    return false;
  } 
  for (int i = 0; i < ht->capacity; i++){
    entry  = ht->buckets[i]->next;
    while (entry!=NULL){
      // For each entry apply the given condition and check if ANY of them is false
      if(!pred(entry->key, entry->value, arg, ht)){
        return false;
      }
      entry = entry->next;
    }
  }
  return true;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value){
  return ioopm_hash_table_any(ht, value_equiv, &value);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key){
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg){
  ioopm_entry_t *entry = NULL;
  elem_t *value_p = calloc(1,sizeof(elem_t));

  for (size_t i = 0; i < ht->capacity; i++){
    entry = ht->buckets[i]->next;
    while (entry!=NULL){
      *value_p = entry->value;
      //For each entry apply the given function on its value
      (apply_fun(entry->key, value_p, arg));
      entry->value = *value_p;
      entry = entry->next;
    }
  }
  free(value_p);
  value_p = NULL;
}

size_t ioopm_hash_table_capacity(ioopm_hash_table_t *ht){
  return ht->capacity;
}
