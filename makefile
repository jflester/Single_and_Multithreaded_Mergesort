CC = gcc
CFLAGS = -ansi -Wall -Werror -g
LDFLAGS = -lpthread
PROGS = mergetest test

all: $(PROGS)

clean: 
	rm -f *.o $(PROGS)

$(PROGS): mergesort.o
mergetest: mergetest.o
test: test.o

mergetest.o: mergesort.h
mergesort.o: mergesort.h mergesort.c
test.o: mergesort.h