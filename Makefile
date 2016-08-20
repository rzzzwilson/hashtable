######
# makefile for the "hashtable" system.
######
# $Revision:$
# $Date:$
#####

define colorecho
    @tput setaf 6
    @echo $1
    @tput sgr0
endef

CC=cc
#CCOPTS=-I ../memcheck -DDEBUG -g
#CCOPTS=-DDEBUG -g
CCOPTS=

SRC=hashtable.c

test:		testsuite
		bash testsuite

testsuite:      test.c hashtable.o Makefile
		$(call colorecho,"Compiling with" $(CC))
		$(CC) $(CCOPTS) -o test test.c hashtable.o

hashtable.o:	$(SRC) Makefile
		$(call colorecho,"Compiling with" $(CC))
		$(CC) $(CCOPTS) -o hashtable.o -c $(SRC)

clean:
		rm -f hashtable.o *.o test
