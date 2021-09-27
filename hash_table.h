#pragma once
#include <stdbool.h>
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }
#define uns_int_elem(x) (elem_t) { .u=(x) }
#define bool_elem(x) (elem_t) { .b=(x) }
#define float_elem(x) (elem_t) { .f=(x) }

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

//the types from above
typedef struct entry_old ioopm_entry_t_old;
typedef struct hash_table_old ioopm_hash_table_t_old;
typedef bool(*ioopm_predicate)(int key, char *value, void *extra);       // change int char -> elem

typedef void(*ioopm_apply_function)(int key, char **value, void *extra); // change int char -> elem

//new generic typ
typedef union elem elem_t;
typedef struct entry ioopm_entry_t;
typedef struct hash_table ioopm_hash_table_t;

typedef bool(*ioopm_predicate_new)(elem_t key, elem_t value, void *extra);


typedef int(*hash_function)(elem_t a);

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);


//@brief Create a new hash table
//@return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create();

//@brief Delete a hash table and free its memory
//@param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

//@brief add key => value entry in hash table ht
//@param ht hash table operated upon
//@param key key to insert
//@param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value, ioopm_eq_function eq_func);

//@brief lookup value for key in hash table ht
//@param ht hash table operated upon
//@param key key to lookup
//@return the value mapped to by key (FIXME: incomplete)
elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, ioopm_eq_function eq_func, bool *success);

//@brief remove any mapping from key to a value
//@param ht hash table operated upon
//@param key key to remove
//@return the value mapped to by key (FIXME: incomplete)
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, ioopm_eq_function eq_func);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an array of keys for hash table h
int *ioopm_hash_table_keys(ioopm_hash_table_t_old *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
char **ioopm_hash_table_values(ioopm_hash_table_t_old *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t_old *ht, int key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t_old *ht, char *value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t_old *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate_new pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t_old *ht, ioopm_apply_function apply_fun, void *arg);
