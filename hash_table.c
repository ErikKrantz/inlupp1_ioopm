#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "hash_table.h"

#define Init_No_Buckets 17 //TODO: remove
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

bool string_eq(elem_t e1, elem_t e2)
{
  return (!strcmp(e1.s, e2.s));
}


//Returns:
//0 if e1 = e2
//- if e1 < e2
//+ if e1 >e2 

int int_eq(elem_t e1, elem_t e2)
{
  return (e1.i-e2.i); // if((e1.i>=0 && e2.i>=0)||(e1.i<=0 && e2.i<=0)) 
 
}

int hash_function_int(elem_t e)
{
  return e.i;
}

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
Om key finns returneras det entry som ligger före,
Om key inte finns returneras det entry som "borde" ligga före
*/
static ioopm_entry_t *find_previous_entry_for_key(elem_t key, ioopm_eq_function_key key_cmp, ioopm_entry_t *cursor)
{
 ioopm_entry_t *tmp = cursor->next;
 while (tmp != NULL)
 {
    if (key_cmp(tmp->key, key) >= 0){ // tmp->key >key TODO: lägg till detta. Fall: vi vill inserta något först i en listsa
      return cursor;
      }
  cursor = tmp;
  tmp = cursor->next;
 }
  return cursor;

}

void reorder_entrys(ioopm_hash_table_t *ht, size_t old_capacity){
  size_t new_capacity = ht->capacity;
  hash_function hash_fun = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;

  for(int i = 0; i<old_capacity; ++i){
    ioopm_entry_t *tmp_entry = ht->buckets[i];
    ioopm_entry_t *entry = tmp_entry->next;
    while (entry != NULL)
    {
      int new_bucket = possitve_modulo(hash_fun(entry->key),new_capacity);
      //If entry is not placed in the right bucket, move to the right bucket
      if(new_bucket!=i){
        ioopm_entry_t *pre_entry = find_previous_entry_for_key(entry->key, key_cmp, ht->buckets[new_bucket]);
        ioopm_entry_t *next_entry = pre_entry->next;
        pre_entry->next = entry;
        entry->next = next_entry;
      //remove entry from previous bucket
      tmp_entry->next = entry->next;
      }
      tmp_entry = entry;
      entry = tmp_entry->next;
    }
  }
}

void create_buckets(ioopm_entry_t **buckets_test, ioopm_hash_table_t *ht, size_t old_capacity){
  
  //buckets_test = (ioopm_entry_t**)calloc(17, sizeof(ioopm_entry_t*));
  //buckets_test = calloc(17, sizeof(*buckets_test));
  //buckets_test = realloc(buckets_test, sizeof(*buckets_test));
  size_t new_capacity = ht->capacity;
  buckets_test = (ioopm_entry_t**)realloc(buckets_test, new_capacity*sizeof(ioopm_entry_t*));

  printf("realloc works for capacity: %lu\n",new_capacity);

   for (int i = old_capacity; i <  new_capacity; i++)
    {
      buckets_test[i] = entry_create(int_elem(0),ptr_elem(NULL),NULL);
      //printf("Create dummy for bucket, key: %d \n",i);
    }
    buckets_test[new_capacity-1];
    printf("Created dummys for capacity %lu\n", new_capacity);

  (ht->buckets) = buckets_test;
}

void test_func(ioopm_hash_table_t *ht){
  printf("Line 146: Capacity: %lu\n", ht->capacity);
  
  for (size_t siz = 0; siz<ht->capacity; siz++){
    ioopm_entry_t *entry = ht->buckets[siz];

    while (entry->next != NULL)
    {
      printf("Buckets[%lu] contains: %d\n", siz, entry->key.i);
      entry = entry->next;
    }
  }
}

void ioopm_hash_table_resize(ioopm_hash_table_t *ht){
size_t old_capacity = ht->capacity;
  size_t primes[] = {0, 17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
  size_t length_primes = 11;
  size_t new_capacity = 0;
  for (size_t i = 0; i<length_primes; i++)
    {
     if (primes[i] == old_capacity){
     new_capacity = primes[i+1];
     }
    }
  ht->capacity = new_capacity;
  //test_func(ht);
  create_buckets(ht->buckets,ht, old_capacity);
  reorder_entrys(ht, old_capacity);
  test_func(ht);
  /*
  ioopm_entry_t **tmp = (ioopm_entry_t **)realloc(ht->buckets, new_capacity *sizeof(ioopm_entry_t*));
   
  //ht->buckets = (ioopm_entry_t **)realloc(ht->buckets, new_capacity *sizeof(ioopm_entry_t*));

  for (int i = old_capacity; i < new_capacity; i++)
  {
    tmp[i] =entry_create(int_elem(0),ptr_elem(NULL),NULL);
  }
  
  ht->buckets = tmp;
  */
  

}



ioopm_hash_table_t *ioopm_hash_table_create(hash_function hash_func, ioopm_eq_function value_cmp, ioopm_eq_function_key key_cmp) //TODO check pointers
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to ioopm_entry_t's
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->capacity = 0;
  result->hash_function = hash_func; 
  result->value_cmp = value_cmp;       
  result->key_cmp = key_cmp;
  ioopm_hash_table_resize(result);
  
  return result;
  
}


