######
# makefile for the "hashtable" system.
######
# $Revision:$
# $Date:$
#####

CC=cc
#CCOPTS=-I ../memcheck -DDEBUG -g
CCOPTS=-DDEBUG -g

SRC=hashtable.c

test:		testsuite
		./testsuite

testsuite:      test.c hashtable.o Makefile
		$(CC) $(CCOPTS) -o test test.c hashtable.o

hashtable.o:	$(SRC) Makefile
		$(CC) $(CCOPTS) -o hashtable.o -c $(SRC)

clean:
		rm -f hashtable.o *.o
