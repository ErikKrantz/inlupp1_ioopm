
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
C_GDB_TEST     = gcc -pedantic -Wall -g
C_UNIT_TEST    = gcc -ggdb -Wall -std=c11 -g
C_VALGRIND     = valgrind --leak-check=full --show-leak-kinds=all
C_COVERAGE_C   = gcc --coverage
C_COVERAGE     = gcov -abcfu

#-o gör en o fil som kan köras

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

runhash: hash_table.c linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) hash_table.c linked_list.c -o run

runlinked: linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) linked_list.c -o run

runiterator: iterator.c linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) iterator.c linked_list.c -o run

runread: readfile.c hash_table.c linked_list.c
	$(C_COMPILER) $(C_LINK_OPTIONS) readfile.c hash_table.c linked_list.c -o run
	
gdb_hash: hash_table.c linked_list.c 
	$(C_GDB_TEST) hash_table.c linked_list.c -o gdb_run

gdb_linked: linked_list.c
	$(C_GDB_TEST) linked_list.c -o gdb_run

gdb_read: readfile.c hash_table.c linked_list.c
	$(C_GDB_TEST) readfile.c hash_table.c linked_list.c -o gdb_run

test_hash: hash_table.c linked_list.c hash_table_test.c
	$(C_UNIT_TEST) hash_table.c linked_list.c hash_table_test.c -o test -lcunit

test_linked: linked_list.c linked_list_test.c
	$(C_UNIT_TEST) linked_list.c linked_list_test.c -o test -lcunit

test_iterator: iterator.c linked_list.c iterator_test.c
	$(C_UNIT_TEST) iterator.c linked_list.c iterator_test.c -o test -lcunit

valgrind_iterator: test_iterator
	$(C_VALGRIND) ./test -g

valgrind_linked: test_linked
	$(C_VALGRIND) ./test -g

valgrind_hash: test_hash
	$(C_VALGRIND) ./test -g

valgrind_read: runread
	$(C_VALGRIND) ./run -g

coverage_hash: hash_table_test.c
	$(C_COVERAGE_C) -o hash_cov hash_table_test.c hash_table.c linked_list.c -lcunit
	./hash_cov
	$(C_COVERAGE) hash_table_test.c
    
coverage_linked: linked_list_test.c
	$(C_COVERAGE_C) -o hash_cov linked_list_test.c linked_list.c -lcunit
	./hash_cov
	$(C_COVERAGE) linked_list_test.c

coverage_iterator: iterator_test.c iterator.c linked_list.c
	$(C_COVERAGE_C) -o hash_cov iterator_test.c iterator.c linked_list.c -lcunit
	./hash_cov
	$(C_COVERAGE) iterator_test.c




myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.

clean:
	rm -f *.o test run gdb_run linked_list.gcda linked_list.gcno hash_table.gcda hash_table.gcno hash_table_test.gcda hash_table_test.gcno iterator.gcda iterator.gcno iterator_test.gcda iterator_test.gcno iterator_test.c.gcov hash_table_test.c.gcov hash_table.c.gcov linked_list_test.c.gcov linked_list_test.gcda linked_list_test.gcno linked_list.gcov hash_cov linked_list.c.gcov gmon.out