static void update_entry_value(elem_t value, ioopm_entry_t *entry){
    entry->value = value; //ändrar value på en nyckel som finns
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value){

 
  if (ioopm_hash_table_size(ht) >= Load_Factor*ht->capacity){
    printf("Threshold met, size ht: %lu\n", ioopm_hash_table_size(ht));
    ioopm_hash_table_resize(ht);
  }

  /// Calculate the bucket for this entry
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  int hashed_key = hash_func(key);
  int bucket = possitve_modulo(hashed_key, ht->capacity);

  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);//en av två alternativ, kopia vs adress
  ioopm_entry_t *cur_entry = pre_entry->next;

  if(cur_entry != NULL && 0==key_cmp(key, cur_entry->key)){ //TODO: check segfault"om vi inte är i slutet av listan, alltså finns nyckeln"
    update_entry_value(value, pre_entry->next);
  }
  else{//vi måste skapa en ny entry och lägga till den sist
  cur_entry = entry_create(key,value,pre_entry->next); 
  pre_entry->next = cur_entry;
  }
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, bool *success){
  hash_function hash_func = ht->hash_function;
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  int bucket = possitve_modulo(hash_func(key), ht->capacity);
  /// Find the previous entry for key
  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, key_cmp,ht->buckets[bucket]);
  ioopm_entry_t *cur_entry = pre_entry->next;
  if (cur_entry != NULL)
  {
    if(0==key_cmp(cur_entry->key, key))
   {
    *success = true;
    return cur_entry->value;
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
  ioopm_eq_function_key key_cmp = ht->key_cmp;

  //ioopm_eq_function *value_cmp = ht->value_cmp;     TODO: uncomment when used
  int bucket = possitve_modulo(hash_func(key), ht->capacity);

  ioopm_entry_t *pre_entry= find_previous_entry_for_key(key, key_cmp, ht->buckets[bucket]);
  ioopm_entry_t *delete_entry = pre_entry->next;
  if(delete_entry == NULL || 0 != key_cmp(delete_entry->key, key)){ //check segfault
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
 //TODO: destroy bucket
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht){
  for(int i = 0; i<ht->capacity; ++i){
    bucket_destroy(ht->buckets[i]);
  }
  free(ht);
  ht=NULL;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < ht->capacity; ++i)
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
  for (int i = 0; i < ht->capacity; i++)              // Iterera över alla buckets
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
  if (no_keys==0) // empty hash table
  {
    //Kanske ska destory list? Det gjorde vi på förra
    return list;
  }
  for (int i = 0; i < ht->capacity; i++){ // iterate over array of buckets
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
  for (int i = 0; i < ht->capacity; i++){ // iterate over array of bucket
    entry = ht->buckets[i]->next;
    while (entry != NULL){
    ioopm_linked_list_append(list,entry->value);
    entry = entry->next;
    }  
  }
  return list;
}

static bool key_equiv(elem_t key, elem_t value, void *x,  ioopm_hash_table_t *ht ){
  ioopm_eq_function_key key_cmp = ht->key_cmp;
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return (0==key_cmp(key, other_key));
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
  for (size_t i = 0; i < ht->capacity; i++){
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
  for (size_t i = 0; i < ht->capacity; i++){
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
  for (size_t i = 0; i < ht->capacity; i++){
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


int main(void)
{ 
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function_int,string_eq,int_eq);

  ioopm_hash_table_insert(ht, int_elem(34),str_elem("tretiofyra"));
  ioopm_hash_table_insert(ht, int_elem(17), str_elem(NULL));
  ioopm_hash_table_insert(ht,int_elem(-17),str_elem("minus17"));
  ioopm_hash_table_insert(ht,int_elem(51),str_elem("femctio1"));
  ioopm_hash_table_insert(ht,int_elem(3),str_elem("tre"));
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("ett"));
  ioopm_hash_table_insert(ht,int_elem(0), str_elem("noll"));
//total of 7 entrys
  printf("%d, ",ht->buckets[1]->next->key.i);
  printf("%d, ",ht->buckets[3]->next->key.i);

  printf("Bucket[0] = ");
  printf("%d, ",ht->buckets[0]->next->key.i);
  printf("%d, ",ht->buckets[0]->next->next->key.i);
  printf("%d, ",ht->buckets[0]->next->next->next->key.i);
  printf("%d, ",ht->buckets[0]->next->next->next->next->key.i);
  printf("%d\n",ht->buckets[0]->next->next->next->next->next->key.i);


  //new bucket size = 31

  ioopm_hash_table_insert(ht,int_elem(31), str_elem("noll"));
  ioopm_hash_table_insert(ht,int_elem(62), str_elem("noll"));
  ioopm_hash_table_insert(ht,int_elem(93), str_elem("noll"));
  ioopm_hash_table_insert(ht,int_elem(124), str_elem("noll"));
  //12th entry
  ioopm_hash_table_insert(ht,int_elem(155), str_elem("noll"));

  ioopm_hash_table_insert(ht,int_elem(186), str_elem("noll"));

  //Threshold met here:
  ioopm_hash_table_insert(ht,int_elem(217), str_elem("noll"));
  //ioopm_hash_table_insert(ht,int_elem(248), str_elem("noll"));

  printf("Bucket[31] = ");
  //printf("%d, ",ht->buckets[17]->key.i);

  ioopm_hash_table_destroy(ht);
}