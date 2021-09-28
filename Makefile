
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
C_GDB_TEST     = gcc -pedantic -Wall -g
C_UNIT_TEST    = gcc -ggdb -Wall -std=c11 -g
C_VALGRIND     = valgrind --leak-check=full

#-o gör en o fil som kan köras

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

runhash: hash_table.c linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) hash_table.c linked_list.c -o runhash

runlinked: linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) linked_list.c -o runlinked

runiterator: iterator.c linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) iterator.c linked_list.c -o runiterator

gdb_hash: hash_table.c
	$(C_GDB_TEST) hash_table.c -o gdb_hash

gdb_linked: linked_list.c
	$(C_GDB_TEST) linked_list.c -o gdb_linked

test_hash: hash_table.c hash_table_test.c
	$(C_UNIT_TEST) hash_table.c hash_table_test.c -o test_hash -lcunit

test_linked: linked_list.c linked_list_test.c
	$(C_UNIT_TEST) linked_list.c linked_list_test.c -o test_linked -lcunit

test_iterator: iterator.c linked_list.c iterator_test.c
	$(C_UNIT_TEST) iterator.c linked_list.c iterator_test.c -o test_iterator -lcunit

valgrind_iterator: test_iterator
	$(C_VALGRIND) ./test_iterator -g

valgrind_linked: test_linked
	$(C_VALGRIND) ./test_linked -g

valgrind_hash: test_hash
	$(C_VALGRIND) ./test_hash -g





myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.

clean:
	rm -f *.o myprog