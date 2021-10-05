#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

typedef struct link link_t;

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char c;
  char *s;
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
    ioopm_eq_function eq_func;
    size_t size;
};

struct list_iterator
{
    link_t *current;
    ioopm_list_t *list;
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list){
    ioopm_list_iterator_t *iter = calloc(1,sizeof(ioopm_list_iterator_t));

    iter->current = list->first;
    iter->list = list;

    return iter;
}

//Creates a new link
static link_t *create_link(elem_t elem, link_t *pointer){
    link_t *entry = calloc(1,sizeof(link_t));
    entry->element = elem;
    entry->next = pointer;
    return entry;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func){
    ioopm_list_t *list = calloc(1,sizeof(ioopm_list_t));
    link_t *dummy = create_link(int_elem(0),NULL);
    list->first = dummy;
    list->last = dummy;
    list->size = 0;
    list->eq_func = eq_func;
    return list;
}

//Frees allocated memory by a link
static void destroy_link (link_t *link){
    free(link);
    link = NULL;
}

void ioopm_linked_list_destroy(ioopm_list_t *list){
    link_t *current_link = list->first;
    link_t *next_link = NULL;
    while(current_link!=NULL){
        next_link = current_link->next;
        destroy_link(current_link);
        current_link = next_link;
    }
    free(list);
    list = NULL;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value){
    link_t *new_last = create_link(value,NULL);
    list->last->next = new_last;
    list->last = new_last;
    list->size +=1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value){
    link_t *first_link = create_link(value,list->first->next); //first element is dummy
    list->first->next = first_link;
    if(first_link->next==NULL){ //when prepending a link to en empty list, that link becomes last as well
        list->last = first_link;
    }
    list->size +=1;
}

// Finds a previous link based on index, assumes that index exists
static link_t *find_previous_link(ioopm_list_t *list, int index){
    link_t *pre_link = list->first;
    for(int i = 0; i < index; i++){
        pre_link = pre_link->next;
    }
    return pre_link;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value){
   
    link_t *pre_link = find_previous_link(list,index);
    list->size +=1;
    if(pre_link->next == NULL){ //ifall vi ska lägga till en link sist i listan
        ioopm_linked_list_append(list,value);
        return;
    }
    link_t *new_link = create_link(value,pre_link->next);
    pre_link->next = new_link;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index){
    link_t *pre_link = find_previous_link(list, index);

    link_t *remove_link = pre_link->next;

    list->size -=1;

    // if remove_link is last, also change last pointer
    if (remove_link->next==NULL){
        list->last = pre_link;   
        pre_link->next = NULL;
        elem_t value = remove_link->element;
        destroy_link(remove_link);
        return value;
    }

    // remove link from list att free allocated memory
    pre_link->next = remove_link->next;
    elem_t value = remove_link->element;
    destroy_link(remove_link);

    return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index){
    link_t *pre_link = find_previous_link(list, index);

    elem_t element = pre_link->next->element;

    return element;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element){
  ioopm_eq_function cmp = list->eq_func;
  for (link_t *cursor = list->first->next; cursor; cursor = cursor->next)
    {
      if (cmp(cursor->element, element)) return true;
    }
  return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list){
    size_t c = 0;
    link_t *link = list->first;
    while (link->next != NULL)
    {
        ++c;
        link = link->next;
    }
    return c;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list){
    return 0 == ioopm_linked_list_size(list);
}


void ioopm_linked_list_clear(ioopm_list_t *list){
    link_t *current_link = list->first->next;
    link_t *next_link;

    while(current_link != NULL){
        next_link = current_link->next;
        destroy_link(current_link);
        current_link = next_link;
    }
    list->first->next = NULL;
    list->last = list->first;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_char_predicate prop, void *extra){
    if (ioopm_linked_list_is_empty(list)) {
        return false;
    }
    link_t *current_link = list->first->next;
    do
    {
        if(!prop(current_link->element, extra)){
            return false;
        }
        current_link = current_link->next;

    } while (current_link != NULL);
    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_char_predicate prop, void *extra){
    if (ioopm_linked_list_is_empty(list)) {
        return false;
    }

    link_t *current_link = list->first->next;
    do
    {
        if(prop(current_link->element, extra)){
            return true;
        }
        current_link = current_link->next;

    } while (current_link != NULL);
    return false;
}

void ioopm_linked_apply_to_all(ioopm_list_t *list, apply_fun_t func, void *extra){
    for (link_t *cursor = list->first->next; cursor; cursor = cursor->next)
    {
        func(&cursor->element, extra);
    }
}
