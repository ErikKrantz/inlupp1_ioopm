
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
C_GDB_TEST     = gcc -pedantic -Wall -g
C_UNIT_TEST    = gcc -ggdb -Wall -std=c11 

#-o gör en o fil som kan köras

%.o:  %.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

myprog: hash_table.c
	$(C_COMPILER) $(C_LINK_OPTIONS) hash_table.c -o myprog

rungdb: hash_table.c
	$(C_GDB_TEST) hash_table.c -o -rungdb

unittest: hash_table.c test.c
	$(C_UNIT_TEST) hash_table.c test.c -o unittest -lcunit

myprog.final: file1.o file2.o file3.o
# TODO: add e.g. optimisation flags, remove unnecessary linking, etc.

clean:
	rm -f *.o myprog