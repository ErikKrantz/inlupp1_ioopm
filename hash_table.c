#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "hash_table.h"

#define No_Buckets 17
#define _XOPEN_SOURCE 700
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
  ioopm_entry_t *buckets[No_Buckets];
  hash_function hash_function; 
  ioopm_eq_function value_cmp;
  ioopm_eq_function key_cmp;
};

static ioopm_entry_t *entry_create(elem_t key, elem_t value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(hash_function hash_func, ioopm_eq_function value_cmp, ioopm_eq_function key_cmp) //TODO check pointers
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to ioopm_entry_t's
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  /// Initialise the ioopm_entry_t pointers to NULL
  for (int i = 0; i < No_Buckets; ++i)
    {
      result->buckets[i] = NULL;
    }
   /// Get a pointer to the first entry in the bucket
  for (int i = 0; i < No_Buckets; i++)
  {
    result->buckets[i] = entry_create(int_elem(0),ptr_elem(NULL),NULL);
  }
  result->hash_function = hash_func; 
  result->value_cmp = value_cmp;       
  result->key_cmp = key_cmp;
  return result;
}


/*

Returnerar NULL om entry ska läggas först i bucketen,
annars returnerar entry för den som ligger före
*/

static ioopm_entry_t *find_previous_entry_for_key(elem_t key, ioopm_eq_function key_cmp, ioopm_entry_t *cursor)
{
  ioopm_entry_t *tmp = cursor->next;
 while (tmp != NULL)
 {
    if (key_cmp(key, tmp->key)){ // || hash_func(tmp->key) > hash_func(key)){ // ||tmp->key >key
      return cursor;
      }
  cursor = tmp;
  tmp = cursor->next;
 }
  return cursor;
}

static int possitve_modulo (int n, int mod){
  n = n % mod;
  if(n<0){
    n += mod;
  }
  return n;
}

static void update_entry_value(elem_t value, ioopm_entry_t *entry){
    entry->value = value; //ändrar value på en nyckel som finns
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value){
  /// Calculate the bucket for this entry
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function key_cmp = ht->key_cmp;
  int bucket = possitve_modulo(hash_func(key), No_Buckets);

  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);//en av två alternativ, kopia vs adress
  ioopm_entry_t *cur_entry = NULL;

  if(pre_entry->next != NULL){ //"om vi inte är i slutet av listan, alltså finns nyckeln"
    update_entry_value(value, pre_entry->next);
  }
  else{//vi måste skapa en ny entry och lägga till den sist
  cur_entry = entry_create(key,value,pre_entry->next); 
  pre_entry->next = cur_entry;
  }
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, bool *success){
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function key_cmp = ht->key_cmp;
  int bucket = possitve_modulo(hash_func(key), No_Buckets);
  /// Find the previous entry for key
  ioopm_entry_t *tmp = find_previous_entry_for_key(key, key_cmp,ht->buckets[bucket]);
  ioopm_entry_t *next = tmp->next;
  if (next != NULL)
  {
    if(key_cmp(next->key, key))
   {
    *success = true;
    return next->value;
   }
  }
  *success = false;
  return ptr_elem(NULL);
}

static void entry_destroy(ioopm_entry_t *entry){
    free(entry);
    entry = NULL;
}

//TODO: felmeddelande om vi vill remova något som inte finns
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key){
  hash_function hash_func = ht->hash_function;      // TODO fix ptr
  ioopm_eq_function key_cmp = ht->key_cmp;

  //ioopm_eq_function *value_cmp = ht->value_cmp;     TODO: uncomment when used
  int bucket = possitve_modulo(hash_func(key), No_Buckets);

  ioopm_entry_t *pre_entry= find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);
  ioopm_entry_t *delete_entry = pre_entry->next;
  if(delete_entry ==NULL){
    return ptr_elem(NULL);
  }
  ioopm_entry_t *next_entry = delete_entry->next;
  pre_entry->next = next_entry; // uppdaterar next i pre_entry
  elem_t value_deleted = delete_entry->value;
  entry_destroy(delete_entry);
  return value_deleted;
}

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

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht){
  for(int i = 0; i<No_Buckets; ++i){
    bucket_destroy(ht->buckets[i]);
  }
  free(ht);
  ht=NULL;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; ++i)
  {
    if (ht->buckets[i]->next != NULL) //"If there exists entries"
    {
      return false;
    }
  }
  return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)              // Iterera över alla buckets
  {
    if(ht->buckets[i]->next != NULL){               // Om första elementet efter dummyn inte är NULL
      bucket_destroy(ht->buckets[i]->next);
      ht->buckets[i]->next = NULL;
    }
  }
}

