#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  //char c;
  char *s;
};

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void add_end_line(char *filename){
    FILE *file2 = fopen(filename, "a+");
    fprintf(file2, "\n");
    fclose(file2);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  bool success = true;
  char *dup_word = strdup(word);
  int freq = ioopm_hash_table_has_key(ht, str_elem(word))?   (ioopm_hash_table_lookup(ht,  str_elem(word), &success)).i:    0;
  ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(freq+1));
  //int freq = (ioopm_hash_table_lookup(ht,  str_elem(word), &success)).i;
  //success? ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(freq+1)) : ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(1));
  
}

void process_file(char *filename, ioopm_hash_table_t *ht){

  add_end_line(filename);

  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f))
    {
      free(buf);      
      break;
    }
    
    for (char *word = strtok(buf, Delimiters); word && *word; word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }

    free(buf);
    
  }

  fclose(f);
}

int string_sum_hash(elem_t e)
{
  char *str = e.s;
  int result = 0;
  do
    {
      result += *str;
    }
  while (*++str != '\0');
  return result;
}

int string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.s, e2.s);
}

bool int_eq(elem_t e1, elem_t e2)
{
  return (e1.i==e2.i);
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, int_eq, string_eq);
  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    size_t size = ioopm_linked_list_size(keys);

    char *keys_array[size];
    for (size_t i = 0; i < size ; i++)
    {
        keys_array[i] = ioopm_linked_list_get(keys,i).s;

    }
    sort_keys(keys_array, size);

    for (int i = 0; i < size; ++i) 
    {
      bool success = true;
      int freq = (ioopm_hash_table_lookup(ht, str_elem(keys_array[i]), &success)).i;
      printf("%s: %d\n", keys_array[i], freq);
    }

    
    for (int i = 0; i < size; i++)
    {
      ioopm_linked_list_get(keys,i);
    }
    
    ioopm_linked_list_destroy(keys);

  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }

  // FIXME: Leaks memory! Use valgrind to find out where that memory is 
  // being allocated, and then insert code here to free it.

  ioopm_hash_table_destroy(ht);

}

