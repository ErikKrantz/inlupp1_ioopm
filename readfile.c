#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "hash_table.h"
#include <unistd.h> // TODO remove

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
/*
int convert_str_to_key (elem_t a){
    return 1;
}*/

bool value_compare(elem_t a, elem_t b){
    return a.i == b.i;
}

int convert_str_to_key(elem_t a){
    int result = 0;
    char *str = a.s;
    do
        {
        result += *str;
        }
    while (*++str != '\0');
    return result;
}

static int cmpstringp(const void *p1, const void *p2){  
    return strcmp(*(char *const *)p1, *(char *const *)p2);
    }

void print_all(ioopm_hash_table_t *ht){
    
    size_t size = ioopm_hash_table_size(ht);
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    //create an array keys
    size_t no_keys = ioopm_linked_list_size(keys);
    char *keys_array[no_keys];
    for (size_t i = 0; i < no_keys ; i++)
    {
        keys_array[i] = ioopm_linked_list_get(keys,i).s;
    }
    //sort the keys
    qsort(keys_array,no_keys,sizeof(char*),cmpstringp);

    //print all keys with corresponding value
    for (int i = 0; i<size; i++){
        char *key = keys_array[i];
        bool success = true;
        int value = ioopm_hash_table_lookup(ht, str_elem(key), &success).i;
        printf("%s : %d\n", key, value);
    }
    ioopm_linked_list_destroy(keys);
}

void add_word(char *word,ioopm_hash_table_t *ht)
{
    // If word exist, increment occurence by +1
    if(ioopm_hash_table_has_key(ht, str_elem(word))){
       bool success = true;
       elem_t occurence = ioopm_hash_table_lookup(ht, str_elem(word), &success);
       ++occurence.i;
       ioopm_hash_table_insert(ht,str_elem(strdup(word)),occurence);
    }
    // If word does not exist, add a new element with value = word and key = 1  
    else{
    ioopm_hash_table_insert(ht, str_elem(strdup(word)), int_elem(1));
    }
}

void create_hash_for_file(char *str,ioopm_hash_table_t *ht){
   const char *s = "+-#@()[]{}.,:;!? \t\n\r";
   char *token = strtok(str, s);
      
   /* walk through other tokens */
   while( token != NULL ) {
      add_word(token,ht);
      token = strtok(NULL, s);
      //free(token);
   }
   //free(str);
   //free(token);
}

void read_file(char *filename, ioopm_hash_table_t *ht){
    FILE *file = fopen(filename,"r");
    while(true){
        char *result = NULL;
        size_t size = 0;
        getline(&result, &size, file);
        create_hash_for_file(result,ht);     

        if (feof(file)){
            free(result);
            fclose(file);
            break;
        }
        free(result);
    }
}

int main(int argc, char *argv[]){

    ioopm_hash_table_t *ht = ioopm_hash_table_create(convert_str_to_key,value_compare);

    int file_amount = argc;
    
    for (int i = 1; i<file_amount; i++){
        read_file(argv[i],ht);
    }
    
    print_all(ht);
    
    ioopm_hash_table_destroy(ht);
    return 0;
}

