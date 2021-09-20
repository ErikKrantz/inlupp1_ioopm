#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"
#define No_Buckets 17
#define _XOPEN_SOURCE 700
//#include <errno.h>

//the types from above
typedef struct entry ioopm_entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  ioopm_entry_t *buckets[No_Buckets];
};


static ioopm_entry_t *entry_create(int key, char *value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create()
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
    result->buckets[i] = entry_create(0,NULL,NULL);
  }
  return result;
}
/*

Returnerar NULL om entry ska läggas först i bucketen,
annars returnerar entry för den som ligger före
*/ 

static ioopm_entry_t *find_previous_entry_for_key(int key, ioopm_entry_t *cursor)
{
  ioopm_entry_t *tmp = cursor->next;
  if (tmp == NULL)
  {
    return cursor;
  }
 while (tmp != NULL)
 {
    if (tmp->key == key || tmp->key > key){
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

static void update_entry_value(char* value, ioopm_entry_t *entry, int key){
    if(key == entry->key){ //key == cur_entry->key
    entry->value = value; //ändrar value på en nyckel som finns
    }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value){
  /// Calculate the bucket for this entry
  int bucket = possitve_modulo(key, No_Buckets);

  ioopm_entry_t *pre_entry = find_previous_entry_for_key(key, ht->buckets[bucket]);//en av två alternativ, kopia vs adress
  ioopm_entry_t *cur_entry;

  if(pre_entry->next != NULL){ //"om vi inte är i slutet av listan, alltså finns nyckeln"
    update_entry_value(value, pre_entry->next, key);
  }
  cur_entry = entry_create(key,value,pre_entry->next); //
  pre_entry->next = cur_entry;
}

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, bool *success){
  /// Find the previous entry for key
  ioopm_entry_t *tmp = find_previous_entry_for_key(key,ht->buckets[key % No_Buckets]);
  ioopm_entry_t *next = tmp->next;
  if (next != NULL)
  {
    if(next->key == key)
   {
    *success = true;
    return next->value;
   }
  }
  *success = false;
  return NULL;
}

static void entry_destroy(ioopm_entry_t *entry){
    free(entry);
}

char *ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key){
  ioopm_entry_t *pre_entry= find_previous_entry_for_key(key,ht->buckets[possitve_modulo(key, No_Buckets)]);
  ioopm_entry_t *delete_entry = pre_entry->next;
  if(delete_entry ==NULL){
    return NULL;
  }
  ioopm_entry_t *next_entry = delete_entry->next;
  pre_entry->next = next_entry; // uppdaterar next i pre_entry
  char *value_deleted = delete_entry->value;
  entry_destroy(delete_entry); // Kanske behövs mer free för (value/key/next)
  return value_deleted;
}

static void bucket_destroy(ioopm_entry_t *current_entry)
{
  ioopm_entry_t *next_entry;
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
}

void itterate_over_bucket(int *counter, ioopm_entry_t *entry)
{
  while (entry->next != NULL)
  {
    ++*counter;
    entry = entry->next;
  }
}
   
int ioopm_hash_table_size(ioopm_hash_table_t *ht){
 int c = 0;
 for (int i = 0; i < No_Buckets; i++)
 {
   ioopm_entry_t *first_entry = ht->buckets[i];
   itterate_over_bucket(&c, first_entry);
 }
 return c;
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

// TODO: REMOVE (helper test function)
void print_array(char **pointer)
{
  printf("[ ");
  while (*pointer != NULL)
  {
    printf("%s ", *pointer);
    pointer++;
  };
  puts("]");
}

int *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  int no_keys = ioopm_hash_table_size(ht);
  int *keys = calloc(no_keys, sizeof(int));
  int *tmp_adress = keys;
  ioomp_entry_t *entry;
  if (no_keys==0) // empty hash table
  {
    return keys;
  } 
  for (int i = 0; i < No_Buckets; i++){ // iterate over array of buckets
     entry = ht->buckets[i]->next;
     while (entry != NULL){
      *tmp_adress = entry->key;
      ++tmp_adress;
      entry = entry->next;
    }
  }
  return keys;
}

/*
    Denna kan användas för M39
*/
char **ioopm_hash_table_values(ioopm_hash_table_t *ht){
  int no_values = ioopm_hash_table_size(ht);
  char **values = calloc(no_values+1, sizeof(char*)); //values is a pointer to the first element, which is a pointer to the first char in a string
  char **tmp_adress = values;
  ioomp_entry_t *entry;
  if (no_values==0) // empty hash table
  {
    *values = NULL;
    return values;
  } 
  for (int i = 0; i < No_Buckets; i++){ // iterate over array of bucket
    entry = ht->buckets[i]->next;
    while (entry != NULL){
    *tmp_adress = entry->value;
    ++tmp_adress;
    entry = entry->next;
    }  
  }
  values[no_values] = NULL;
  return values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, int key){
  bool success;
  ioopm_hash_table_lookup(ht,key,&success);
  return success;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, char *value){
  char **values = ioopm_hash_table_values(ht);
  char **adress = values;
  while(*adress != NULL)
  {
    if (!strcmp(*adress,value))
    {
      free(values);
      return true;
    }
    ++adress;
  }
  free(values);
  return false;
}

/*
int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
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

