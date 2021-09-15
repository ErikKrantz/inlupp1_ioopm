#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"
//#include <errno.h>

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

//the types from above
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;


entry_t *entry_create(int key, char *value, entry_t *next)
{
    entry_t *entry = calloc(1, sizeof(entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to entry_t's
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  /// Initialise the entry_t pointers to NULL
  for (int i = 0; i < 17; ++i)
    {
      result->buckets[i] = NULL;
    }
   /// Get a pointer to the first entry in the bucket
  for (int i = 0; i < 17; i++)
  {
    result->buckets[i] = entry_create(0,NULL,NULL);
  }
  return result;
}
/*

Returnerar NULL om entry ska läggas först i bucketen,
annars returnerar entry för den som ligger före
*/

entry_t *find_previous_entry_for_key(int key, entry_t *cursor)
{
  entry_t *tmp = cursor->next;
  if (tmp == NULL)
  {
    return cursor;
  }
 while (tmp != NULL)
 {
    if (tmp->key == key){
      return cursor;
      }
  cursor = cursor->next;
  tmp = cursor->next;
 }
  return cursor;
}

int possitve_modulo (int n, int mod){
  n = n % mod;
  if(n<0){
    n += mod;
  }
  return n;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value){
  /// Calculate the bucket for this entry
  int bucket = possitve_modulo(key, 17);

  entry_t *pre_entry = find_previous_entry_for_key(key, ht->buckets[bucket]);//en av två alternativ, kopia vs adress
  entry_t *cur_entry;

  if(pre_entry->next != NULL){
    if( key == pre_entry->next->key){
    cur_entry = pre_entry->next;
    cur_entry->value = value;
    }
  }
  cur_entry = entry_create(key,value,pre_entry->next);
  pre_entry->next = cur_entry;
}

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, bool *success){
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(key,ht->buckets[key % 17]);
  entry_t *next = tmp->next;
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

void entry_destroy(entry_t *entry){
    free(entry);
}

char *ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key){
  entry_t *pre_entry= find_previous_entry_for_key(key,ht->buckets[possitve_modulo(key, 17)]);
  entry_t *delete_entry = pre_entry->next;
  if(delete_entry ==NULL){
    return NULL;
  }
  entry_t *next_entry = delete_entry->next;
  pre_entry->next = next_entry; // uppdaterar next i pre_entry
  char *value_deleted = delete_entry->value;
  entry_destroy(delete_entry); // Kanske behövs mer free för (value/key/next)
  return value_deleted;
}
/*

int main(int argc, char *argv[])
{
  
  entry_t *a = entry_create(54, "femtifyra", NULL);
  entry_t *b = entry_create(37, "tresju",a);
  entry_t *c = entry_create(20, "tjugo",b);
  entry_t *d = entry_create(0, NULL, c);

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ht->buckets[3] = d;
  ioopm_hash_table_insert(ht,3,"tr3" );
  ioopm_hash_table_insert(ht,54,"femti4" );
  //ioopm_hash_table_insert(ht,1,"tr3" );
  bool *success = calloc(1, sizeof(bool));
  char *result = ioopm_hash_table_lookup(ht, 54, success);
  if (*success && result != NULL){
  printf("%s\n", result);
  }
  if (!*success){
    // Här kan vi byta EINVAL (errno) till rimligt error message
  printf("done\n");
  }
  free(success);
  return 0;
}
*/
