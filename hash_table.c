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
  hash_function hash_function; // Todo check ptr
  ioopm_eq_function elem_cmp;  // Todo check ptr

};

static ioopm_entry_t *entry_create(elem_t key, elem_t value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(hash_function hash_func, ioopm_eq_function elem_cmp) //TODO check pointers
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
  result->elem_cmp = elem_cmp;       
  return result;
}


/*

Returnerar NULL om entry ska läggas först i bucketen,
annars returnerar entry för den som ligger före
*/

static ioopm_entry_t *find_previous_entry_for_key(elem_t key, hash_function hash_func, ioopm_entry_t *cursor)
{
  ioopm_entry_t *tmp = cursor->next;
  if (tmp == NULL)
  {
    return cursor;
  }
 while (tmp != NULL)
 {
    if (hash_func(key) == hash_func(tmp->key)){ // ||tmp->key >key
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

static void update_entry_value(elem_t value, ioopm_entry_t *entry, elem_t key, ioopm_eq_function eq_func){
    if(eq_func(key, entry->key)){ //key == cur_entry_old->key
    entry->value = value; //ändrar value på en nyckel som finns
    }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value){
  /// Calculate the bucket for this entry
  hash_function hash_func = ht->hash_function; // TODO: check ptr
  ioopm_eq_function elem_cmp = ht->elem_cmp;   // TODO: check ptr
  int bucket = possitve_modulo(hash_func(key), No_Buckets);

  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, hash_func, ht->buckets[bucket]);//en av två alternativ, kopia vs adress
  ioopm_entry_t *cur_entry = NULL;

  if(pre_entry->next != NULL){ //"om vi inte är i slutet av listan, alltså finns nyckeln"
    update_entry_value(value, pre_entry->next, key, elem_cmp);
  }
  cur_entry = entry_create(key,value,pre_entry->next); //
  pre_entry->next = cur_entry;
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, bool *success){
  hash_function hash_func = ht->hash_function;
  int bucket = possitve_modulo(hash_func(key), No_Buckets);
  /// Find the previous entry for key
  ioopm_entry_t *tmp = find_previous_entry_for_key(key, hash_func,ht->buckets[bucket]);
  ioopm_entry_t *next = tmp->next;
  if (next != NULL)
  {
    if(hash_func(next->key) == hash_func(key))
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
  //ioopm_eq_function *elem_cmp = ht->elem_cmp;     TODO: uncomment when used
  int bucket = possitve_modulo(hash_func(key), No_Buckets);

  ioopm_entry_t *pre_entry= find_previous_entry_for_key(key, hash_func, ht->buckets[bucket]);
  ioopm_entry_t *delete_entry = pre_entry->next;
  if(delete_entry ==NULL){
    return ptr_elem(NULL);
  }
  ioopm_entry_t *next_entry = delete_entry->next;
  pre_entry->next = next_entry; // uppdaterar next i pre_entry
  elem_t value_deleted = delete_entry->value;
  entry_destroy(delete_entry); // Kanske behövs mer free för (value/key/next)
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
  ioopm_list_t *list = ioopm_linked_list_create();
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
  ioopm_list_t *list = ioopm_linked_list_create();
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
    }  
  }
  return list;
}

static bool key_equiv(elem_t key, elem_t value, void *x, ioopm_eq_function elem_cmp, hash_function hash_func){

  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return (hash_func(key) == hash_func(other_key)); // return (0 == hash_func(key) == hash_func(other_key));
}

static bool value_equiv(elem_t key, elem_t value, void *x, ioopm_eq_function elem_cmp, hash_function hash_func){
  elem_t *other_value_ptr = x;
  elem_t other_value = *other_value_ptr;
  return elem_cmp(value, other_value);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  ioopm_entry_t *entry = NULL; 
  ioopm_eq_function elem_cmp = ht->elem_cmp;      // TODO check ptr
  hash_function hash_func = ht->hash_function;    // TODO check ptr
  //kanske bättre prestanda att kolla empty_table först
  for (size_t i = 0; i < No_Buckets; i++){
    entry = ht->buckets[i]->next;
    while (entry!=NULL){
      if (pred(entry->key, entry->value, arg, elem_cmp, hash_func)){
        return true;
      }
      entry = entry->next;
    }
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  ioopm_eq_function elem_cmp = ht->elem_cmp;       // TODO: fix ptr
  hash_function hash_func = ht->hash_function;     // TODO: fix ptr
  ioopm_entry_t *entry = NULL;
  if(ioopm_hash_table_is_empty(ht)){
    return false;
  } 
  for (size_t i = 0; i < No_Buckets; i++){
    entry  = ht->buckets[i]->next;
    while (entry!=NULL){
      if(!pred(entry->key, entry->value, arg, elem_cmp, hash_func)){
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

/*
int main(int argc, char const *argv[])
{
  
  return 0;
}
*/

/*
static void recursion_over_bucket(size_t *counter, ioopm_entry_t_old *entry)
{
  if (entry_old->next != NULL){
    ++*counter;
    recursion_over_bucket(counter,entry_old->next);
  }
}
*/



/*
int main(int argc, char *argv[])
{
  ioopm_hash_table_t_old *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 34,"tretiofyra");
  ioopm_hash_table_insert(ht,0, "noll");
  ioopm_hash_table_insert(ht, 17, NULL);
  ioopm_hash_table_insert(ht,-17,"minus17");
  ioopm_hash_table_insert(ht,51,"femtio1");
  ioopm_hash_table_insert(ht,3,"tre");
  ioopm_hash_table_insert(ht,1,"ett");

  printf("%d, ",ht->buckets[1]->next->key);
  printf("%d, ",ht->buckets[3]->next->key);
  printf("%d, ",ht->buckets[0]->next->key);
  printf("%d, ",ht->buckets[0]->next->next->key);
  printf("%d, ",ht->buckets[0]->next->next->next->key);
  printf("%d, ",ht->buckets[0]->next->next->next->next->key);
  printf("%d\n",ht->buckets[0]->next->next->next->next->next->key);

 if(ioopm_hash_table_has_key_2(ht, 17)){
    printf("Test has 17 key = passed");
  }

  if(!ioopm_hash_table_has_key_2(ht, 27)){
    printf("Test has 27 key = passed");
  }
   

  int *testkey = ioopm_hash_table_keys(ht);

  for (int i = 0; i < 7; i++)
  {
    printf("%d. %d\n", i+1, *testkey);
    ++testkey;
  }
  
  char **testvalues = ioopm_hash_table_values(ht);
  for (int i = 0; i < 8; i++)
  {
    printf("%d. %s\n", i+1, *testvalues);
    ++testvalues;
  }
  //ht->buckets[3] = d;

  bool success;
  char *result = ioopm_hash_table_lookup(ht, -17, &success);
  if (success && result != NULL){
  printf("value = %s\n", result);
  }
   if (success && result==NULL){
     printf("value = NULL\n");
   }
  if (!success){
    // Här kan vi byta EINVAL (errno) till rimligt error message
  printf("Did not find a value\n");
  }
  int x = ioopm_hash_table_size(ht);
  printf("%d\n", x);

  // TODO: Remove - testar value
  
  
  ioopm_hash_table_destroy(ht);



  return 0;
}
*/