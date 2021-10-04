# inlupp1_ioopm
Inlupp 1 ioopm

**HOW TO USE THE PROGRAM**

# Compiling tests:
> make test_hash
> make test_linked
> make test_iterator
# Running tests:
> ./test

# Running valgrind:
> make valgrind_hash
> make valgrind_linked 
> make valgrind_iterator
# Running coverage:
> make coverage_hash
> make coverage_linked
> make coverage_iterator

Questions to be answered:
2. For each input, what are the top 3 functions in your code (that you have written), or is it library functions?


**TOPP 3 FUNCTIONS FOR EACH INPUT:**
Small.txt
1. string_eq - calls: 455
2.  key_equiv - calls: 351
3. find_previous_entry_for_key - calls: 54

  %   cumulative   self              self     total
 time   seconds   seconds    calls  Ts/call  Ts/call  name
  0.00      0.00     0.00      455     0.00     0.00  string_eq
  0.00      0.00     0.00      351     0.00     0.00  key_equiv
  0.00      0.00     0.00       54     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00       54     0.00     0.00  find_previous_link
  0.00      0.00     0.00       54     0.00     0.00  ioopm_linked_list_get
  0.00      0.00     0.00       54     0.00     0.00  possitve_modulo
  0.00      0.00     0.00       54     0.00     0.00  string_sum_hash
  0.00      0.00     0.00       44     0.00     0.00  entry_create

1k-long-words.txt
1. string_eq - calls: 16 088
2. key_equiv - calls: 12 063
3. find_previous_entry_for_key - calls: 2000

  %   cumulative   self              self     total
 time   seconds   seconds    calls  Ts/call  Ts/call  name
  0.00      0.00     0.00    16088     0.00     0.00  string_eq
  0.00      0.00     0.00    12063     0.00     0.00  key_equiv
  0.00      0.00     0.00     2000     0.00     0.00  find_previous_entry_for_key
  0.00      0.00     0.00     2000     0.00     0.00  possitve_modulo
  0.00      0.00     0.00     2000     0.00     0.00  string_sum_hash
  0.00      0.00     0.00     1000     0.00     0.00  ioopm_hash_table_any
  0.00      0.00     0.00     1000     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.00     0.00     1000     0.00     0.00  ioopm_hash_table_insert

10k-words.txt
1. ioopm_hash_table_any - calls: 10 000 (% time: 50.09, cumulative seconds 0.01, self seconds 0.01)
2. ioopm_hash_table_insert - calls: 10 000 (% time: 50.09, cumulative seconds 0.02, self seconds 0.01)
3. string_eq - calls: 1 150 089

  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 50.09      0.01     0.01    10000     0.00     0.00  ioopm_hash_table_any
 50.09      0.02     0.01    10000     0.00     0.00  ioopm_hash_table_insert
  0.00      0.02     0.00  1150089     0.00     0.00  string_eq
  0.00      0.02     0.00  1019530     0.00     0.00  key_equiv
  0.00      0.02     0.00    20000     0.00     0.00  find_previous_entry_for_key
  0.00      0.02     0.00    20000     0.00     0.00  possitve_modulo
  0.00      0.02     0.00    20000     0.00     0.00  string_sum_hash
  0.00      0.02     0.00    10000     0.00     0.00  ioopm_hash_table_has_key
  0.00      0.02     0.00    10000     0.00     0.00  ioopm_hash_table_lookup
  0.00      0.02     0.00    10000     0.00     0.00  process_word
  0.00      0.02     0.00     9801     0.00     0.00  update_entry_value

16k-words.txt - most of the words only appear 1-4 times
1. ioopm_hash_table_any - calls: 16 992 (% time: 23.85, cumulative seconds 0.15, self seconds 0.15, self ms/call 0.01, total ms/call 0.02)
2. find_previous_entry_for_key - calls: 33 984 (% time: 23.06, cumulative seconds 0.30, self seconds 0.15, self ms/call 0.00, total ms/call 0.00)
3. find_previous_link - calls: 8 740 (% time: 22.26, cumulative seconds 0.44, self seconds 0.14, self ms/call 0.02, total ms/call 0.02)


  %   cumulative   self              self     total
 time   seconds   seconds    calls  ms/call  ms/call  name
 23.85      0.15     0.15    16992     0.01     0.02  ioopm_hash_table_any
 23.06      0.30     0.15    33984     0.00     0.00  find_previous_entry_for_key
 22.26      0.44     0.14     8740     0.02     0.02  find_previous_link
 15.90      0.54     0.10 27000072     0.00     0.00  key_equiv
  6.36      0.58     0.04 29512872     0.00     0.00  string_eq
  1.59      0.59     0.01    16992     0.00     0.03  process_word
  1.59      0.60     0.01     4387     0.00     0.00  entry_create
  1.59      0.61     0.01       34     0.29     0.29  itterate_over_bucket
  1.59      0.62     0.01       17     0.59     0.59  bucket_destroy
  1.59      0.63     0.01                             main
  0.00      0.63     0.00    33984     0.00     0.00  possitve_modulo



**Are the top 3 functions in your code consistent across the inputs? Why? Why not?**
No, for smaller text files the the top-functions are the compare-functions and 
for larger text files the top-functions are search-functions.

For example, 1k-long-words consists of words that appear 38-45 times, the size of the hash table and linked list is relatively small due to
to limited amount of unique words. A short linked list leads to less time spent executing find_previous_link because
we have less words (linkes) to search. Search time for find_previous_link is O(n). The opposite applys for 16k-words.txt
which mostly consist of word that only appear 1-4 times, therefore we have a much greater amount of unique words.

Additionally, for the smaller textfiles we have an overall reduced running time, which therefore results in most functions "behaves" like O(1). Thus, the amount of calls is what determine a greater part of the running program's time.



**Is there some kind of trend?**
Similarily, what we mentions in the previous question. The trend is that the amount of calls are what mostly defines smaller text files. Whereas, longer text files are more dependant on the time complexity of the search functions.

The compare-functions, (key_equiv, string_eq etc.) are still the ones that have the most amount of calls, indepent of the file size, must as the text files become larger the time complexity increases for the search functions (find_previous_entry_for_key,find_previous_link).

**Do the results correspond with your expectations?**
 Yes, we expected both that ioopm_hash_table_insert, and it's callee function find_previous_entry_for_key, would take up a lot of the running time since it is used everytime we add a word to out hash table. But to our suprise only find_previous_entry_for_key was one of the top functions. This is because almost all of the time is spent in
 find_previous_entry_for_key when calling ioopm_hash_table_insert. Therefore, ioopm_hash_table_insert does not make that much of an impact on the total running time.
 
 Otherwise, the functions behaved as expected.

**Based on these results, do you see a way to make your program go faster?**
  !!!göra koller innan anrop t.ex.

 in process_word: (looked at the call graph to find)
 change:
  int freq = ioopm_hash_table_has_key(ht, str_elem(word))?   (ioopm_hash_table_lookup(ht,  str_elem(word), &success)).i:    0;
  ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(freq+1));
 
 to:
  int freq = (ioopm_hash_table_lookup(ht,  str_elem(word), &success)).i;
  success? ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(freq+1)) : ioopm_hash_table_insert(ht, str_elem(dup_word),int_elem(1));

  That way you do not need to call has_key, which might search an entire bucket if the key is not in the hash-table but insted use the 
  bool from lookup which tells ut if the return value is valid = the key exists.

  (NOTE TO SELF: kolla process_file)
  

