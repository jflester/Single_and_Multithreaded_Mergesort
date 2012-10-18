CC = gcc
CFLAGS = -ansi -Wall -Werror -g
LDFLAGS = -lpthread
PROGS = mergetest public00 public01 public02 public03 \
	public04 public05 test

all: $(PROGS)

clean: 
	rm -f *.o $(PROGS)

$(PROGS): mergesort.o
mergetest: mergetest.o
public%: public%.o
test: test.o

mergetest.o: mergesort.h
mergesort.o: mergesort.h mergesort.c
public%.o: mergesort.h
test.o: mergesort.h