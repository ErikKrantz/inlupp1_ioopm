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

void ioopm_linked_list_destroy(ioopm_list_t *list){
    //Just nu antar funktionen att det alltid finns en dummy
    link_t *current_link = list->first;
    link_t *next_link = current_link->next;
    while(next_link!=NULL){
        current_link = NULL;
        free(current_link);
        current_link = next_link;
        next_link = current_link->next;
    }
    current_link=NULL;
    free(current_link);
    free(list);
}

link_t *create_link(int elem, link_t *pointer){
    link_t *entry = calloc(1,sizeof(link_t));
    entry->element = elem;
    entry->next = pointer;
    return entry;
    }

void ioopm_linked_list_append(ioopm_list_t *list, int value){
    link_t *last = list->last;
    link_t *new_last = create_link(value,NULL);
    last->next = new_last;
    list->last = new_last;
    }

void main(){
    printf("hej\n");
}