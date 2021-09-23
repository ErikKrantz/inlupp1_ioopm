#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

typedef int elem_t;
typedef struct link link_t; /// Meta: struct definition goes in C file

struct link 
{
    elem_t element;
    link_t *next;
};

struct list
{
  link_t *first;
  link_t *last;
};

ioopm_list_t *ioopm_linked_list_create(){
    ioopm_list_t *list = calloc(1,sizeof(ioopm_list_t));
    link_t *dummy = calloc(1,sizeof(link_t));
    dummy->element = 0;
    dummy->next = NULL;
    list->first = dummy;
    list->last = dummy;
    return list;
}

void destroy_link (link_t *link){
    free(link);
    link = NULL;
}

void ioopm_linked_list_destroy(ioopm_list_t *list){
    //Just nu antar funktionen att det alltid finns en dummy
    link_t *current_link = list->first;
    link_t *next_link;
    while(current_link!=NULL){
        next_link = current_link->next;
        destroy_link(current_link);
        current_link = next_link;
    }
    free(list);
    list = NULL;
}

link_t *create_link(int elem, link_t *pointer){
    link_t *entry = calloc(1,sizeof(link_t));
    entry->element = elem;
    entry->next = pointer;
    return entry;
    }

void ioopm_linked_list_append(ioopm_list_t *list, int value){
    link_t *new_last = create_link(value,NULL);
    list->last->next = new_last;
    list->last = new_last;
    }

void ioopm_linked_list_prepend(ioopm_list_t *list, int value){
    link_t *first_link = create_link(value,list->first->next); //first element is dummy
    list->first->next = first_link;
    if(first_link->next==NULL){ //om vi lägger till ett link först i en tom lista blir den också sist
        list->last = first_link;
    }
}

// find_prev_link antar ett korrekt index
static link_t *find_previous_link(ioopm_list_t *list, int index){
    link_t *pre_link = list->first;
    for(int i = 0; i < index; i++){
        pre_link = pre_link->next;
    }
    return pre_link;
}
//funktionen antar ett correct index
void ioopm_linked_list_insert(ioopm_list_t *list, int index, int value){
   
    link_t *pre_link = find_previous_link(list,index);

    if(pre_link->next == NULL){ //ifall vi ska lägga till en link sist i listan
        ioopm_linked_list_append(list,value);
        return;
    }
    link_t *new_link = create_link(value,pre_link->next);
    pre_link->next = new_link;
}

int ioopm_linked_list_remove(ioopm_list_t *list, int index){
    link_t *pre_link = find_previous_link(list, index);

    // hitta linken som ska tas bort
    link_t *remove_link = pre_link->next;
    // kolla ifall remove link är last
    if (remove_link->next==NULL){
        list->last = pre_link;   
        pre_link->next = NULL;
        int value = remove_link->element;
        destroy_link(remove_link);
        return value;
    }

    // ersätt länken innans next med den
    pre_link->next = remove_link->next;

    int value = remove_link->element;

    // destroy the link
    destroy_link(remove_link);

    // returnera elementet för remove_link
    return value;
}

// Only accepts valid indexes
int ioopm_linked_list_get(ioopm_list_t *list, int index){
    link_t *pre_link = find_previous_link(list, index);

    int element = pre_link->next->element;

    return element;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, int element){

    link_t *link = list->first;
    do{
        link = link->next;
        if (link->element == element){
            return true;
        }
    }while (link->next != NULL);
    return false;
}

int ioopm_linked_list_size(ioopm_list_t *list){
    int c = 0;
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

static bool ele_exist(int element, void *x){
  int *other_ele_ptr = x;
  int other_ele = *other_ele_ptr;
  return element == other_ele;
}


static void change_element (int *element, void *x){
  int *other_ele_ptr = x;
  int other_ele = *other_ele_ptr;
  *element = other_ele;
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

void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function fun, void *extra){
    if (ioopm_linked_list_is_empty(list)) {
        return;
    }

    link_t *current_link = list->first->next;
    do
    {   
        fun(&current_link->element, extra);
        current_link = current_link->next;

    } while (current_link != NULL);

}

int main(){
  ioopm_list_t *list = ioopm_linked_list_create();
  if(ioopm_linked_list_is_empty(list)) printf("List is empty\n");
  ioopm_linked_list_append(list,4);
  if(list->last->element == 4){
      printf("Success append!\n");
  }

  ioopm_linked_list_prepend(list,3);
  if(list->first->next->element == 3){
      printf("Success prepend!\n");
  }

  ioopm_linked_list_insert(list,0,1);
  if(list->first->next->element == 1){
      printf("Success insert first!\n");
  }
  ioopm_linked_list_insert(list,1,2);
  if(list->first->next->next->element == 2){
      printf("Success insert middle!\n");
  }
  
  ioopm_linked_list_insert(list,4,15);
  if(list->last->element == 15){
      printf("Success insert last!\n");
  }
  

  int value_removed = ioopm_linked_list_remove(list,4);

  printf("%d\n", value_removed);

  if(list->last->element != 15)
{
    puts("Successfullt removed last");
}

int value_index = ioopm_linked_list_get(list,3);
printf("Value at index 3 = %d\n", value_index);

// 5 8 10 7

bool contains;
contains = ioopm_linked_list_contains(list, 1);
printf("%d\n", contains);
contains =ioopm_linked_list_contains(list, 2);
printf("%d\n", contains);
contains =ioopm_linked_list_contains(list, 3);
printf("%d\n", contains);
contains =ioopm_linked_list_contains(list, 4);
printf("%d\n", contains);
int list_size = ioopm_linked_list_size(list);
printf("List size = %d\n", list_size);
if(!ioopm_linked_list_is_empty(list)) printf("List is not empty\n");

int ele = 88;
bool any_test = ioopm_linked_list_any(list,ele_exist, &ele);
if (any_test){
    puts("Success any test");
}

ioopm_linked_apply_to_all(list,change_element,&ele);
int x = list->first->next->element;
printf("%d\n",x);

x = list->first->next->next->element;
printf("%d\n",x);

x = list->first->next->next->next->element;
printf("%d\n",x);

x = list->first->next->next->next->next->element;

printf("%d\n",x);
bool all_test = ioopm_linked_list_all(list,ele_exist, &ele);
if (all_test){
    puts("Success all test");
}
ioopm_linked_list_clear(list);
list_size = ioopm_linked_list_size(list);
printf("List size = %d\n", list_size);
if(ioopm_linked_list_is_empty(list)) printf("List is empty\n");

ioopm_linked_list_destroy(list);
  return 0;
}