static void itterate_over_bucket(size_t *counter, ioopm_entry_t *entry)
{
  while (entry->next != NULL)
  {
    ++*counter;
    entry = entry->next;
  }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht){
 size_t c = 0;
 for (int i = 0; i < No_Buckets; i++)
 {
   ioopm_entry_t *first_entry = ht->buckets[i];
   itterate_over_bucket(&c, first_entry);
 }
 return c;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  ioopm_list_t *list = ioopm_linked_list_create(ht->value_cmp);
  size_t no_keys = ioopm_hash_table_size(ht);
  ioopm_entry_t *entry = NULL;
  if (no_keys==0) // empty hash table
  {
    //Kanske ska destory list? Det gjorde vi på förra
    return list;
  }
  for (int i = 0; i < No_Buckets; i++){ // iterate over array of buckets
     entry = ht->buckets[i]->next;
     while (entry != NULL){
      ioopm_linked_list_append(list, entry->key);
      entry = entry->next;
    }
  }
  return list;
}

// Denna kan användas för M39
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht){
  ioopm_list_t *list = ioopm_linked_list_create(ht->value_cmp);
  size_t no_values = ioopm_hash_table_size(ht);
  ioopm_entry_t *entry = NULL;
  if (no_values==0) // empty hash table
  {
    //Kanske ska destory list? Det gjorde vi på förra
    return list;
  } 
  for (int i = 0; i < No_Buckets; i++){ // iterate over array of bucket
    entry = ht->buckets[i]->next;
    while (entry != NULL){
    ioopm_linked_list_append(list,entry->value);
    entry = entry->next;
    }  
  }
  return list;
}

static bool key_equiv(elem_t key, elem_t value, void *x,  ioopm_hash_table_t *ht ){
  ioopm_eq_function key_cmp = ht->key_cmp;
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return (key_cmp(key, other_key)); // return (0 == hash_func(key) == hash_func(other_key));
}

static bool value_equiv(elem_t key, elem_t value, void *x, ioopm_hash_table_t *ht){
  ioopm_eq_function value_cmp = ht->value_cmp;
  elem_t *other_value_ptr = x;
  elem_t other_value = *other_value_ptr;
  return value_cmp(value, other_value);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  ioopm_entry_t *entry = NULL; 
  //ioopm_eq_function value_cmp = ht->value_cmp;      // TODO check ptr
  //kanske bättre prestanda att kolla empty_table först
  for (size_t i = 0; i < No_Buckets; i++){
    entry = ht->buckets[i]->next;
    while (entry!=NULL){
      if (pred(entry->key, entry->value, arg, ht)){
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  //ioopm_eq_function value_cmp = ht->value_cmp;
  //hash_function hash_func = ht->hash_function;
  ioopm_entry_t *entry = NULL;
  if(ioopm_hash_table_is_empty(ht)){
    return false;
  } 
  for (size_t i = 0; i < No_Buckets; i++){
    entry  = ht->buckets[i]->next;
    while (entry!=NULL){
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

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg){
  ioopm_entry_t *entry = NULL;
  elem_t *value_p = calloc(1,sizeof(elem_t));
  for (size_t i = 0; i < No_Buckets; i++){
    entry = ht->buckets[i]->next;
    while (entry!=NULL){
      *value_p = entry->value;
      (apply_fun(entry->key, value_p, arg));
      entry->value = *value_p;
      entry = entry->next;
    }
  }
  free(value_p);
  value_p = NULL;
}