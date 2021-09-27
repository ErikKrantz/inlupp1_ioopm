
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "iterator.h"

typedef struct link link_t;
typedef union elem elem_t;
union elem
{
  /// TODO: update the names of the fields to something better? 
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
};

struct link
{
    elem_t element;
    link_t *next;
};

struct list
{
    link_t *first;
    link_t *last;
    ioopm_eq_function *eq_fun;
    int size;
};

struct list_iterator
{
    link_t *current;
    ioopm_list_t *list;
};

// TODO Global errno value

extern int errno;

// Checks if iterator has a next value
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter){

    if (iter->current->next != NULL){
        return true;
    }

    return false;
}


// Will return its current element if next does not exist
elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter){
    if (ioopm_iterator_has_next(iter)){
        //Update pointer
        iter->current = iter->current->next;

        //Return element of updated pointer;
        return iter->current->element;
    }
    // No next can be found the
    errno = EIO;

    return iter->current->element;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter){
    iter->current=iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter){
    return iter->current->element;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter){
    free(iter);
    iter = NULL;
}

/*
int main(int argc, char const *argv[])
{
    ioopm_list_t *list = ioopm_linked_list_create();
    //ioopm_list_insert(list, int_elem(10));
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    ioopm_iterator_next(iter);
    
    if(errno==EIO){
        puts("Error check");
    }
    return 0;
}
*/