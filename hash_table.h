#pragma once
#include <stdbool.h>
#include "linked_list.h"


/*
 * @file hash_table.h
 * @author Elisabet Hansson & Erik Krantz
 * @date 1 Sep 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */


typedef struct entry ioopm_entry_t; //TODO make this private?
typedef struct hash_table ioopm_hash_table_t;

typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *extra, ioopm_hash_table_t *ht);
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

//returns 0 if equal, possitve if a>b, negative if a<b
typedef int(*ioopm_eq_function_key)(elem_t a, elem_t b);

///@brief Create a new hash table
/// @param hash_fun the hash function
/// @param elem_cmp the function to compare values
/// @param key_cmp the function to compare keys
//@return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(hash_function hash_func, ioopm_eq_function elem_cmp, ioopm_eq_function_key key_cmp);

///@brief Delete a hash table and free its memory
///@param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

///@brief add key => value entry in hash table ht
/// @param ht hash table operated upon
///@param key key to insert
///@param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

///@brief lookup value for key in hash table ht
///@param ht hash table operated upon
///@param key key to lookup
///@param succes bool will be changed to true if key exists, else false
///@return the value mapped to by key
elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, bool *success);

///@brief remove any mapping from key to a value
///@param ht hash table operated upon
///@param key key to remove
///@param succes bool will be changed to true if key exists, else false
///@return the value mapped to by key
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, bool *success);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return an array of keys for hash table ht
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return an array of values for hash table ht
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// @return true if ht contains the key, else false 
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @return true if ht contains the value, else false 
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if ht pred is true for all entrys, else false 
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return true if ht pred is true for any entry, else false 
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg);

/// @brief check the current capacity of hashtable
/// @param ht hash table operated upon
/// @param value the value sought
/// @return the capacity of ht 
size_t ioopm_hash_table_capacity(ioopm_hash_table_t *ht);
