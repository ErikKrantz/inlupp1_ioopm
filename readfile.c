#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

//compares p1 and p2 as strings
static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

//Sorts keys in lexicon order
void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

//adds a last blank line to filename
void add_end_line(char *filename){
    FILE *file2 = fopen(filename, "a+");
    fprintf(file2, "\n");
    fclose(file2);
}

//adds word to ht
void process_word(char *word, ioopm_hash_table_t *ht)
{
  bool success = true;
  char *dup_word = strdup(word);
  int freq = ioopm_hash_table_has_key(ht, str_elem(word))?   (ioopm_hash_table_lookup(ht,  str_elem(word), &success)).i:    0;
  ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(freq+1));  
}

//Opens a file, extract all words and characters and add them to a hash table, and closes file
void process_file(char *filename, ioopm_hash_table_t *ht){

  add_end_line(filename);

  FILE *f = fopen(filename, "r");

  while (true){
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f)){
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

//A hash function for strings to int
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

//Returns 0 if e1==e2, 1 if e1 > e2 and -1 if e1<e2
int string_eq(elem_t e1, elem_t e2)
{
  return strcmp(e1.s, e2.s);
}

//Checks if e1 and e2 are equal
bool int_eq(elem_t e1, elem_t e2)
{
  return (e1.i==e2.i);
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, int_eq, string_eq);
  if (argc > 1)
  {
    //Proccess one file at a time, and adds their words to a chared hash table
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
    
    ioopm_linked_list_destroy(keys);

  }
  else{
    puts("Usage: freq-count file1 ... filen");
  }
  ioopm_hash_table_destroy(ht);
}

